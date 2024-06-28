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
#include "Shadow.h"

#include <MysteryEngine/G3D/G3dGlobal.h>
#include <MysteryEngine/G3D/GlCheck.h>

Game::Shadow::Shadow() :
	m_vertexVBO(0),
	m_vao(0) {}

Game::Shadow::~Shadow() {}

bool Game::Shadow::Setup() {
	for (int i = 0; i < 4; ++i) {
		m_vertex[i].color = { 1.0f, 0.0f, 0.0f, 1.0f };
		m_vertex[i].texCoord = { 0.0f, 0.0f };
	}
	m_vertex[0].position = { -0.25f, -0.25f };
	m_vertex[1].position = { 0.25f, -0.25f };
	m_vertex[2].position = { 0.25f, 0.25f };
	m_vertex[3].position = { -0.25f, 0.25f };

	ME::G3dGlobal::setActive(true);

	glCheck(glGenVertexArrays(1, &m_vao));
	glCheck(glBindVertexArray(m_vao));
	glCheck(glGenBuffers(1, &m_vertexVBO));
	glCheck(glBindVertexArray(0));

	constexpr GLsizei drawCount = (GLsizei)4;
	constexpr GLsizei stride = sizeof(m_vertex[0]);
	constexpr size_t texCoordOffset = sizeof(m_vertex[0].position);
	constexpr size_t colorOffset = texCoordOffset + sizeof(m_vertex[0].texCoord);

	glCheck(glBindVertexArray(m_vao));

	glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO));

	glCheck(glBufferData(GL_ARRAY_BUFFER, drawCount * stride, &(m_vertex[0]), GL_STATIC_DRAW));
	glCheck(glEnableVertexAttribArray(static_cast<GLuint>(Game::ActorVertexAttribute::Position)));
	glCheck(glVertexAttribPointer(static_cast<GLuint>(Game::ActorVertexAttribute::Position), 2, GL_FLOAT, GL_FALSE, stride, 0));
	glCheck(glEnableVertexAttribArray(static_cast<GLuint>(Game::ActorVertexAttribute::TexCoord)));
	glCheck(glVertexAttribPointer(static_cast<GLuint>(Game::ActorVertexAttribute::TexCoord), 2, GL_FLOAT, GL_FALSE, stride, (void*)texCoordOffset));
	glCheck(glEnableVertexAttribArray(static_cast<GLuint>(Game::ActorVertexAttribute::Color)));
	glCheck(glVertexAttribPointer(static_cast<GLuint>(Game::ActorVertexAttribute::Color), 4, GL_FLOAT, GL_FALSE, stride, (void*)colorOffset));

	glCheck(glBindVertexArray(0));

	ME::G3dGlobal::setActive(false);
	return true;
}

void Game::Shadow::Clear() {
	ME::G3dGlobal::setActive(true);

	if (m_vertexVBO) {
		glCheck(glBindVertexArray(m_vao));
		glCheck(glDeleteBuffers(1, &m_vertexVBO));
	}
	if (m_vao) {
		glCheck(glDeleteVertexArrays(1, &m_vao));
	}
	glCheck(glBindVertexArray(0));

	ME::G3dGlobal::setActive(false);
}

void Game::Shadow::Draw(ME::Camera* camera, ME::Shader* shader) {
	glCheck(glBindVertexArray(m_vao));
	glCheck(glDrawArrays(GL_QUADS, 0, 4));
	glCheck(glBindVertexArray(0));
}

void Game::Shadow::DrawInstance(int count) {
	glCheck(glBindVertexArray(m_vao));
	glCheck(glDrawArraysInstanced(GL_QUADS, 0, 4, count));
	glCheck(glBindVertexArray(0));
}

void Game::Shadow::SetColor(float r, float g, float b, float a) {

}

void Game::Shadow::UpdateShader(ME::Shader* shader, ME::Camera* camera) {}
