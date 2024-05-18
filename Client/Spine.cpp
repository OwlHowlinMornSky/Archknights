
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

/// <summary>
/// 矩形的 index
/// </summary>
spine::Vector<unsigned short> quadIndices;

/// <summary>
/// Convert an sf::BlendMode::Factor constant to the corresponding OpenGL constant.
/// </summary>
/// <param name="blendFactor"></param>
/// <returns></returns>
sf::Uint32 factorToGlConstant(sf::BlendMode::Factor blendFactor) {
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

} // end namespace

namespace ohms {

SpinePose::SpinePose() :
	atlas(nullptr),
	skeletonData(nullptr),
	animationStateData(nullptr) {}

SpinePose::~SpinePose() {
	delete animationStateData;
	delete skeletonData;
	delete atlas;
}

// class SpineEntity
SpineEntity::SpineEntity(const ohms::SpinePose* pose) :
	poseRef(pose),

	outline(false),

	skeleton(nullptr),
	animationState(nullptr),

	drawCount(0) {

	// Create the skeleton
	this->skeleton = new spine::Skeleton(pose->skeletonData);
	// Create the animation state
	this->animationState = new spine::AnimationState(pose->animationStateData);

	this->worldVertices.clear();
	this->worldVertices.ensureCapacity(SPINE_MESH_VERTEX_COUNT_MAX);
	this->skeleton->setScaleX(spine_global_scale);
	this->skeleton->setScaleY(-spine_global_scale);

	this->bonesRef = &this->skeleton->getBones();

	glCheck(glGenVertexArrays(1, &this->vao));
	glCheck(glBindVertexArray(this->vao));
	glCheck(glGenBuffers(1, &this->vertexVBO));
	glCheck(glBindVertexArray(0));
	return;
}

SpineEntity::~SpineEntity() {
	delete this->skeleton;
	delete this->animationState;
	return;
}

void SpineEntity::Update(float dt) {
	this->skeleton->update(dt);
	this->animationState->update(dt);
	this->animationState->apply(*this->skeleton);
	this->skeleton->updateWorldTransform();
	return;
}

void SpineEntity::UpdateShader(ME::Shader* shader, ME::Camera* camera) {
	ComputeMatrix();

	glm::mat4 viewProj = camera->getMatPV() * this->m_matM;

	//glm::fvec3 campos = { camera->Position.x - this->Position.x, camera->Position.y - this->Position.y, camera->Position.z - this->Position.z };
	shader->updateUniformMat4fvName("uMatPVM", &viewProj[0][0]);
	//shader->UpdateUniform(UniformType::TransformV, &camera->ViewMatrix()[0][0]);
	//shader->UpdateUniform(UniformType::TransformM, &this->transform[0][0]);
	//shader->UpdateUniform(UniformType::CameraPosition, &campos);
	//this->shaderRef = static_cast<ohms::shader::Spine*>(shader);
	return;
}
void SpineEntity::Draw(ME::Camera& camera, ME::Shader& shader) {
	vertexArray.clear();
	sf::Texture* previousTexture = nullptr;

	// Early out if skeleton is invisible
	if (this->skeleton->getColor().a == 0.0f)
		return;

	UpdateShader(&shader, &camera);

	spine::BlendMode previousBlend;
	ME::Vertex vertex;
	sf::Texture* texture = nullptr;
	for (size_t i = 0, n = this->skeleton->getSlots().size(); i < n; ++i) {
		spine::Slot& slot = *this->skeleton->getDrawOrder()[i];
		spine::Attachment* attachment = slot.getAttachment();
		if (!attachment) continue;

		// Early out if the slot color is 0 or the bone is not active
		if (slot.getColor().a == 0 || !slot.getBone().isActive()) {
			this->clipper.clipEnd(slot);
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
				clipper.clipEnd(slot);
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
				clipper.clipEnd(slot);
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
			clipper.clipStart(slot, clip);
			continue;
		}
		else continue;

		spine::Color light;
		light.r = this->skeleton->getColor().r * slot.getColor().r * attachmentColor->r;
		light.g = this->skeleton->getColor().g * slot.getColor().g * attachmentColor->g;
		light.b = this->skeleton->getColor().b * slot.getColor().b * attachmentColor->b;
		light.a = this->skeleton->getColor().a * slot.getColor().a * attachmentColor->a;

		spine::BlendMode blend = slot.getData().getBlendMode();

		if (previousTexture == nullptr) {
			previousTexture = texture;
			previousBlend = blend;
		}

		if (previousBlend != blend || previousTexture != texture) {

			drawCount = (GLsizei)vertexArray.size();
			GLsizei stride = sizeof(vertexArray[0]);
			size_t texCoordOffset = sizeof(vertexArray[0].position);
			size_t colorOffset = texCoordOffset + sizeof(vertexArray[0].texCoord);
			size_t normalOffset = colorOffset + sizeof(vertexArray[0].color);


			glCheck(glBindVertexArray(vao));

			glCheck(glBindBuffer(GL_ARRAY_BUFFER, vertexVBO));

			glCheck(glBufferData(GL_ARRAY_BUFFER, drawCount * stride, vertexArray.data(), GL_DYNAMIC_DRAW));
			glCheck(glEnableVertexAttribArray(static_cast<GLuint>(ME::VertexAttribute::Position)));
			glCheck(glVertexAttribPointer(static_cast<GLuint>(ME::VertexAttribute::Position), 3, GL_FLOAT, GL_FALSE, stride, 0));
			glCheck(glEnableVertexAttribArray(static_cast<GLuint>(ME::VertexAttribute::TexCoord)));
			glCheck(glVertexAttribPointer(static_cast<GLuint>(ME::VertexAttribute::TexCoord), 2, GL_FLOAT, GL_FALSE, stride, (void*)texCoordOffset));
			glCheck(glEnableVertexAttribArray(static_cast<GLuint>(ME::VertexAttribute::Color)));
			glCheck(glVertexAttribPointer(static_cast<GLuint>(ME::VertexAttribute::Color), 4, GL_FLOAT, GL_FALSE, stride, (void*)colorOffset));

			sf::Texture::bind(texture);

			glCheck(glDrawArrays(GL_TRIANGLES, 0, drawCount));

			glCheck(glBindVertexArray(0));

			vertexArray.clear();
			previousTexture = texture;
			previousBlend = blend;
		}

		if (clipper.isClipping()) {
			clipper.clipTriangles(worldVertices, *indices, *uvs, 2);
			vertices = &clipper.getClippedVertices();
			verticesCount = clipper.getClippedVertices().size() >> 1;
			uvs = &clipper.getClippedUVs();
			indices = &clipper.getClippedTriangles();
			indicesCount = clipper.getClippedTriangles().size();
		}

		for (size_t ii = 0; ii < indicesCount; ++ii) {
			unsigned int index = (*indices)[ii] << 1;
			vertexArray.emplace_back(
				glm::vec3((*vertices)[index] / spine_to3d_scale_i, (*vertices)[index + 1] / spine_to3d_scale_i, 0.0f),
				glm::vec2((*uvs)[index], (*uvs)[index + 1]),
				glm::vec4(light.r * light.a, light.g * light.a, light.b * light.a, light.a)
			);
		}
		clipper.clipEnd(slot);
	}
	clipper.clipEnd();

	drawCount = (GLsizei)vertexArray.size();
	GLsizei stride = sizeof(vertexArray[0]);
	size_t texCoordOffset = sizeof(vertexArray[0].position);
	size_t colorOffset = texCoordOffset + sizeof(vertexArray[0].texCoord);
	size_t normalOffset = colorOffset + sizeof(vertexArray[0].color);


	glCheck(glBindVertexArray(vao));

	glCheck(glBindBuffer(GL_ARRAY_BUFFER, vertexVBO));

	glCheck(glBufferData(GL_ARRAY_BUFFER, drawCount * stride, vertexArray.data(), GL_DYNAMIC_DRAW));
	glCheck(glEnableVertexAttribArray(static_cast<GLuint>(ME::VertexAttribute::Position)));
	glCheck(glVertexAttribPointer(static_cast<GLuint>(ME::VertexAttribute::Position), 3, GL_FLOAT, GL_FALSE, stride, 0));
	glCheck(glEnableVertexAttribArray(static_cast<GLuint>(ME::VertexAttribute::TexCoord)));
	glCheck(glVertexAttribPointer(static_cast<GLuint>(ME::VertexAttribute::TexCoord), 2, GL_FLOAT, GL_FALSE, stride, (void*)texCoordOffset));
	glCheck(glEnableVertexAttribArray(static_cast<GLuint>(ME::VertexAttribute::Color)));
	glCheck(glVertexAttribPointer(static_cast<GLuint>(ME::VertexAttribute::Color), 4, GL_FLOAT, GL_FALSE, stride, (void*)colorOffset));

	sf::Texture::bind(texture);

	/*if (this->outline) {
		this->shaderRef->outline = true;
		for (unsigned char i = 0; i < 8; ++i) {
			this->shaderRef->offsetX = outline_thickness * cosf(0.7853981633974483f * i);
			this->shaderRef->offsetY = outline_thickness * sinf(0.7853981633974483f * i);
			this->shaderRef->Update();
			glCheck(glDrawArrays(GL_TRIANGLES, 0, drawCount));
		}
		this->shaderRef->outline = false;
		this->shaderRef->Update();
	}*/

	glCheck(glDrawArrays(GL_TRIANGLES, 0, drawCount));

	glCheck(glBindVertexArray(0));
	return;
}
/*
void SpineEntity::DrawH(ME::Camera& camera, ME::Shader& shader) {
	// Early out if skeleton is invisible
	if (this->skeleton->getColor().a == 0.0f)
		return;

	vertexArray.clear();
	sf::Texture* texture = nullptr;
	spine::Slot* slot = nullptr;
	spine::Attachment* attachment = nullptr;

	for (size_t i = 0, n = this->skeleton->getSlots().size(); i < n; ++i) {
		slot = this->skeleton->getDrawOrder()[i];
		attachment = slot->getAttachment();
		if (!attachment) continue;
		// Early out if the slot color is 0 or the bone is not active
		if (slot->getColor().a == 0 || !slot->getBone().isActive()) {
			this->clipper.clipEnd(*slot);
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
				clipper.clipEnd(*slot);
				continue;
			}
			worldVertices.setSize(8, 0);
			regionAttachment->computeWorldVertices(slot->getBone(), worldVertices, 0, 2);
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
				clipper.clipEnd(*slot);
				continue;
			}
			worldVertices.setSize(mesh->getWorldVerticesLength(), 0);
			texture = (sf::Texture*)((spine::AtlasRegion*)mesh->getRendererObject())->page->getRendererObject();
			mesh->computeWorldVertices(*slot, 0, mesh->getWorldVerticesLength(), worldVertices, 0, 2);
			verticesCount = mesh->getWorldVerticesLength() >> 1;
			uvs = &mesh->getUVs();
			indices = &mesh->getTriangles();
			indicesCount = mesh->getTriangles().size();
		}
		else if (attachment->getRTTI().isExactly(spine::ClippingAttachment::rtti)) {
			spine::ClippingAttachment* clip = (spine::ClippingAttachment*)slot->getAttachment();
			clipper.clipStart(*slot, clip);
			continue;
		}
		else continue;

		spine::Color light;
		light.r = this->skeleton->getColor().r * slot->getColor().r * attachmentColor->r;
		light.g = this->skeleton->getColor().g * slot->getColor().g * attachmentColor->g;
		light.b = this->skeleton->getColor().b * slot->getColor().b * attachmentColor->b;
		light.a = this->skeleton->getColor().a * slot->getColor().a * attachmentColor->a;

		if (clipper.isClipping()) {
			clipper.clipTriangles(worldVertices, *indices, *uvs, 2);
			vertices = &clipper.getClippedVertices();
			verticesCount = clipper.getClippedVertices().size() >> 1;
			uvs = &clipper.getClippedUVs();
			indices = &clipper.getClippedTriangles();
			indicesCount = clipper.getClippedTriangles().size();
		}

		for (size_t ii = 0; ii < indicesCount; ++ii) {
			unsigned int index = (*indices)[ii] << 1;
			vertexArray.emplace_back(
				glm::vec3((*vertices)[index] / spine_to3d_scale_i, (*vertices)[index + 1] / spine_to3d_scale_i, 0.0f),
				glm::vec2((*uvs)[index], (*uvs)[index + 1]),
				glm::vec4(light.r * light.a, light.g * light.a, light.b * light.a, light.a)
			);
		}
		clipper.clipEnd(*slot);
	}
	clipper.clipEnd();

	size_t tempSize = vertexArray.size();
	if (tempSize > (size_t)INT_MAX) {
		//throw std::exception("Vertex Array Too Huge!");
		return;
	}
	drawCount = (GLsizei)tempSize;

	GLsizei stride = sizeof(vertexArray[0]);
	size_t texCoordOffset = sizeof(vertexArray[0].position);
	size_t colorOffset = texCoordOffset + sizeof(vertexArray[0].texCoord);
	size_t normalOffset = colorOffset + sizeof(vertexArray[0].color);

	UpdateShader(&shader, &camera);

	glCheck(glBindVertexArray(vao));

	glCheck(glBindBuffer(GL_ARRAY_BUFFER, vertexVBO));

	glCheck(glBufferData(GL_ARRAY_BUFFER, drawCount * stride, vertexArray.data(), GL_DYNAMIC_DRAW));
	glCheck(glEnableVertexAttribArray(static_cast<GLuint>(ME::VertexAttribute::Position)));
	glCheck(glVertexAttribPointer(static_cast<GLuint>(ME::VertexAttribute::Position), 3, GL_FLOAT, GL_FALSE, stride, 0));
	glCheck(glEnableVertexAttribArray(static_cast<GLuint>(ME::VertexAttribute::TexCoord)));
	glCheck(glVertexAttribPointer(static_cast<GLuint>(ME::VertexAttribute::TexCoord), 2, GL_FLOAT, GL_FALSE, stride, (void*)texCoordOffset));
	glCheck(glEnableVertexAttribArray(static_cast<GLuint>(ME::VertexAttribute::Color)));
	glCheck(glVertexAttribPointer(static_cast<GLuint>(ME::VertexAttribute::Color), 4, GL_FLOAT, GL_FALSE, stride, (void*)colorOffset));

	sf::Texture::bind(texture);

	/*if (this->outline) {
		this->shaderRef->outline = true;
		for (unsigned char i = 0; i < 8; ++i) {
			this->shaderRef->offsetX = outline_thickness * cosf(0.7853981633974483f * i);
			this->shaderRef->offsetY = outline_thickness * sinf(0.7853981633974483f * i);
			this->shaderRef->Update();
			glCheck(glDrawArrays(GL_TRIANGLES, 0, drawCount));
		}
		this->shaderRef->outline = false;
		this->shaderRef->Update();
	}/

	glCheck(glDrawArrays(GL_TRIANGLES, 0, drawCount));

	glCheck(glBindVertexArray(0));
	return;
}*/

void SpineEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform.translate(200.0f, 200.0f);

	sf::VertexArray* vertexArray = new sf::VertexArray();
	vertexArray->setPrimitiveType(sf::PrimitiveType::Triangles);
	vertexArray->clear();
	states.texture = NULL;

	spine::VertexEffect* vertexEffect = nullptr;

	// Early out if skeleton is invisible
	if (skeleton->getColor().a == 0) return;

	if (vertexEffect != NULL) vertexEffect->begin(*skeleton);

	sf::Vertex vertex;
	sf::Texture* texture = NULL;
	for (unsigned i = 0; i < skeleton->getSlots().size(); ++i) {
		spine::Slot& slot = *skeleton->getDrawOrder()[i];
		spine::Attachment* attachment = slot.getAttachment();
		if (!attachment) continue;

		// Early out if the slot color is 0 or the bone is not active
		if (slot.getColor().a == 0 || !slot.getBone().isActive()) {
			clipper.clipEnd(slot);
			continue;
		}

		spine::Vector<float>* vertices = &worldVertices;
		int verticesCount = 0;
		spine::Vector<float>* uvs = NULL;
		spine::Vector<unsigned short>* indices = NULL;
		int indicesCount = 0;
		spine::Color* attachmentColor;

		if (attachment->getRTTI().isExactly(spine::RegionAttachment::rtti)) {
			spine::RegionAttachment* regionAttachment = (spine::RegionAttachment*)attachment;
			attachmentColor = &regionAttachment->getColor();

			// Early out if the slot color is 0
			if (attachmentColor->a == 0) {
				clipper.clipEnd(slot);
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
				clipper.clipEnd(slot);
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
			clipper.clipStart(slot, clip);
			continue;
		}
		else continue;

		sf::Uint8 r = static_cast<sf::Uint8>(skeleton->getColor().r * slot.getColor().r * attachmentColor->r * 255);
		sf::Uint8 g = static_cast<sf::Uint8>(skeleton->getColor().g * slot.getColor().g * attachmentColor->g * 255);
		sf::Uint8 b = static_cast<sf::Uint8>(skeleton->getColor().b * slot.getColor().b * attachmentColor->b * 255);
		sf::Uint8 a = static_cast<sf::Uint8>(skeleton->getColor().a * slot.getColor().a * attachmentColor->a * 255);
		vertex.color.r = r;
		vertex.color.g = g;
		vertex.color.b = b;
		vertex.color.a = a;

		sf::Color light;
		light.r = r / 255.0f;
		light.g = g / 255.0f;
		light.b = b / 255.0f;
		light.a = a / 255.0f;

		sf::BlendMode blend;
		//if (!usePremultipliedAlpha) {
		if (!true) {
			switch (slot.getData().getBlendMode()) {
			case spine::BlendMode_Normal:
				blend = normal;
				break;
			case spine::BlendMode_Additive:
				blend = additive;
				break;
			case spine::BlendMode_Multiply:
				blend = multiply;
				break;
			case spine::BlendMode_Screen:
				blend = screen;
				break;
			default:
				blend = normal;
			}
		}
		else {
			switch (slot.getData().getBlendMode()) {
			case spine::BlendMode_Normal:
				blend = normalPma;
				break;
			case spine::BlendMode_Additive:
				blend = additivePma;
				break;
			case spine::BlendMode_Multiply:
				blend = multiplyPma;
				break;
			case spine::BlendMode_Screen:
				blend = screenPma;
				break;
			default:
				blend = normalPma;
			}
		}

		if (states.texture == 0) states.texture = texture;

		if (states.blendMode != blend || states.texture != texture) {
			target.draw(*vertexArray, states);
			vertexArray->clear();
			states.blendMode = blend;
			states.texture = texture;
		}

		if (clipper.isClipping()) {
			clipper.clipTriangles(worldVertices, *indices, *uvs, 2);
			vertices = &clipper.getClippedVertices();
			verticesCount = clipper.getClippedVertices().size() >> 1;
			uvs = &clipper.getClippedUVs();
			indices = &clipper.getClippedTriangles();
			indicesCount = clipper.getClippedTriangles().size();
		}

		sf::Vector2u size = texture->getSize();

		/*if (vertexEffect != 0) {
			tempUvs.clear();
			tempColors.clear();
			for (int ii = 0; ii < verticesCount; ii++) {
				sf::Color vertexColor = light;
				sf::Color dark;
				dark.r = dark.g = dark.b = dark.a = 0;
				int index = ii << 1;
				float x = (*vertices)[index];
				float y = (*vertices)[index + 1];
				float u = (*uvs)[index];
				float v = (*uvs)[index + 1];
				vertexEffect->transform(x, y, u, v, vertexColor, dark);
				(*vertices)[index] = x;
				(*vertices)[index + 1] = y;
				tempUvs.add(u);
				tempUvs.add(v);
				tempColors.add(vertexColor);
			}

			for (int ii = 0; ii < indicesCount; ++ii) {
				int index = (*indices)[ii] << 1;
				vertex.position.x = (*vertices)[index];
				vertex.position.y = (*vertices)[index + 1];
				vertex.texCoords.x = (*uvs)[index] * size.x;
				vertex.texCoords.y = (*uvs)[index + 1] * size.y;
				Color vertexColor = tempColors[index >> 1];
				vertex.color.r = static_cast<Uint8>(vertexColor.r * 255);
				vertex.color.g = static_cast<Uint8>(vertexColor.g * 255);
				vertex.color.b = static_cast<Uint8>(vertexColor.b * 255);
				vertex.color.a = static_cast<Uint8>(vertexColor.a * 255);
				vertexArray->append(vertex);
			}
		}*/
		//else {
			for (int ii = 0; ii < indicesCount; ++ii) {
				int index = (*indices)[ii] << 1;
				vertex.position.x = (*vertices)[index];
				vertex.position.y = (*vertices)[index + 1];
				vertex.texCoords.x = (*uvs)[index] * size.x;
				vertex.texCoords.y = (*uvs)[index + 1] * size.y;
				vertexArray->append(vertex);
			}
		//}
		clipper.clipEnd(slot);
	}
	target.draw(*vertexArray, states);
	clipper.clipEnd();

	if (vertexEffect != 0) vertexEffect->end();
}

spine::TrackEntry* SpineEntity::setAnimation(size_t trackIndex, const std::string& animationName, bool loop) {
	return this->animationState->setAnimation(trackIndex, animationName.c_str(), loop);
}

spine::TrackEntry* SpineEntity::setAnimation(size_t trackIndex, spine::Animation* animation, bool loop) {
	return this->animationState->setAnimation(trackIndex, animation, loop);
}

spine::TrackEntry* SpineEntity::setEmptyAnimation(size_t trackIndex, float mixDuration) {
	return this->animationState->setEmptyAnimation(trackIndex, mixDuration);
}

void SpineEntity::setEmptyAnimations(float mixDuration) {
	return this->animationState->setEmptyAnimations(mixDuration);
}

spine::TrackEntry* SpineEntity::addAnimation(size_t trackIndex, const std::string& animationName, bool loop, float delay) {
	return this->animationState->addAnimation(trackIndex, animationName.c_str(), loop, delay);
}

spine::TrackEntry* SpineEntity::addAnimation(size_t trackIndex, spine::Animation* animation, bool loop, float delay) {
	return this->animationState->addAnimation(trackIndex, animation, loop, delay);
}

spine::TrackEntry* SpineEntity::addEmptyAnimation(size_t trackIndex, bool loop, float delay) {
	return this->animationState->addEmptyAnimation(trackIndex, loop, delay);
}

spine::Animation* SpineEntity::findAnimation(const std::string& animationName) const {
	return this->poseRef->skeletonData->findAnimation(animationName.c_str());
}

sf::Vector2f SpineEntity::getBonePosition(const std::string& boneName) const {
	spine::Bone* bone = this->skeleton->findBone(boneName.c_str());
	return sf::Vector2f(bone->getWorldX() / (-spine_to3d_scale_i) * spine_global_scale, bone->getWorldY() / spine_to3d_scale_i * spine_global_scale);
}

sf::Vector2f SpineEntity::getBonePositionByIndex(int boneIndex) const {
	spine::Bone* bone = (*(this->bonesRef))[boneIndex];
	return sf::Vector2f(bone->getWorldX() / (-spine_to3d_scale_i) * spine_global_scale, bone->getWorldY() / spine_to3d_scale_i * spine_global_scale);
}

int SpineEntity::getBoneIndex(const std::string& boneName) const {
	return this->skeleton->findBoneIndex(boneName.c_str());
}
// end class SpineEntity

// class SpineEntitySet
SpineEntitySet::SpineEntitySet(const ohms::SpinePose* pose) :
	poseRef(pose) {}

SpineEntitySet::~SpineEntitySet() {
	//while (!this->entities.empty()) {
	//	delete this->entities.top();
	//	this->entities.pop();
	//}
	return;
}

SpineEntity* SpineEntitySet::runOneEntity() {
	ohms::SpineEntity* res = nullptr;
	res = new SpineEntity(poseRef);
	return res;
}
// end class SpineEntitySet

// class SpineManager
SpineManager::SpineManager() {
	//this->timeScale = 1.0f;

	this->usePremultipliedAlpha = true;
	this->tempUvs.ensureCapacity(16);
	this->tempColors.ensureCapacity(16);

	quadIndices.clear();
	quadIndices.add(0);
	quadIndices.add(1);
	quadIndices.add(2);
	quadIndices.add(2);
	quadIndices.add(3);
	quadIndices.add(0);

	//this->timeScale = 1.0f;

	//this->shaderRef = nullptr;
	//this->cameraRef = nullptr;
	return;
}

SpineManager::~SpineManager() {
	return;
}

bool SpineManager::addPoseBinary(const std::string& binaryPath, const std::string& atlasPath) {
	ohms::SpinePose* pose = new ohms::SpinePose;

	// Load the texture atlas
	pose->atlas = new spine::Atlas(atlasPath.c_str(), &this->texLoader);
	if (pose->atlas->getPages().size() == 0) {
		printf("Failed to load atlas");
		delete pose;
		return false;
	}

	// Load the skeleton data
	spine::SkeletonBinary binary(pose->atlas);
	pose->skeletonData = binary.readSkeletonDataFile(binaryPath.c_str());

	if (!pose->skeletonData) {
		printf("Failed to load skeleton data");
		delete pose;
		return false;
	}
	// Setup
	pose->animationStateData = new spine::AnimationStateData(pose->skeletonData);

	ohms::SpineEntitySet* set = new ohms::SpineEntitySet(pose);
	this->sets.push_back(set);
	this->poses.push_back(pose);
	return true;
}

ohms::SpineEntitySet* SpineManager::addPose(const std::string& name, unsigned char type) {
	bool ok = false;
	switch (type) {
	case 0:
		ok = this->addPoseBinary(std::string("res/characters/").append(name).append("/animation/batf/skel").c_str(), std::string("res/characters/").append(name).append("/animation/batf/atlas").c_str());
		break;
	case 1:
		ok = this->addPoseBinary(std::string("res/characters/").append(name).append("/animation/batb/skel").c_str(), std::string("res/characters/").append(name).append("/animation/batb/atlas").c_str());
		break;
	case 2:
		ok = this->addPoseBinary(std::string("res/characters/").append(name).append("/animation/building/skel").c_str(), std::string("res/characters/").append(name).append("/animation/building/atlas").c_str());
		break;
	default:
		break;
	}
	if (!ok)
		return nullptr;
	return this->sets.back();
}

void SFMLTextureLoader::load(spine::AtlasPage& page, const spine::String& path) {
	sf::Texture* texture = new sf::Texture();
	if (!texture->loadFromFile(path.buffer())) return;

	if (page.magFilter == spine::TextureFilter_Linear) texture->setSmooth(true);
	if (page.uWrap == spine::TextureWrap_Repeat && page.vWrap == spine::TextureWrap_Repeat) texture->setRepeated(true);

	page.setRendererObject(texture);
	sf::Vector2u size = texture->getSize();
	page.width = size.x;
	page.height = size.y;
}

void SFMLTextureLoader::unload(void* texture) {
	delete (sf::Texture*)texture;
}

//SpineExtension* getDefaultExtension() {
//	return new DefaultSpineExtension();
//}

// end class SpineManager
} // end namespace ohms
