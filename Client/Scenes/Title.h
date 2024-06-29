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

#include <MysteryEngine/G3D/GlCheck.h>
#include <MysteryEngine/G3D/Vertex.h>
#include <MysteryEngine/G3D/IModel.h>

#include "ITitle.h"

namespace {

struct Vertex {
	glm::vec3 vertex0;
	glm::vec3 vertex1;
	glm::vec2 offset;
	glm::vec2 texCoord;

	Vertex(glm::vec3 v0, glm::vec3 v1, glm::vec2 off) :
		vertex0(v0),
		vertex1(v1),
		offset(off),
		texCoord() {}
};

class TitleSphereShader final :
	public ME::Shader {
public:
	virtual void setup() override;

	virtual void UpdateUniform(int id, GLfloat* data) const override;

protected:
	GLint m_ul_matp;
	GLint m_ul_matv;
	GLint m_ul_matm;
};

class LineModel final :
	public ME::IModel {
public:
	bool LoadModelData(const std::vector<::Vertex>& vertexArray);

	virtual void Clear() override;

	virtual void Update(float dt) override;
	virtual void Draw(ME::Camera* camera, ME::Shader* shader) override;

protected:
	unsigned int vao;
	unsigned int vertexVBO;
	unsigned int drawCount;
};

} // namespace

namespace Scene {

class Title final :
	public ITitle {
public:
	Title();
	~Title();

public:
	virtual void SetScale(float r) override;
	virtual void SetOffset(float r) override;

public:
	virtual void setup(int code = 0, void* data = nullptr) override;
	virtual void clear() override;
	virtual void update(float dt) override;

protected:
	virtual void onRender();
	virtual void onSizeChanged(sf::Vector2u newsize);

protected:
	::TitleSphereShader m_shader;
	::LineModel m_llm;
	float m_rotSpeed[3];
};

} // namespace title
