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
#include <GL/glew.h>

#include "Scene_Title.h"

#include <glm/gtx/transform.hpp>
#include <MysteryEngine/G3D/G3dGlobal.h>
#include <MysteryEngine/Core/RandGen.h>

#include <array>
#include <vector>

namespace {

const char g_vs2[] =
"#version 330\n\
attribute vec3 a_vertex0;\
attribute vec3 a_vertex1;\
attribute vec2 a_offset;\
attribute vec2 a_texCoord;\
uniform mat4 u_matP;\
uniform mat4 u_matV;\
uniform mat4 u_matM;\
varying float v_alpha;\
varying vec2 v_uv;\
\
void main() {\
  vec4 vertex0 = vec4(a_vertex0, 1.0);\
  vec4 vertex1 = vec4(a_vertex1, 1.0);\
  vec4 center = vec4(0.0, 0.0, 0.0, 1.0);\
  \
  mat4 mat_vm = u_matV * u_matM;\
  vertex0 = mat_vm * vertex0;\
  vertex1 = mat_vm * vertex1;\
  center = mat_vm * center;\
  \
  vertex0 *= center.z / vertex0.z;\
  vertex1 *= center.z / vertex1.z;\
  \
  vec4 delta = vertex1 - vertex0;\
  vec2 nn = vec2(-delta.y, delta.x);\
  nn /= length(nn);\
  nn *= a_offset.y;\
  \
  vec4 finalPosition = mix(vertex0, vertex1, a_offset.x);\
  finalPosition.xy += nn;\
  \
  gl_Position = u_matP * finalPosition;\
  \
  vec4 depthPosition = vec4(mix(a_vertex0, a_vertex1, a_offset.x), 1.0);\
  depthPosition = u_matM * depthPosition;\
  v_alpha = 0.75 + depthPosition.z / 2.0;\
  v_uv = a_texCoord;\
}";

const char g_fs[] =
"#version 330\n"\
"uniform sampler2D u_texture0;"\
"varying float v_alpha;"\
"varying vec2 v_uv;"\
"void main() {"\
"  vec4 cl = vec4(1.0, 0.9, 0.0, 1.0);"
"  gl_FragColor = cl * 0.8 * min(1.0, v_alpha);"
"}";

void Shader_Title_Sphere::setup() {
	clear();
	loadFromMemory(g_vs2, ME::ShaderType::Vertex);
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
	this->updateUniformMat4fv(m_ul_matp, &(camera.getMatP()[0][0]));
	this->updateUniformMat4fv(m_ul_matv, &(camera.getMatV()[0][0]));
}

void LineModel::update() {
	if (m_rotationChanged) {
		m_matM = glm::translate(m_position);
		m_matM *= glm::rotate(glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		m_matM *= glm::rotate(glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		m_matM *= glm::rotate(glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
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

constexpr unsigned long Vertices[] = {
	0x3f59c440, 0, 0x3f069650,
	0x3f069650, 0x3f59c440, 0,
	0, 0x3f069650, 0x3f59c440,
	0, 0xbf069650, 0x3f59c440,
	0x3f069650, 0xbf59c440, 0,
	0x3f59c440, 0, 0xbf069650,
	0xbf59c440, 0, 0xbf069650,
	0, 0x3f069650, 0xbf59c440,
	0xbf069650, 0x3f59c440, 0,
	0xbf59c440, 0, 0x3f069650,
	0xbf069650, 0xbf59c440, 0,
	0, 0xbf069650, 0xbf59c440,
	0x3f4f1bbd, 0x3f000000, 0x3e9e3779,
	0x3f000000, 0x3e9e3779, 0x3f4f1bbd,
	0x3f000000, 0xbe9e3779, 0x3f4f1bbd,
	0x3f4f1bbd, 0xbf000000, 0x3e9e3779,
	0x3f800000, 0, 0,
	0x3e9e3779, 0x3f4f1bbd, 0x3f000000,
	0, 0, 0x3f800000,
	0x3e9e3779, 0xbf4f1bbd, 0x3f000000,
	0x3f4f1bbd, 0xbf000000, 0xbe9e3779,
	0x3f4f1bbd, 0x3f000000, 0xbe9e3779,
	0x3e9e3779, 0x3f4f1bbd, 0xbf000000,
	0xbe9e3779, 0x3f4f1bbd, 0x3f000000,
	0xbf000000, 0xbe9e3779, 0x3f4f1bbd,
	0, 0xbf800000, 0,
	0x3f000000, 0xbe9e3779, 0xbf4f1bbd,
	0, 0x3f800000, 0,
	0xbf000000, 0x3e9e3779, 0x3f4f1bbd,
	0xbe9e3779, 0xbf4f1bbd, 0x3f000000,
	0x3e9e3779, 0xbf4f1bbd, 0xbf000000,
	0x3f000000, 0x3e9e3779, 0xbf4f1bbd,
	0xbf000000, 0x3e9e3779, 0xbf4f1bbd,
	0xbf4f1bbd, 0x3f000000, 0xbe9e3779,
	0xbf800000, 0, 0,
	0xbf4f1bbd, 0xbf000000, 0xbe9e3779,
	0xbf000000, 0xbe9e3779, 0xbf4f1bbd,
	0xbe9e3779, 0x3f4f1bbd, 0xbf000000,
	0xbf4f1bbd, 0x3f000000, 0x3e9e3779,
	0xbf4f1bbd, 0xbf000000, 0x3e9e3779,
	0xbe9e3779, 0xbf4f1bbd, 0xbf000000,
	0, 0, 0xbf800000
};

constexpr int Indices[] = {
	0, 12, 12, 1, 0, 13, 13, 2, 0, 14, 14, 3, 0, 15, 15, 4, 0, 16, 16, 5, 1, 17, 17,
	2, 2, 18, 18, 3, 3, 19, 19, 4, 4, 20, 20, 5, 5, 21, 21, 1, 1, 22, 22, 7, 2, 23, 23,
	8, 3, 24, 24, 9, 4, 25, 25, 10, 5, 26, 26, 11, 8, 27, 27, 1, 9, 28, 28, 2, 10, 29,
	29, 3, 11, 30, 30, 4, 7, 31, 31, 5, 6, 32, 32, 7, 6, 33, 33, 8, 6, 34, 34, 9, 6,
	35, 35, 10, 6, 36, 36, 11, 7, 37, 37, 8, 8, 38, 38, 9, 9, 39, 39, 10, 10, 40, 40,
	11, 11, 41, 41, 7, 13, 12, 12, 17, 17, 13, 14, 13, 13, 18, 18, 14, 15, 14, 14, 19,
	19, 15, 16, 15, 15, 20, 20, 16, 12, 16, 16, 21, 21, 12, 32, 33, 33, 37, 37, 32, 33,
	34, 34, 38, 38, 33, 34, 35, 35, 39, 39, 34, 35, 36, 36, 40, 40, 35, 36, 32, 32, 41,
	41, 36, 27, 22, 22, 37, 37, 27, 28, 23, 23, 38, 38, 28, 29, 24, 24, 39, 39, 29, 30,
	25, 25, 40, 40, 30, 31, 26, 26, 41, 41, 31, 27, 23, 23, 17, 17, 27, 28, 24, 24, 18,
	18, 28, 29, 25, 25, 19, 19, 29, 30, 26, 26, 20, 20, 30, 31, 22, 22, 21, 21, 31,
};

} // namespace

namespace Scene {

Title::Title() :
	m_rotSpeed() {
	m_camera.setType(ME::Camera::Type::Perspective);
}

Title::~Title() {}

void Title::SetScale(float r) {
	sf::Vector2u size = m_rtex.getSize();
	m_camera.setAspectRatio(1.0f * size.x / size.y);
	return;
}

void Title::SetOffset(float r) {
	m_llm.setPosition(0.0f, 0.0f, 0.0f - r);
}

void Title::setup(sf::Vector2u size) {
	m_rtex.create(size.x, size.y);
	m_sp.setTexture(m_rtex.getTexture(), true);

	m_camera.setAspectRatio(1.0f * size.x / size.y);
	m_camera.setPosition(0.0f, 0.0f, 12.0f);
	m_camera.setFOV(9.0f);

	float thick = 0.0075f;

	union I2F { // 用于把bin转化为float
		unsigned long i = 0;
		float f;
	} a[3];

	std::array<glm::vec3, 42> vertices;
	for (int i = 0; i < 42; ++i) {
		a[0].i = Vertices[i * 3];
		a[1].i = Vertices[i * 3 + 1];
		a[2].i = Vertices[i * 3 + 2];
		vertices[i] = { a[0].f, a[1].f, a[2].f };
	}

	std::vector<::Vertex> va;
	va.reserve(480);
	for (int i = 0; i < 120; ++i) {
		glm::vec3& v0 = vertices[Indices[(i << 1)]];
		glm::vec3& v1 = vertices[Indices[(i << 1) | 1]];

		va.emplace_back(v0, v1, glm::vec2(0.0f, thick));
		va.emplace_back(v0, v1, glm::vec2(0.0f, -thick));
		va.emplace_back(v0, v1, glm::vec2(1.0f, -thick));
		va.emplace_back(v0, v1, glm::vec2(1.0f, thick));
	}

	m_rotSpeed[0] = ME::RandGen::getUni01() * 20.0f + 5.0f;
	m_rotSpeed[1] = ME::RandGen::getUni01() * 20.0f + 5.0f;
	m_rotSpeed[2] = ME::RandGen::getUni01() * 20.0f + 5.0f;

	m_llm.setRotation(
		ME::RandGen::getUni01() * 360.0f,
		ME::RandGen::getUni01() * 360.0f,
		ME::RandGen::getUni01() * 360.0f
	);

	ME::G3dGlobal::setActive(true);

	m_shader.setup();

	glCheck(glEnable(GL_BLEND));
	glCheck(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));

	glCheck(glClearColor(0.2f, 0.2f, 0.2f, 1.0f)); // 设置clear颜色

	m_llm.LoadModelData(va);

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
	m_camera.setAspectRatio(1.0f * size.x / size.y);
}

void Title::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	return target.draw(m_sp, states);
}

} // namespace Scene
