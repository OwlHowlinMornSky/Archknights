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
#include <MysteryEngine/G3D/Camera.Oblique.h>

#include <glm/gtx/transform.hpp>

namespace ME {

ME::ObliqueCamera::ObliqueCamera() :
	m_dimX(8.0f),
	m_dimY(6.0f),
	m_sheerX(0.70710678118654752440f),
	m_sheerY(0.70710678118654752440f) {}

void ObliqueCamera::setDim(float x, float y) {
	m_dimX = x;
	m_dimY = y;
	m_matP_needUpdate = true;
}

float ObliqueCamera::getDimX() const {
	return m_dimX;
}

float ObliqueCamera::getDimY() const {
	return m_dimY;
}

void ObliqueCamera::setSheer(float a, float b) {
	m_sheerX = a;
	m_sheerY = b;
	m_matP_needUpdate = true;
}

float ObliqueCamera::getSheerX() const {
	return m_sheerX;
}

float ObliqueCamera::getSheerY() const {
	return m_sheerY;
}

Camera::Type ObliqueCamera::getType() const {
	return Camera::Type::Oblique;
}

void ObliqueCamera::updateMatP() {
	// 计算半宽和半高
	float x = m_dimX / 2.0f;
	float y = m_dimY / 2.0f;
	// 计算矩阵
	m_matP = glm::identity<glm::mat4>();
	m_matP[2][0] = -m_sheerX;
	m_matP[2][1] = -m_sheerY;
	m_matP = glm::ortho(-x, x, -y, y, m_zNear, m_zFar) * m_matP;
	// 标记
	m_matP_needUpdate = false;
	m_matPVChanged = true;
	return;
}

} // namespace ME
