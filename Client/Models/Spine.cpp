
#define SPINE_SHORT_NAMES

#ifndef SPINE_MESH_VERTEX_COUNT_MAX
#define SPINE_MESH_VERTEX_COUNT_MAX 1000
#endif

#include <GL/glew.h>

#include "Spine.h"
#include <MysteryEngine/G3D/Shader.h>
#include <MysteryEngine/G3D/Camera.h>
#include <MysteryEngine/G3D/GlCheck.h>


namespace {

constexpr float spine_to3d_scale_i = 128.0f;
constexpr float spine_global_scale = 0.7125f;
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

/// <summary>
/// Convert an sf::BlendMode::Factor constant to the corresponding OpenGL constant.
/// </summary>
/// <param name="blendFactor"></param>
/// <returns></returns>
/*sf::Uint32 factorToGlConstant(sf::BlendMode::Factor blendFactor) {
	switch (blendFactor) {
	case sf::BlendMode::Zero:             return GL_ZERO;
	case sf::BlendMode::One:              return GL_ONE;
	case sf::BlendMode::SrcColor:         return GL_SRC_COLOR;
	case sf::BlendMode::OneMinusSrcColor: return GL_ONE_MINUS_SRC_COLOR;
	case sf::BlendMode::DstColor:         return GL_DST_COLOR;
	case sf::BlendMode::OneMinusDstColor: return GL_ONE_MINUS_DST_COLOR;
	case sf::BlendMode::SrcAlpha:         return GL_SRC_ALPHA;
	case sf::BlendMode::OneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
	case sf::BlendMode::DstAlpha:         return GL_DST_ALPHA;
	case sf::BlendMode::OneMinusDstAlpha: return GL_ONE_MINUS_DST_ALPHA;
	}
	sf::err() << "Invalid value for sf::BlendMode::Factor! Fallback to sf::BlendMode::Zero." << std::endl;
	assert(false);
	return GL_ZERO;
}

static const sf::BlendMode normal = sf::BlendMode(sf::BlendMode::SrcAlpha, sf::BlendMode::OneMinusSrcAlpha);
static const sf::BlendMode additive = sf::BlendMode(sf::BlendMode::SrcAlpha, sf::BlendMode::One);
static const sf::BlendMode multiply = sf::BlendMode(sf::BlendMode::DstColor, sf::BlendMode::OneMinusSrcAlpha);
static const sf::BlendMode screen = sf::BlendMode(sf::BlendMode::One, sf::BlendMode::OneMinusSrcColor);

static const sf::BlendMode normalPma = sf::BlendMode(sf::BlendMode::One, sf::BlendMode::OneMinusSrcAlpha);
static const sf::BlendMode additivePma = sf::BlendMode(sf::BlendMode::One, sf::BlendMode::One);
static const sf::BlendMode multiplyPma = sf::BlendMode(sf::BlendMode::DstColor, sf::BlendMode::OneMinusSrcAlpha);
static const sf::BlendMode screenPma = sf::BlendMode(sf::BlendMode::One, sf::BlendMode::OneMinusSrcColor);
*/

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

namespace ohms {

// class SpineAnimation
SpineAnimation::SpineAnimation(const ohms::SpinePoseData _pose) :
	m_pose(_pose),
	m_outline(false)

{
	// Create the skeleton
	m_skeleton = new spine::Skeleton(m_pose.skeletonData);
	// Create the animation state
	m_animationState = new spine::AnimationState(m_pose.animationStateData);

	worldVertices.clear();
	worldVertices.ensureCapacity(SPINE_MESH_VERTEX_COUNT_MAX);
	m_skeleton->setScaleX(spine_global_scale);
	m_skeleton->setScaleY(spine_global_scale);

	m_bonesRef = &m_skeleton->getBones();

	glCheck(glGenVertexArrays(1, &m_vao));
	glCheck(glBindVertexArray(m_vao));
	glCheck(glGenBuffers(1, &m_vertexVBO));
	glCheck(glBindVertexArray(0));
	return;
}

SpineAnimation::~SpineAnimation() {
	delete m_animationState;
	delete m_skeleton;
	return;
}

void SpineAnimation::Update(float dt) {
	m_skeleton->update(dt);
	m_animationState->update(dt);
	m_animationState->apply(*m_skeleton);
	m_skeleton->updateWorldTransform();
	return;
}

void SpineAnimation::Draw(ME::Camera& camera, ME::Shader& shader) {
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
		light.r = m_skeleton->getColor().r * slot.getColor().r * attachmentColor->r;
		light.g = m_skeleton->getColor().g * slot.getColor().g * attachmentColor->g;
		light.b = m_skeleton->getColor().b * slot.getColor().b * attachmentColor->b;
		light.a = m_skeleton->getColor().a * slot.getColor().a * attachmentColor->a;

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

sf::Vector2f SpineAnimation::getBonePosition(const std::string& boneName) const {
	spine::Bone* bone = m_skeleton->findBone(boneName.c_str());
	return sf::Vector2f(bone->getWorldX() / (-spine_to3d_scale_i) * spine_global_scale, bone->getWorldY() / spine_to3d_scale_i * spine_global_scale);
}

sf::Vector2f SpineAnimation::getBonePositionByIndex(int boneIndex) const {
	spine::Bone* bone = m_bonesRef[0][boneIndex];
	return sf::Vector2f(bone->getWorldX() / (-spine_to3d_scale_i) * spine_global_scale, bone->getWorldY() / spine_to3d_scale_i * spine_global_scale);
}

int SpineAnimation::getBoneIndex(const std::string& boneName) const {
	return m_skeleton->findBoneIndex(boneName.c_str());
}

void SpineAnimation::UpdateShader(ME::Shader& shader, ME::Camera& camera) {
	ComputeMatrix();

	glm::mat4 viewProj = camera.getMatPV() * m_matM;

	glm::vec3 camP = camera.getPos();
	glm::vec3 campos = { camP.x - m_position.x, camP.y - m_position.y, camP.z - m_position.z };

	shader.UpdateUniform(Game::ActorShaderUniformId::Mat4_PVM, &viewProj[0][0]);
	shader.UpdateUniform(Game::ActorShaderUniformId::Mat4_M, &m_matM[0][0]);
	shader.UpdateUniform(Game::ActorShaderUniformId::Vec3_CamPos, &campos[0]);

	return;
}

void SpineAnimation::DrawVertices(ME::Shader& shader, sf::Texture* texture) {
	GLsizei drawCount = (GLsizei)vertexArray.size();
	GLsizei stride = sizeof(vertexArray[0]);
	size_t texCoordOffset = sizeof(vertexArray[0].position);
	size_t colorOffset = texCoordOffset + sizeof(vertexArray[0].texCoord);

	glCheck(glBindVertexArray(m_vao));

	glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO));

	glCheck(glBufferData(GL_ARRAY_BUFFER, drawCount * stride, vertexArray.data(), GL_DYNAMIC_DRAW));
	glCheck(glEnableVertexAttribArray(static_cast<GLuint>(Game::ActorVertexAttribute::Position)));
	glCheck(glVertexAttribPointer(static_cast<GLuint>(Game::ActorVertexAttribute::Position), 2, GL_FLOAT, GL_FALSE, stride, 0));
	glCheck(glEnableVertexAttribArray(static_cast<GLuint>(Game::ActorVertexAttribute::TexCoord)));
	glCheck(glVertexAttribPointer(static_cast<GLuint>(Game::ActorVertexAttribute::TexCoord), 2, GL_FLOAT, GL_FALSE, stride, (void*)texCoordOffset));
	glCheck(glEnableVertexAttribArray(static_cast<GLuint>(Game::ActorVertexAttribute::Color)));
	glCheck(glVertexAttribPointer(static_cast<GLuint>(Game::ActorVertexAttribute::Color), 4, GL_FLOAT, GL_FALSE, stride, (void*)colorOffset));

	sf::Texture::bind(texture);

	if (m_outline) {
		shader.UpdateUniform4(Game::ActorShaderUniformId::Vec4_CvrClr, 1.0f, 1.0f, 0.0f, 1.0f);
		shader.UpdateUniformI1(Game::ActorShaderUniformId::Int1_CvrClr, 1);
		for (unsigned char i = 0; i < 8; ++i) {
			shader.UpdateUniform2(
				Game::ActorShaderUniformId::Vec2_Offset,
				::CircleOffsetX[i],
				::CircleOffsetY[i]
			);
			glCheck(glDrawArrays(GL_TRIANGLES, 0, drawCount));
		}
		shader.UpdateUniformI1(Game::ActorShaderUniformId::Int1_CvrClr, 0);
		shader.UpdateUniform2(Game::ActorShaderUniformId::Vec2_Offset, 0.0f, 0.0f);
		shader.UpdateUniform4(Game::ActorShaderUniformId::Vec4_CvrClr, 1.0f, 1.0f, 1.0f, 1.0f);
	}

	glCheck(glDrawArrays(GL_TRIANGLES, 0, drawCount));

	glCheck(glBindVertexArray(0));
	vertexArray.clear();
}

// end class SpineAnimation

// class SpinePose
SpinePose::SpinePose(ohms::SpinePoseData _pose) :
	m_pose(_pose) {}

SpinePose::~SpinePose() {
	delete m_pose.animationStateData;
	delete m_pose.skeletonData;
	delete m_pose.atlas;
	return;
}

std::shared_ptr<ISpineAnimation> SpinePose::CreateAnimation() {
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

bool SpineFactory::CreatePose(std::unique_ptr<ISpinePose>& ptr, const std::string& name, unsigned char type) {
	ohms::ISpinePose* res = nullptr;
	switch (type) {
	case 0:
		res = createPoseBinary(
			std::string("res/characters/").append(name).append("/animation/batf/skel").c_str(),
			std::string("res/characters/").append(name).append("/animation/batf/atlas").c_str()
		);
		break;
	case 1:
		res = createPoseBinary(
			std::string("res/characters/").append(name).append("/animation/batb/skel").c_str(),
			std::string("res/characters/").append(name).append("/animation/batb/atlas").c_str()
		);
		break;
	case 2:
		res = createPoseBinary(
			std::string("res/characters/").append(name).append("/animation/building/skel").c_str(),
			std::string("res/characters/").append(name).append("/animation/building/atlas").c_str()
		);
		break;
	}
	if (res != nullptr)
		ptr = std::unique_ptr<ISpinePose>(res);
	return res != nullptr;
}

ohms::SpinePose* SpineFactory::createPoseBinary(const std::string& binaryPath, const std::string& atlasPath) {
	ohms::SpinePoseData pose{};

	// Load the texture atlas
	pose.atlas = new spine::Atlas(atlasPath.c_str(), &::SFMLTextureLoader::instance);
	if (pose.atlas->getPages().size() == 0) {
		//printf("Failed to load atlas");
		return nullptr;
	}

	// Load the skeleton data
	spine::SkeletonBinary binary(pose.atlas);
	pose.skeletonData = binary.readSkeletonDataFile(binaryPath.c_str());

	if (!pose.skeletonData) {
		//printf("Failed to load skeleton data");
		delete pose.atlas;
		return nullptr;
	}
	// Setup
	pose.animationStateData = new spine::AnimationStateData(pose.skeletonData);

	return new ohms::SpinePose(pose);
}

// end class SpineFactory
} // end namespace ohms

std::unique_ptr<ohms::ISpineFactory> ohms::ISpineFactory::Create() {
	return std::make_unique<ohms::SpineFactory>();
}
