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
#pragma once

#include <MysteryEngine/G3D/IModel.h>
#include <MysteryEngine/G3D/Vertex.h>
#include <string_view>
#include <array>

namespace Game {

class AnimationFrames :
	public ME::IModel {
public:
	int Load(std::string_view file);

public:
	virtual void Update(float dt) override;
	virtual void Draw() override;

protected:
	float m_test;
	std::array<ME::Vertex, 4> vertexArray;
	unsigned int vao;
	unsigned int vertexVBO;
	unsigned int drawCount;
};

}
