﻿/*
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
#pragma once

#include <MysteryEngine/G3D/IModel.h>
#include <memory>
#include <list>
#include "IActorGroup.h"

namespace Game {

namespace ActorVertexAttribute {
enum : unsigned int {
	Position = 0, // 顶点坐标。
	TexCoord,     // 纹理坐标。
	Color,        // 颜色。
	COUNT         // [计数]
};
} // namespace ActorVertexAttribute

struct ActorVertex {
	ActorVertex() :
		position({ 0 }),
		color({ 0 }),
		texCoord({ 0 }) {}

	ActorVertex(const glm::vec2& pos) :
		position(pos),
		color(1, 1, 1, 1),
		texCoord({ 0 }) {}

	ActorVertex(const glm::vec2& pos, const glm::vec2& tex) :
		position(pos),
		color(1, 1, 1, 1),
		texCoord(tex) {}

	ActorVertex(const glm::vec2& pos, const glm::vec4& tint) :
		position(pos),
		color(tint),
		texCoord({ 0 }) {}

	ActorVertex(const glm::vec2& pos, const glm::vec2& tex, const glm::vec4& tint) :
		position(pos),
		color(tint),
		texCoord(tex) {}

	glm::vec2 position; // 顶点坐标。
	glm::vec2 texCoord; // 纹理坐标。
	glm::vec4 color;    // 颜色。
};

class ActorGroup final:
	public IActorGroup {
public:
	ActorGroup();

public:
	void AddActor(std::shared_ptr<IModel> actor);

public:
	virtual void Update(float dt) override;
	virtual void Draw(ME::Camera& camera, ME::Shader& shader) override;

protected:
	std::list<std::shared_ptr<IModel>> m_actors;
	std::unique_ptr<ME::Shader> m_shader;
};

}
