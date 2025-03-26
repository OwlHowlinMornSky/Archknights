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
#include <GL/glew.h>

#include <MysteryEngine/G3D/LineFrameModel.h>

#include <MysteryEngine/G3D/GlCheck.h>
#include <MysteryEngine/G3D/Vertex.h>
#include <MysteryEngine/G3D/Scene.h>
#include <glm/gtx/transform.hpp>
#include <MysteryEngine/G3D/G3dGlobal.h>
#include <MysteryEngine/Core/RandGen.h>

#include <array>

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

} // namespace

namespace ME {

LineFrameVertex::LineFrameVertex(glm::vec3 v0, glm::vec3 v1, glm::vec2 off) :
	vertex0(v0),
	vertex1(v1),
	offset(off),
	texCoord() {}

LineFrameShader::LineFrameShader() :
	m_ul_matp(0),
	m_ul_matv(0),
	m_ul_matm(0) {}

void LineFrameShader::setup() {
	clear();
	auto vid = loadFromMemory(g_vs2, ME::ShaderType::Vertex);
	auto fid = loadFromMemory(g_fs, ME::ShaderType::Fragment);
	glCheck(glBindAttribLocation(m_program, 0, "a_vertex0"));
	glCheck(glBindAttribLocation(m_program, 1, "a_vertex1"));
	glCheck(glBindAttribLocation(m_program, 2, "a_offset"));
	glCheck(glBindAttribLocation(m_program, 3, "a_texCoord"));
	linkShader();
	glCheck(glDetachShader(m_program, vid));
	glCheck(glDetachShader(m_program, fid));
	Bind(this);
	m_ul_matp = getUniformLocation("u_matP");
	m_ul_matv = getUniformLocation("u_matV");
	m_ul_matm = getUniformLocation("u_matM");
	updateUniform1iName("u_texture0", 0);
	Bind(nullptr);
	return;
}

void LineFrameShader::update(int id, GLfloat* data) const {
	switch (id) {
	case 0:
		updateUniformMat4fv(m_ul_matp, data);
		break;
	case 1:
		updateUniformMat4fv(m_ul_matv, data);
		break;
	case 2:
		updateUniformMat4fv(m_ul_matm, data);
		break;
	}
}

LineFrameModel::LineFrameModel() :
	vao(0),
	vertexVBO(0),
	drawCount(0) {}

bool LineFrameModel::LoadModelData(const std::vector<LineFrameVertex>& vertexArray) {
	this->drawCount = (unsigned int)vertexArray.size();

	unsigned int stride = (unsigned int)sizeof(vertexArray[0]);
	unsigned long long offset1 = sizeof(vertexArray[0].vertex0);
	unsigned long long offset2 = offset1 + sizeof(vertexArray[0].vertex1);
	unsigned long long offset3 = offset2 + sizeof(vertexArray[0].offset);

	ME::G3dGlobal::SetActive(true);

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

	ME::G3dGlobal::SetActive(false);
	return true;
}

void LineFrameModel::clear() {
	ME::G3dGlobal::SetActive(true);

	if (vertexVBO) {
		glCheck(glDeleteBuffers(1, &vertexVBO));
		vertexVBO = 0;
	}
	if (vao) {
		glCheck(glDeleteVertexArrays(1, &vao));
		vao = 0;
	}

	ME::G3dGlobal::SetActive(false);
}

void LineFrameModel::update(float dt) {}

void LineFrameModel::draw(ME::Camera* camera, ME::Shader* shader) {
	if (m_rotationChanged) {
		m_matM = glm::translate(m_position);
		m_matM *= glm::rotate(glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		m_matM *= glm::rotate(glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		m_matM *= glm::rotate(glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		m_matM *= glm::scale(m_scale);
		m_rotationChanged = false;
	}

	shader->update(0, &(camera->getMatP()[0][0]));
	shader->update(1, &(camera->getMatV()[0][0]));
	shader->update(2, &(m_matM[0][0]));

	glCheck(glBindVertexArray(this->vao));
	glCheck(glDrawArrays(GL_QUADS, 0, this->drawCount));
	glCheck(glBindVertexArray(0));
}

}
