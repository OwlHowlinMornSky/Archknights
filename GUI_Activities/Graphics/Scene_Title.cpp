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
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Scene_Title.h"

#include <vector>

namespace {

const char g_vs[] =
"#version 330\n"\
"layout(location = 0) attribute vec3 a_offset;"\
"layout(location = 1) attribute vec2 a_texCoord;"\
"layout(location = 2) attribute vec3 a_vertex1;"\
"layout(location = 3) attribute vec3 a_vertex0;"\
"uniform mat4 u_matP;"\
"uniform mat4 u_matV;"\
"uniform mat4 u_matM;"\
"varying float v_alpha;"\
"varying vec2 v_uv;"\
"void main() {"\
"  vec4 vertex0 = vec4(a_vertex0, 1.0);"\
"  vec4 vertex1 = vec4(a_vertex1, 1.0);"\
"  mat4 mat_vm = u_matV * u_matM;"\
"  vertex0 = mat_vm * vertex0;"\
"  vertex1 = mat_vm * vertex1;"\
"  vec4 delta = vertex1 - vertex0;"\
"  vec2 nn = vec2(-delta.y, delta.x);"\
"  nn = nn / length(nn);"\
"  nn *= a_offset.y;"\
"  vec4 finalPosition = mix(vertex0, vertex1, a_offset.x);"\
"  finalPosition.xy += nn;"\
"  gl_Position = u_matP * finalPosition;"\
"  vec4 depthPosition = vec4(mix(a_vertex0, a_vertex1, a_offset.x), 1.0);"\
"  depthPosition = u_matM * depthPosition;"\
"  v_alpha = 0.8 + depthPosition.z / 3.0;"\
"  v_uv = a_texCoord;"\
"}";

const char g_fs[] =
"#version 330\n"\
"uniform sampler2D u_texture0;"\
"varying float v_alpha;"\
"varying vec2 v_uv;"\
"void main() {"\
"  vec4 cl = vec4(1.0, 0.9, 0.0, 1.0);"
"  gl_FragColor = cl;"\
"}";
//"  gl_FragColor = cl * 0.8 * min(1.0, v_alpha);"

void Shader_Title_Sphere::setup() {
	clear();
	loadFromMemory(g_vs, g3d::ShaderType::Vertex);
	loadFromMemory(g_fs, g3d::ShaderType::Fragment);
	linkShader();
	Bind(this);
	m_ul_matp = getUniformLocation("u_matP");
	m_ul_matv = getUniformLocation("u_matV");
	m_ul_matm = getUniformLocation("u_matM");
	updateUniform1iName("u_texture0", 0);
	Bind(nullptr);
	return;
}

void Shader_Title_Sphere::update(g3d::Camera& camera) {
	Bind(this);
	//glm::mat4 id{};
	this->updateUniformMat4fv(m_ul_matp, &(camera.getMatP()[0][0]));
	this->updateUniformMat4fv(m_ul_matv, &(camera.getMatV()[0][0]));
	//this->updateUniformMat4fv(m_ul_matm, &(id[0][0]));
}

void LineLineModel::update() {
	if (m_rotationChanged) {
		m_matM = glm::rotate(glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		m_matM *= glm::rotate(glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		m_matM *= glm::rotate(glm::radians(m_rotation.y), glm::vec3(0.0f, 0.0f, 1.0f));
		m_rotationChanged = false;
	}
}

bool LineLineModel::LoadModelData(const std::vector<g3d::Vertex>& vertexArray) {
	this->drawCount = (unsigned int)vertexArray.size();

	unsigned int stride = (unsigned int)sizeof(vertexArray[0]);
	unsigned long long texCoordOffset = sizeof(vertexArray[0].position);
	unsigned long long colorOffset = texCoordOffset + sizeof(vertexArray[0].texCoord);
	unsigned long long normalOffset = colorOffset + sizeof(vertexArray[0].color);

	if (this->vertexVBO) {
		glCheck(glDeleteBuffers(1, &this->vertexVBO));
		glCheck(glDeleteVertexArrays(1, &this->vao));
	}
	glCheck(glGenVertexArrays(1, &this->vao));
	glCheck(glBindVertexArray(this->vao));
	glCheck(glGenBuffers(1, &this->vertexVBO));

	glCheck(glBindBuffer(GL_ARRAY_BUFFER, this->vertexVBO));
	glCheck(glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * stride, vertexArray.data(), GL_STATIC_DRAW));
	glCheck(glEnableVertexAttribArray(0));
	glCheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0));
	glCheck(glEnableVertexAttribArray(1));
	glCheck(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)texCoordOffset));
	glCheck(glEnableVertexAttribArray(2));
	glCheck(glVertexAttribPointer(2, /*4*/3, GL_FLOAT, GL_FALSE, stride, (void*)colorOffset));
	glCheck(glEnableVertexAttribArray(3));
	glCheck(glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, stride, (void*)normalOffset));

	glCheck(glBindVertexArray(0));
	return true;
}

void LineLineModel::Draw() {
	glCheck(glBindVertexArray(this->vao));
	glCheck(glDrawArrays(GL_QUADS, 0, this->drawCount));
	glCheck(glBindVertexArray(0));
}

} // namespace

Scene_Title::Scene_Title() {
	m_camera.setDim(16.0f, 9.0f);
	m_camera.setPosition(0.0f, 0.0f, 10.0f);
}

Scene_Title::~Scene_Title() {}

void Scene_Title::setup(sf::Vector2u size) {
	//m_rtex.create(size.x, size.y, sf::ContextSettings(24u));
	m_rtex.create(size.x, size.y);
	m_sp.setTexture(m_rtex.getTexture(), true);

	g3d::base::setActive(true);

	m_shader.setup();

	glCheck(glEnable(GL_BLEND));
	glCheck(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));

	glm::vec3 v0(0.0f, 0.0f, 0.0f);
	glm::vec4 v1(2.0f, 2.0f, 2.0f, 0.0f);
	float thick = 0.1f;
	std::vector<g3d::Vertex> va;
	va.push_back(g3d::Vertex(glm::vec3(0.0f, thick, 0.0f), v0, v1));
	va.push_back(g3d::Vertex(glm::vec3(0.0f, -thick, 0.0f), v0, v1));
	va.push_back(g3d::Vertex(glm::vec3(1.0f, -thick, 0.0f), v0, v1));
	va.push_back(g3d::Vertex(glm::vec3(1.0f, thick, 0.0f), v0, v1));
	m_llm.LoadModelData(va);
	//linetest->Position = { -20.0f, 0.0f, 0.0f };
	//linetest->load();}

	g3d::base::setActive(false);
}

void Scene_Title::update(float dt) {
	m_llm.rotate(20.0f * dt, 40.0f * dt, 60.0f * dt);
	m_llm.normalizeRotation();
}

void Scene_Title::render() {
	g3d::base::setActive(true);
	m_rtex.setActive(true);
	glCheck(glClearColor(1.0f, 0.0f, 0.0f, 0.5f)); // 设置clear颜色
	glCheck(glClear(GL_COLOR_BUFFER_BIT));

	sf::IntRect Viewport(
		0, 0,
		m_rtex.getSize().x,
		m_rtex.getSize().y
	);
	glCheck(glViewport(Viewport.left, Viewport.top, Viewport.width, Viewport.height));

	m_llm.update();
	m_shader.update(m_camera);

	g3d::Shader::Bind(&m_shader);
	m_shader.updateUniformMat4fv(m_shader.m_ul_matm, &(m_llm.m_matM[0][0]));

	m_llm.Draw();

	g3d::Shader::Bind(nullptr);

	m_rtex.display();

	m_rtex.setActive(false);
	g3d::base::setActive(false);
}

void Scene_Title::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(m_sp, states);
	return;
}
