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
#include <MysteryEngine/G3D/Camera.Perspective.h>

#include <glm/gtx/transform.hpp>

namespace ME {

void PerspectiveCamera::setFOV(float degree) {
	if (degree < 1.0f) degree = 1.0f;
	else if (degree > 179.0f) degree = 179.0f;
	m_fov = degree;
	m_matP_needUpdate = true;
	return;
}

void PerspectiveCamera::setAspectRatio(float ratio) {
	m_aspectRatio = ratio;
	m_matP_needUpdate = true;
	return;
}

float PerspectiveCamera::getFOV() const {
	return m_fov;
}

float PerspectiveCamera::getAspectRatio() const {
	return m_aspectRatio;
}

void ME::PerspectiveCamera::updateMatP() {
	// 计算矩阵
	m_matP = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_zNear, m_zFar);
	// 标记
	m_matP_needUpdate = false;
	m_matPVChanged = true;
	return;
}

} // namespace g3d
