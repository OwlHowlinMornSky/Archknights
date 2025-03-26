/*
*    Mystery Engine
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
#include <vector>

namespace ME {

struct ME_API LineFrameVertex {
	glm::vec3 vertex0;
	glm::vec3 vertex1;
	glm::vec2 offset;
	glm::vec2 texCoord;

	LineFrameVertex(glm::vec3 v0, glm::vec3 v1, glm::vec2 off);
};

class ME_API LineFrameShader final :
	public ME::Shader {
public:
	LineFrameShader();
	virtual ~LineFrameShader() = default;

public:
	virtual void setup() override;
	virtual void update(int id, GLfloat* data) const override;

protected:
	GLint m_ul_matp;
	GLint m_ul_matv;
	GLint m_ul_matm;
};

class ME_API LineFrameModel final :
	public ME::IModel {
public:
	LineFrameModel();
	virtual ~LineFrameModel() = default;

public:
	bool LoadModelData(const std::vector<LineFrameVertex>& vertexArray);

	virtual void clear() override;

	virtual void update(float dt) override;
	virtual void draw(ME::Camera* camera, ME::Shader* shader) override;

protected:
	unsigned int vao;
	unsigned int vertexVBO;
	unsigned int drawCount;
};

}
