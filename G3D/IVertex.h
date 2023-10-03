#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace g3d {

namespace VertexAttribute {

enum : unsigned int {
	Position = 0,
	TexCoord,
	Color,
	Normal,
	COUNT
};

}

struct Vertex {
	Vertex() :
		position({ 0 }),
		normal({ 0 }),
		color({ 0 }),
		texCoord({ 0 }) {}

	Vertex(const glm::vec3& pos) :
		position(pos),
		color(1, 1, 1, 1),
		normal({ 0 }),
		texCoord({ 0 }) {}

	Vertex(const glm::vec3& pos, const glm::vec3& norm) :
		position(pos),
		normal(norm),
		color(1, 1, 1, 1),
		texCoord({ 0 }) {}

	Vertex(const glm::vec3& pos, const glm::vec2& tex) :
		position(pos),
		color(1, 1, 1, 1),
		texCoord(tex),
		normal({ 0 }) {}

	Vertex(const glm::vec3& pos, const glm::vec4& tint) :
		position(pos),
		color(tint),
		texCoord({ 0 }),
		normal({ 0 }) {}

	Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& tex) :
		position(pos),
		normal(norm),
		color(1, 1, 1, 1),
		texCoord(tex) {}

	Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec4& tint) :
		position(pos),
		normal(norm),
		color(tint),
		texCoord({ 0 }) {}

	Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec4& tint) :
		position(pos),
		color(tint),
		texCoord(tex),
		normal({ 0 }) {}

	Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec4& tint, const glm::vec2& tex) :
		position(pos),
		normal(norm),
		color(tint),
		texCoord(tex) {}

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec4 color;
	glm::vec2 texCoord;
};

}
