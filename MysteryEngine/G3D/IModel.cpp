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

//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace ME {

IModel::IModel() :
	m_waitingForQuit(false),
	m_matM() {}

IModel::~IModel() {
	clear();
}

bool IModel::setup() {
	return true;
}

void IModel::clear() {}

void IModel::update(float dt) {}

void IModel::setColor(float r, float g, float b, float a) {}

void IModel::setOutlineEnabled(bool enabled) {}

void IModel::setWaitingForQuit() {
	m_waitingForQuit = true;
}

bool IModel::isWaitingForQuit() const {
	return m_waitingForQuit;
}

void IModel::setParameter(int code, float val) {}

void IModel::computeMatrixDefault() {
	glm::mat4 matrix_pos = glm::translate(glm::vec3(m_position.x, m_position.y, m_position.z));
	glm::mat4 matrix_scale = glm::scale(glm::vec3(m_scale.x, m_scale.y, m_scale.z));

	glm::mat4 matrix_rotY = glm::rotate(glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 matrix_rotX = glm::rotate(glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 matrix_rotZ = glm::rotate(glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 matrix_rotation = matrix_rotZ * matrix_rotX * matrix_rotY;

	m_matM = matrix_pos * matrix_rotation * matrix_scale;
}

} // namespace ME
