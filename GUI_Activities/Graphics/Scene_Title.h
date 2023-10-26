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

#include <memory>
#include "../../G3D/base.h"
#include "../../G3D/GlCheck.h"
#include "../../G3D/Shader.h"
#include "../../G3D/Camera.h"
#include "../../G3D/IVertex.h"
#include "../../G3D/Camera.Orthographic.h"

#include <SFML/Graphics.hpp>

namespace {

class Shader_Title_Sphere final :
	public g3d::Shader {
public:
	virtual void setup();
	void update(g3d::Camera& camera);
protected:
	GLint m_ul_matp;
	GLint m_ul_matv;
public:
	GLint m_ul_matm;
};

class LineLineModel final :
	public g3d::ITransformR {
public:
	void update();
	bool LoadModelData(const std::vector<g3d::Vertex>& vertexArray);
	void Draw();
protected:
	unsigned int vao;
	unsigned int vertexVBO;
	unsigned int drawCount;
public:
	glm::mat4 m_matM;
};

} // namespace

class Scene_Title final :
	public sf::Drawable {
public:
	Scene_Title();
	~Scene_Title();

public:
	void setup(sf::Vector2u size);
	void update(float dt);
	void render();

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
	Shader_Title_Sphere m_shader;
	g3d::OrthographicCamera m_camera;
	LineLineModel m_llm;
	sf::RenderTexture m_rtex;
	sf::Sprite m_sp;
};
