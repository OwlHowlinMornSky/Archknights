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
#include <MysteryEngine/G3D/Camera.h>

#include <glm/gtx/transform.hpp>

namespace ME {

Camera::Camera() :
	m_matPVChanged(false),
	m_matP_needUpdate(false),
	m_zNear(0.5f),
	m_zFar(128.0f),
	m_matP(),
	m_matV(),
	m_matPV() {}

void Camera::setZFar(float z) {
	if (z > m_zNear && z <= 65536.0f) {
		m_zFar = z;
		m_matP_needUpdate = true;
	}
	return;
}

float Camera::getZFar() const {
	return m_zFar;
}

void Camera::setZNear(float z) {
	m_zNear = z;
	m_matP_needUpdate = true;
	return;
}

float Camera::getZNear() const {
	return m_zNear;
}

glm::mat4& Camera::getMatP() {
	if (m_matP_needUpdate) {
		updateMatP();
	}
	return m_matP;
}

glm::mat4& Camera::getMatV() {
	if (m_positionChanged || m_rotationChanged) {
		updateMatV();
	}
	return m_matV;
}

glm::mat4& Camera::getMatPV() {
	ensureMatPVUpdated();
	return m_matPV;
}

void Camera::ensureMatPVUpdated() {
	if (m_matP_needUpdate) {
		updateMatP();
	}
	if (m_positionChanged || m_rotationChanged) {
		updateMatV();
	}
	if (m_matPVChanged) {
		m_matPV = m_matP * m_matV;
		m_matPVChanged = false;
	}
	return;
}

void Camera::updateMatV() {
	// 限制俯仰角
	if (m_rotation.x < 0.0f)
		m_rotation.x = 0.0f;
	else if (m_rotation.x > 180.0f)
		m_rotation.x = 180.0f;
	// 计算旋转
	m_matV = glm::rotate(glm::radians(-m_rotation.y), glm::vec3(0.0f, 0.0f, 1.0f));
	m_matV *= glm::rotate(glm::radians(-m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_matV *= glm::rotate(glm::radians(-m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	// 计算位移
	m_matV *= glm::translate(-m_position);
	// 标记
	m_positionChanged = false;
	m_rotationChanged = false;
	m_matPVChanged = true;
	return;
}

} // namespace ME
