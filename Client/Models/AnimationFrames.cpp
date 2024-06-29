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

#ifndef ARCHKNIGHTS_LIMITED

#include <GL/glew.h>
#include <MysteryEngine/G3D/GlCheck.h>

#include "AnimationFrames.h"

#include <MysteryEngine/G3D/Shader.Default.h>

namespace {

ME::DefaultShader* g_shader;
size_t g_test;

}

namespace Model {

int AnimationFrames::Load(std::string_view file) {
	this->drawCount = (unsigned int)vertexArray.size();

	unsigned int stride = (unsigned int)sizeof(vertexArray[0]);
	unsigned long long offset1 = sizeof(vertexArray[0].position);
	unsigned long long offset2 = offset1 + sizeof(vertexArray[0].texCoord);
	unsigned long long offset3 = offset2 + sizeof(vertexArray[0].color);

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
	glCheck(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)offset1));
	glCheck(glEnableVertexAttribArray(2));
	glCheck(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride, (void*)offset2));
	glCheck(glEnableVertexAttribArray(3));
	glCheck(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset3));

	glCheck(glBindVertexArray(0));
	//return true;
	return 0;
}

void AnimationFrames::Update(float dt) {
	m_test += dt;
	while (m_test >= 1.0f) {
		g_test++;
		m_test--;
	}
	return;
}

void AnimationFrames::Draw(ME::Camera& camera, ME::Shader& shader) {
	ME::Shader::Bind(g_shader);

	if (m_positionChanged || m_rotationChanged || m_scaleChanged) {
		ComputeMatrix();
		m_positionChanged = m_rotationChanged = m_scaleChanged = false;
	}
	glm::mat4 pvm = camera.getMatPV() * m_matM;
	g_shader->updatePVM(&(pvm[0][0]));

	glCheck(glBindVertexArray(this->vao));
	glCheck(glDrawArrays(GL_QUADS, 0, this->drawCount));
	glCheck(glBindVertexArray(0));

	ME::Shader::Bind(nullptr);
}

std::shared_ptr<ME::IModel> FramesPose::CreateAnimation() {
	auto res = std::make_shared<AnimationFrames>();

	return res;
}

bool FramesFactory::CreatePose(
	std::unique_ptr<IAnimationPose>& ptr,
	std::string_view name, unsigned char type
) {
	return false;
}

char FramesFactory::CreatePose2(
	std::unique_ptr<IAnimationPose>& ptr0,
	std::unique_ptr<IAnimationPose>& ptr1,
	std::string_view name
) {
	return 0;
}

}

namespace {

std::unique_ptr<Model::FramesFactory> g_frameFactoryInstance;

}

Model::IAnimationFactory* Model::IAnimationFactory::Instance() {
	if (g_frameFactoryInstance == nullptr) {
		g_frameFactoryInstance = std::make_unique<Model::FramesFactory>();
	}
	return g_frameFactoryInstance.get();
}

void Model::IAnimationFactory::Drop() {
	g_frameFactoryInstance.reset();
}

#endif // !ARCHKNIGHTS_LIMITED
