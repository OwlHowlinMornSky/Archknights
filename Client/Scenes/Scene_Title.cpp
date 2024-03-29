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
#include <GL/glew.h>

#include "Scene_Title.h"

//#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <MysteryEngine/G3D/G3dGlobal.h>
#include <MysteryEngine/Core/RandGen.h>

#include <array>
#include <vector>
#include <fstream>
#include <iostream>

namespace {

const char g_vs[] =
"#version 330\n"\
"attribute vec3 a_vertex0;"\
"attribute vec3 a_vertex1;"\
"attribute vec2 a_offset;"\
"attribute vec2 a_texCoord;"\
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
"  v_alpha = 0.75 + depthPosition.z / 2.0;"\
"  v_uv = a_texCoord;"\
"}";

const char g_fs[] =
"#version 330\n"\
"uniform sampler2D u_texture0;"\
"varying float v_alpha;"\
"varying vec2 v_uv;"\
"void main() {"\
"  vec4 cl = vec4(1.0, 0.9, 0.0, 1.0);"
"  gl_FragColor = cl * 0.8 * min(1.0, v_alpha);"
"}";
//"  gl_FragColor = cl * 0.8 * min(1.0, v_alpha);"
//"  gl_FragColor = cl;"\

void Shader_Title_Sphere::setup() {
	clear();
	loadFromMemory(g_vs, ME::ShaderType::Vertex);
	loadFromMemory(g_fs, ME::ShaderType::Fragment);
	glCheck(glBindAttribLocation(m_program, 0, "a_vertex0"));
	glCheck(glBindAttribLocation(m_program, 1, "a_vertex1"));
	glCheck(glBindAttribLocation(m_program, 2, "a_offset"));
	glCheck(glBindAttribLocation(m_program, 3, "a_texCoord"));
	linkShader();
	Bind(this);
	m_ul_matp = getUniformLocation("u_matP");
	m_ul_matv = getUniformLocation("u_matV");
	m_ul_matm = getUniformLocation("u_matM");
	updateUniform1iName("u_texture0", 0);
	Bind(nullptr);
	return;
}

void Shader_Title_Sphere::update(ME::Camera& camera) {
	Bind(this);
	//glm::mat4 id{};
	this->updateUniformMat4fv(m_ul_matp, &(camera.getMatP()[0][0]));
	this->updateUniformMat4fv(m_ul_matv, &(camera.getMatV()[0][0]));
	//this->updateUniformMat4fv(m_ul_matm, &(id[0][0]));
}

void LineModel::update() {
	if (m_rotationChanged) {
		m_matM = glm::translate(m_position);
		m_matM *= glm::rotate(glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		m_matM *= glm::rotate(glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		m_matM *= glm::rotate(glm::radians(m_rotation.y), glm::vec3(0.0f, 0.0f, 1.0f));
		m_matM *= glm::scale(m_scale);
		m_rotationChanged = false;
	}
}

bool LineModel::LoadModelData(const std::vector<::Vertex>& vertexArray) {
	this->drawCount = (unsigned int)vertexArray.size();

	unsigned int stride = (unsigned int)sizeof(vertexArray[0]);
	unsigned long long offset1 = sizeof(vertexArray[0].vertex0);
	unsigned long long offset2 = offset1 + sizeof(vertexArray[0].vertex1);
	unsigned long long offset3 = offset2 + sizeof(vertexArray[0].offset);

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
	glCheck(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset1));
	glCheck(glEnableVertexAttribArray(2));
	glCheck(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offset2));
	glCheck(glEnableVertexAttribArray(3));
	glCheck(glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (void*)offset3));

	glCheck(glBindVertexArray(0));
	return true;
}

void LineModel::Draw() {
	glCheck(glBindVertexArray(this->vao));
	glCheck(glDrawArrays(GL_QUADS, 0, this->drawCount));
	glCheck(glBindVertexArray(0));
}

} // namespace

namespace Scene {

Title::Title() :
	m_rotSpeed() {}

Title::~Title() {}

void Title::SetScale(float r) {
	sf::Vector2u size = m_rtex.getSize();
	m_camera.setDim(9.0f / 4 * size.x / size.y / r, 9.0f / 4 / r);
	return;
}

void Title::SetOffset(float r) {
	m_llm.setPosition(0.0f, 0.0f, 0.0f - r);
}

void Title::setup(sf::Vector2u size) {
	//m_rtex.create(size.x, size.y, sf::ContextSettings(24u));
	m_rtex.create(size.x, size.y);
	m_sp.setTexture(m_rtex.getTexture(), true);

	m_camera.setDim(9.0f / 4 * size.x / size.y, 9.0f / 4);
	m_camera.setPosition(0.0f, 0.0f, 10.0f);

	//glm::vec3 v0(0.0f, 0.0f, 0.0f);
	//glm::vec4 v1(2.0f, 2.0f, 2.0f, 0.0f);
	float thick = 0.0075f;

	union I2F { // 用于把bin转化为float
		unsigned long i = 0;
		float f;
	} a[3];

	std::ifstream ifs;
	ifs.open("assets/sphere.txt", std::ios::in);
	if (!ifs.is_open()) {
		std::cout << "Failed to Open Model File!" << std::endl;
	}
	ifs.unsetf(std::ios::dec);
	ifs.setf(std::ios::hex); // 以16进制输入

	std::array<glm::vec3, 42> vertices;
	for (int i = 0; i < 42; ++i) {
		ifs >> a[0].i >> a[1].i >> a[2].i;
		vertices[i] = { a[0].f, a[1].f, a[2].f };
	}

	ifs.unsetf(std::ios::hex);
	ifs.setf(std::ios::dec); // 以10进制输入

	std::vector<::Vertex> va;
	va.reserve(480);
	for (int i = 0; i < 120; ++i) {
		int ii[2];
		ifs >> ii[0] >> ii[1];

		glm::vec3& v0 = vertices[ii[0]];
		glm::vec3& v1 = vertices[ii[1]];

		va.emplace_back(v0, v1, glm::vec2(0.0f, thick));
		va.emplace_back(v0, v1, glm::vec2(0.0f, -thick));
		va.emplace_back(v0, v1, glm::vec2(1.0f, -thick));
		va.emplace_back(v0, v1, glm::vec2(1.0f, thick));
	}

	m_rotSpeed[0] = ME::RandGen::getUni01() * 20.0f + 5.0f;
	m_rotSpeed[1] = ME::RandGen::getUni01() * 20.0f + 5.0f;
	m_rotSpeed[2] = ME::RandGen::getUni01() * 20.0f + 5.0f;


	ME::G3dGlobal::setActive(true);

	m_shader.setup();

	glCheck(glEnable(GL_BLEND));
	glCheck(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));

	glCheck(glClearColor(0.2f, 0.2f, 0.2f, 1.0f)); // 设置clear颜色


	m_llm.LoadModelData(va);
	//linetest->Position = { -20.0f, 0.0f, 0.0f };
	//linetest->load();}

	//m_llm.setPosition(0.5f, 0, 0);
	//m_llm.setScale(0.1f);

	ME::G3dGlobal::setActive(false);
}

void Title::update(sf::Time dt) {
	float ddt = dt.asSeconds();
	m_llm.rotate(m_rotSpeed[0] * ddt, m_rotSpeed[1] * ddt, m_rotSpeed[2] * ddt);
	m_llm.normalizeRotation();
}

void Title::render() {
	ME::G3dGlobal::setActive(true);
	m_rtex.setActive(true);
	glCheck(glClear(GL_COLOR_BUFFER_BIT));
	glCheck(glViewport(0, 0, m_rtex.getSize().x, m_rtex.getSize().y));

	m_llm.update();
	m_shader.update(m_camera);

	ME::Shader::Bind(&m_shader);
	m_shader.updateUniformMat4fv(m_shader.m_ul_matm, &(m_llm.m_matM[0][0]));

	m_llm.Draw();

	ME::Shader::Bind(nullptr);

	m_rtex.display();

	//m_rtex.setActive(false);
	ME::G3dGlobal::setActive(false);
}

void Title::resize(sf::Vector2u size) {
	m_rtex.create(size.x, size.y);
	m_sp.setTexture(m_rtex.getTexture(), true);
	m_camera.setDim(9.0f / 4 * size.x / size.y, 9.0f / 4);
}

void Title::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	return target.draw(m_sp, states);
}

} // namespace Scene
