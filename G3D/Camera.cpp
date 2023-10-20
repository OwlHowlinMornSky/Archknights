/*
*    Archknights
*
*    Copyright (C) 2023  Tyler Parret True
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
#include "Camera.h"

#include <glm/gtx/transform.hpp>

namespace g3d {

void Camera::ensureMatVUpdated() {
	if (m_positionChanged || m_rotationChanged) {
		if (m_rotation.x < 0.0f)
			m_rotation.x = 0.0f;
		else if (m_rotation.x > 90.0f)
			m_rotation.x = 90.0f;
		m_matV = glm::rotate(glm::radians(-m_rotation.y), glm::vec3(0.0f, 0.0f, 1.0f));
		m_matV *= glm::rotate(glm::radians(-m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		m_matV *= glm::rotate(glm::radians(-m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		m_matV *= glm::translate(-m_position);
		m_positionChanged = false;
		m_rotationChanged = false;
		m_matPVChanged = true;
	}
	return;
}

} // namespace g3d
