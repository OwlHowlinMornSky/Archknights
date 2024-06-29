/*
*    Archknights
*
*    Copyright (C) 2023-2024  Tyler Parret True
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Affero General Public License as published
*    by the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Affero General Public License for more details.
*
*    You should have received a copy of the GNU Affero General Public License
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
* @Authors
*    Tyler Parret True <mysteryworldgod@outlook.com><https://github.com/OwlHowlinMornSky>
*/
#ifdef ARCHKNIGHTS_LIMITED

#define SPINE_SHORT_NAMES

#ifndef SPINE_MESH_VERTEX_COUNT_MAX
#define SPINE_MESH_VERTEX_COUNT_MAX 1000
#endif

#include <GL/glew.h>

#include "AnimationSpine.h"
#include <spine/TextureLoader.h>
#include <spine/BlendMode.h>
#include <spine/Bone.h>
#include <spine/SlotData.h>
#include <spine/Attachment.h>
#include <spine/RegionAttachment.h>
#include <spine/MeshAttachment.h>
#include <spine/ClippingAttachment.h>
#include <spine/SkeletonData.h>
#include <spine/SkeletonBinary.h>
#include <spine/AnimationStateData.h>

#include <MysteryEngine/G3D/Shader.h>
#include <MysteryEngine/G3D/Camera.h>
#include <MysteryEngine/G3D/GlCheck.h>
#include <MysteryEngine/G3D/G3dGlobal.h>

namespace {

constexpr float spine_to3d_scale_i = 256.0f;
//constexpr float spine_global_scale = 0.7125f;
constexpr float outline_thickness  = 0.02f;
constexpr float halfsqrt2 = 0.70710678118654752440084436210485f;
const float CircleOffsetX[8] = {
	1.0f * outline_thickness,
	halfsqrt2 * outline_thickness,
	-0.0f * outline_thickness,
	-halfsqrt2 * outline_thickness,
	-1.0f * outline_thickness,
	-halfsqrt2 * outline_thickness,
	0.0f * outline_thickness,
	halfsqrt2 * outline_thickness
};
const float CircleOffsetY[8] = {
	0.0f * outline_thickness,
	halfsqrt2 * outline_thickness,
	1.0f * outline_thickness,
	halfsqrt2 * outline_thickness,
	-0.0f * outline_thickness,
	-halfsqrt2 * outline_thickness,
	-1.0f * outline_thickness,
	-halfsqrt2 * outline_thickness
};

/// <summary>
/// 矩形的 index
/// </summary>
spine::Vector<unsigned short> quadIndices;

class SFMLTextureLoader :
	public spine::TextureLoader {
public:
	virtual void load(spine::AtlasPage& page, const spine::String& path) {
		sf::Texture* texture = new sf::Texture();
		if (!texture->loadFromFile(path.buffer())) return;

		if (page.magFilter == spine::TextureFilter_Linear) texture->setSmooth(true);
		if (page.uWrap == spine::TextureWrap_Repeat && page.vWrap == spine::TextureWrap_Repeat) texture->setRepeated(true);

		page.setRendererObject(texture);
		sf::Vector2u size = texture->getSize();
		page.width = size.x;
		page.height = size.y;
	}

	virtual void unload(void* texture) {
		delete (sf::Texture*)texture;
	}

public:
	static SFMLTextureLoader instance; // 基类和本类构造函数什么都不干，因此可以为全局。
};
SFMLTextureLoader SFMLTextureLoader::instance;

} // end namespace

namespace Model {

// class SpineAnimation
SpineAnimation::SpineAnimation(const Model::SpinePoseData _pose) :
	m_pose(_pose),
	m_outline(false),

	m_color(1.0f),

	m_vertexVBO(0),
	m_vao(0)

{
	// Create the skeleton
	m_skeleton = new spine::Skeleton(m_pose.skeletonData);
	// Create the animation state
	m_animationState = new spine::AnimationState(m_pose.animationStateData);

	worldVertices.clear();
	worldVertices.ensureCapacity(SPINE_MESH_VERTEX_COUNT_MAX);

	m_bonesRef = &m_skeleton->getBones();

	setRotation(30.0f, 0.0f, 0.0f);
	return;
}

SpineAnimation::~SpineAnimation() {
	Clear();
	delete m_animationState;
	delete m_skeleton;
	return;
}

bool SpineAnimation::Setup() {
	ME::G3dGlobal::setActive(true);

	glCheck(glGenVertexArrays(1, &m_vao));
	glCheck(glBindVertexArray(m_vao));
	glCheck(glGenBuffers(1, &m_vertexVBO));
	glCheck(glBindVertexArray(0));

	ME::G3dGlobal::setActive(false);
	return true;
}

void SpineAnimation::Clear() {
	if (m_vertexVBO) {
		glCheck(glDeleteBuffers(1, &m_vertexVBO));
		m_vertexVBO = 0;
	}
	if (m_vao) {
		glCheck(glDeleteVertexArrays(1, &m_vao));
		m_vao = 0;
	}
	return;
}

void SpineAnimation::Update(float dt) {
	m_skeleton->update(dt);
	m_animationState->update(dt);
	m_animationState->apply(*m_skeleton);
	m_skeleton->updateWorldTransform();
	return;
}

void SpineAnimation::Draw(ME::Camera* camera, ME::Shader* shader) {
	vertexArray.clear();

	// Early out if skeleton is invisible
	if (m_skeleton->getColor().a == 0.0f)
		return;

	UpdateShader(shader, camera);

	spine::BlendMode previousBlend;
	sf::Texture* previousTexture = nullptr;
	sf::Texture* texture = nullptr;
	for (size_t i = 0, n = m_skeleton->getSlots().size(); i < n; ++i) {
		spine::Slot& slot = *m_skeleton->getDrawOrder()[i];
		spine::Attachment* attachment = slot.getAttachment();
		if (!attachment) continue;

		// Early out if the slot color is 0 or the bone is not active
		if (slot.getColor().a == 0 || !slot.getBone().isActive()) {
			m_clipper.clipEnd(slot);
			continue;
		}

		spine::Vector<float>* vertices = &worldVertices;
		size_t verticesCount = 0;
		spine::Vector<float>* uvs = nullptr;
		spine::Vector<unsigned short>* indices = nullptr;
		size_t indicesCount = 0;
		spine::Color* attachmentColor;

		if (attachment->getRTTI().isExactly(spine::RegionAttachment::rtti)) {
			spine::RegionAttachment* regionAttachment = (spine::RegionAttachment*)attachment;
			attachmentColor = &regionAttachment->getColor();

			// Early out if the slot color is 0
			if (attachmentColor->a == 0) {
				m_clipper.clipEnd(slot);
				continue;
			}

			worldVertices.setSize(8, 0);
			regionAttachment->computeWorldVertices(slot.getBone(), worldVertices, 0, 2);
			verticesCount = 4;
			uvs = &regionAttachment->getUVs();
			indices = &quadIndices;
			indicesCount = 6;
			texture = (sf::Texture*)((spine::AtlasRegion*)regionAttachment->getRendererObject())->page->getRendererObject();
		}
		else if (attachment->getRTTI().isExactly(spine::MeshAttachment::rtti)) {
			spine::MeshAttachment* mesh = (spine::MeshAttachment*)attachment;
			attachmentColor = &mesh->getColor();

			// Early out if the slot color is 0
			if (attachmentColor->a == 0) {
				m_clipper.clipEnd(slot);
				continue;
			}

			worldVertices.setSize(mesh->getWorldVerticesLength(), 0);
			texture = (sf::Texture*)((spine::AtlasRegion*)mesh->getRendererObject())->page->getRendererObject();
			mesh->computeWorldVertices(slot, 0, mesh->getWorldVerticesLength(), worldVertices, 0, 2);
			verticesCount = mesh->getWorldVerticesLength() >> 1;
			uvs = &mesh->getUVs();
			indices = &mesh->getTriangles();
			indicesCount = mesh->getTriangles().size();
		}
		else if (attachment->getRTTI().isExactly(spine::ClippingAttachment::rtti)) {
			spine::ClippingAttachment* clip = (spine::ClippingAttachment*)slot.getAttachment();
			m_clipper.clipStart(slot, clip);
			continue;
		}
		else continue;

		spine::Color light;
		light.r = m_skeleton->getColor().r * slot.getColor().r * attachmentColor->r * m_color.r;
		light.g = m_skeleton->getColor().g * slot.getColor().g * attachmentColor->g * m_color.g;
		light.b = m_skeleton->getColor().b * slot.getColor().b * attachmentColor->b * m_color.b;
		light.a = m_skeleton->getColor().a * slot.getColor().a * attachmentColor->a * m_color.a;

		spine::BlendMode blend = slot.getData().getBlendMode();

		if (previousTexture == nullptr) {
			previousTexture = texture;
			previousBlend = blend;
		}

		if (previousBlend != blend || previousTexture != texture) {
			DrawVertices(shader, previousTexture);
			previousTexture = texture;
			previousBlend = blend;
		}

		if (m_clipper.isClipping()) {
			m_clipper.clipTriangles(worldVertices, *indices, *uvs, 2);
			vertices = &m_clipper.getClippedVertices();
			verticesCount = m_clipper.getClippedVertices().size() >> 1;
			uvs = &m_clipper.getClippedUVs();
			indices = &m_clipper.getClippedTriangles();
			indicesCount = m_clipper.getClippedTriangles().size();
		}

		for (size_t ii = 0; ii < indicesCount; ++ii) {
			unsigned int index = (*indices)[ii] << 1;
			vertexArray.emplace_back(
				glm::vec2((*vertices)[index], (*vertices)[index + 1]),
				glm::vec2((*uvs)[index], (*uvs)[index + 1]),
				glm::vec4(light.r * light.a, light.g * light.a, light.b * light.a, light.a)
			);
		}
		m_clipper.clipEnd(slot);
	}
	m_clipper.clipEnd();
	DrawVertices(shader, texture);
	return;
}

void SpineAnimation::SetOutline(bool enabled) {
	m_outline = enabled;
}

void SpineAnimation::SetColor(float r, float g, float b, float a) {
	m_color = { r, g, b, a };
}

spine::TrackEntry* SpineAnimation::setAnimation(size_t trackIndex, const std::string& animationName, bool loop) {
	return m_animationState->setAnimation(trackIndex, animationName.c_str(), loop);
}

spine::TrackEntry* SpineAnimation::setAnimation(size_t trackIndex, spine::Animation* animation, bool loop) {
	return m_animationState->setAnimation(trackIndex, animation, loop);
}

spine::TrackEntry* SpineAnimation::setEmptyAnimation(size_t trackIndex, float mixDuration) {
	return m_animationState->setEmptyAnimation(trackIndex, mixDuration);
}

void SpineAnimation::setEmptyAnimations(float mixDuration) {
	return m_animationState->setEmptyAnimations(mixDuration);
}

spine::TrackEntry* SpineAnimation::addAnimation(size_t trackIndex, const std::string& animationName, bool loop, float delay) {
	return m_animationState->addAnimation(trackIndex, animationName.c_str(), loop, delay);
}

spine::TrackEntry* SpineAnimation::addAnimation(size_t trackIndex, spine::Animation* animation, bool loop, float delay) {
	return m_animationState->addAnimation(trackIndex, animation, loop, delay);
}

spine::TrackEntry* SpineAnimation::addEmptyAnimation(size_t trackIndex, bool loop, float delay) {
	return m_animationState->addEmptyAnimation(trackIndex, loop, delay);
}

spine::Animation* SpineAnimation::findAnimation(const std::string& animationName) const {
	return m_pose.skeletonData->findAnimation(animationName.c_str());
}

glm::vec2 SpineAnimation::getBonePosition(const std::string& boneName) const {
	spine::Bone* bone = m_skeleton->findBone(boneName.c_str());
	return glm::vec2(bone->getWorldX() / spine_to3d_scale_i * m_scale.x, bone->getWorldY() / spine_to3d_scale_i * m_scale.y);
}

glm::vec2 SpineAnimation::getBonePositionByIndex(int boneIndex) const {
	spine::Bone* bone = m_bonesRef[0][boneIndex];
	return glm::vec2(bone->getWorldX() / spine_to3d_scale_i * m_scale.x, bone->getWorldY() / spine_to3d_scale_i * m_scale.y);
}

int SpineAnimation::getBoneIndex(const std::string& boneName) const {
	return m_skeleton->findBoneIndex(boneName.c_str());
}

void SpineAnimation::setListener(spine::AnimationStateListenerObject* listener) {
	return m_animationState->setListener(listener);
}

void SpineAnimation::UpdateShader(ME::Shader* shader, ME::Camera* camera) {
	if (m_positionChanged || m_rotationChanged || m_scaleChanged) {
		ComputeMatrix();
		m_positionChanged = false;
		m_rotationChanged = false;
		m_scaleChanged = false;
	}
	shader->UpdateUniform(Game::ActorShaderUniformId::Mat4_M, &m_matM[0][0]);
	return;
}

void SpineAnimation::DrawVertices(ME::Shader* shader, sf::Texture* texture) {
	GLsizei drawCount = (GLsizei)vertexArray.size();
	GLsizei stride = sizeof(vertexArray[0]);
	size_t texCoordOffset = sizeof(vertexArray[0].position);
	size_t colorOffset = texCoordOffset + sizeof(vertexArray[0].texCoord);

	glCheck(glBindVertexArray(m_vao));

	glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO));

	glCheck(glBufferData(GL_ARRAY_BUFFER, drawCount * stride, vertexArray.data(), GL_STREAM_DRAW));
	glCheck(glEnableVertexAttribArray(static_cast<GLuint>(Game::ActorVertexAttribute::Position)));
	glCheck(glVertexAttribPointer(static_cast<GLuint>(Game::ActorVertexAttribute::Position), 2, GL_FLOAT, GL_FALSE, stride, 0));
	glCheck(glEnableVertexAttribArray(static_cast<GLuint>(Game::ActorVertexAttribute::TexCoord)));
	glCheck(glVertexAttribPointer(static_cast<GLuint>(Game::ActorVertexAttribute::TexCoord), 2, GL_FLOAT, GL_FALSE, stride, (void*)texCoordOffset));
	glCheck(glEnableVertexAttribArray(static_cast<GLuint>(Game::ActorVertexAttribute::Color)));
	glCheck(glVertexAttribPointer(static_cast<GLuint>(Game::ActorVertexAttribute::Color), 4, GL_FLOAT, GL_FALSE, stride, (void*)colorOffset));

	sf::Texture::bind(texture);

	if (m_outline) {
		float sx = 1.0f / m_scale.x;
		float sy = 1.0f / m_scale.y;
		shader->UpdateUniform4(Game::ActorShaderUniformId::Vec4_CvrClr, 1.0f, 1.0f, 0.0f, 1.0f);
		shader->UpdateUniformI1(Game::ActorShaderUniformId::Int1_CvrClr, 1);
		for (unsigned char i = 0; i < 8; ++i) {
			shader->UpdateUniform2(
				Game::ActorShaderUniformId::Vec2_Offset,
				::CircleOffsetX[i] * sx,
				::CircleOffsetY[i] * sy
			);
			glCheck(glDrawArrays(GL_TRIANGLES, 0, drawCount));
		}
		shader->UpdateUniformI1(Game::ActorShaderUniformId::Int1_CvrClr, 0);
		shader->UpdateUniform2(Game::ActorShaderUniformId::Vec2_Offset, 0.0f, 0.0f);
		shader->UpdateUniform4(Game::ActorShaderUniformId::Vec4_CvrClr, 1.0f, 1.0f, 1.0f, 1.0f);
	}

	glCheck(glDrawArrays(GL_TRIANGLES, 0, drawCount));

	glCheck(glBindVertexArray(0));
	vertexArray.clear();
}

// end class SpineAnimation

// class SpinePose
SpinePose::SpinePose(Model::SpinePoseData _pose) :
	m_pose(_pose) {}

SpinePose::~SpinePose() {
	delete m_pose.animationStateData;
	delete m_pose.skeletonData;
	delete m_pose.atlas;
	return;
}

std::shared_ptr<ME::IModel> SpinePose::CreateAnimation() {
	return std::make_shared<SpineAnimation>(m_pose);
}
// end class SpinePose

// class SpineFactory
SpineFactory::SpineFactory() {
	quadIndices.clear();
	quadIndices.add(0);
	quadIndices.add(1);
	quadIndices.add(2);
	quadIndices.add(2);
	quadIndices.add(3);
	quadIndices.add(0);
	return;
}

SpineFactory::~SpineFactory() {
	return;
}

bool SpineFactory::CreatePose(std::unique_ptr<IAnimationPose>& ptr, std::string_view name, unsigned char type) {
	Model::IAnimationPose* res = nullptr;
	std::string path("res/chararts/");
	switch (type) {
	case 0: path += "bat0/"; break;
	case 1: path += "bat1/"; break;
	case 2: path += "build/build_"; break;
	default: return false;
	}
	path += name;
	res = createPoseBinary(path + ".skel", path + ".atlas");
	if (res != nullptr)
		ptr = std::unique_ptr<IAnimationPose>(res);
	return res != nullptr;
}

char SpineFactory::CreatePose2(
	std::unique_ptr<IAnimationPose>& ptr0,
	std::unique_ptr<IAnimationPose>& ptr1,
	std::string_view name
) {
	char res = 0;
	std::string path0("res/chararts/bat0/");
	std::string path1("res/chararts/bat1/");
	path0 += name;
	path1 += name;
	Model::IAnimationPose* res0 = createPoseBinary(path0 + ".skel", path0 + ".atlas");
	Model::IAnimationPose* res1 = createPoseBinary(path1 + ".skel", path1 + ".atlas");
	if (res0 != nullptr) {
		ptr0 = std::unique_ptr<IAnimationPose>(res0);
		res |= 0x01;
	}
	if (res1 != nullptr) {
		ptr1 = std::unique_ptr<IAnimationPose>(res1);
		res |= 0x02;
	}
	return res;
}

bool SpineFactory::CreateEnemyPose(std::unique_ptr<IAnimationPose>& ptr, std::string_view name) {
	Model::IAnimationPose* res = nullptr;
	std::string path("res/battle/enemy/");
	path += name;
	res = createPoseBinary(path + ".skel", path + ".atlas");
	if (res != nullptr)
		ptr = std::unique_ptr<IAnimationPose>(res);
	return res != nullptr;
}

Model::SpinePose* SpineFactory::createPoseBinary(
	std::string_view binaryPath,
	std::string_view atlasPath
) {
	Model::SpinePoseData pose{};

	// Load the texture atlas
	pose.atlas = new spine::Atlas(atlasPath.data(), &::SFMLTextureLoader::instance);
	if (pose.atlas->getPages().size() == 0) {
		//printf("Failed to load atlas");
		return nullptr;
	}

	// Load the skeleton data
	spine::SkeletonBinary binary(pose.atlas);
	pose.skeletonData = binary.readSkeletonDataFile(binaryPath.data());

	if (!pose.skeletonData) {
		//printf("Failed to load skeleton data");
		delete pose.atlas;
		return nullptr;
	}
	// Setup
	pose.animationStateData = new spine::AnimationStateData(pose.skeletonData);

	return new Model::SpinePose(pose);
}

// end class SpineFactory
} // end namespace Model

namespace {

std::unique_ptr<Model::SpineFactory> g_spineFactoryInstance;

}

namespace Model {

Model::IAnimationFactory* IAnimationFactory::Instance() {
	if (g_spineFactoryInstance == nullptr) {
		g_spineFactoryInstance = std::make_unique<Model::SpineFactory>();
	}
	return g_spineFactoryInstance.get();
}

void IAnimationFactory::Drop() {
	g_spineFactoryInstance.reset();
}

}

#endif // ARCHKNIGHTS_LIMITED
