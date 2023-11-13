/*
*    Archknights
*
*    Copyright (C) 2023  Tyler Parret True
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
#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace g3d {

/**
 * @brief 公共的顶点属性。
*/
namespace VertexAttribute {

enum : unsigned int {
	Position = 0, // 顶点坐标。
	TexCoord,     // 纹理坐标。
	Color,        // 颜色。
	Normal,       // 法向。
	COUNT         // [计数]
};

} // namespace VertexAttribute

/**
 * @brief 公共的基本顶点类型。
*/
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

	glm::vec3 position; // 顶点坐标。
	glm::vec2 texCoord; // 纹理坐标。
	glm::vec4 color;    // 颜色。
	glm::vec3 normal;   // 法向。
};

} // namespace g3d
