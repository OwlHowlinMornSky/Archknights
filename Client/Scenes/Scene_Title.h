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
#include <MysteryEngine/G3D/Shader.h>
#include <MysteryEngine/G3D/Camera.h>
#include <MysteryEngine/G3D/Vertex.h>
#include <MysteryEngine/G3D/Camera.Orthographic.h>
#include <MysteryEngine/G3D/ITransformS.h>

#include "Scene_ITitle.h"

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

class Shader_Title_Sphere final :
	public ME::Shader {
public:
	virtual void setup();
	void update(ME::Camera& camera);
protected:
	GLint m_ul_matp;
	GLint m_ul_matv;
public:
	GLint m_ul_matm;
};

class LineModel final :
	public ME::ITransformR,
	public ME::ITransformT,
	public ME::ITransformS {
public:
	void update();
	bool LoadModelData(const std::vector<::Vertex>& vertexArray);
	void Draw();
protected:
	unsigned int vao;
	unsigned int vertexVBO;
	unsigned int drawCount;
public:
	glm::mat4 m_matM;
};

} // namespace

namespace Scene {

class Title final :
	public ITitle {
public:
	Title();
	~Title();

public:
	virtual void setup(sf::Vector2u size) override;
	virtual void update(float dt) override;
	virtual void render() override;

	virtual void resize(sf::Vector2u size) override;

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
	::Shader_Title_Sphere m_shader;
	ME::OrthographicCamera m_camera;
	::LineModel m_llm;
	sf::RenderTexture m_rtex;
	sf::Sprite m_sp;
	float m_rotSpeed[3];
};

} // namespace title
