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
#include <MysteryEngine/G3D/Camera.Orthographic.h>

#include <glm/gtx/transform.hpp>

namespace ME {

void ME::OrthographicCamera::updateMatP() {
	// 计算半宽和半高
	float x = m_dimX / 2.0f;
	float y = m_dimY / 2.0f;
	// 计算矩阵
	m_matP = glm::ortho(-x, x, -y, y, m_zNear, m_zFar);
	// 标记
	m_matP_needUpdate = false;
	m_matPVChanged = true;
	return;
}

} // namespace g3d
