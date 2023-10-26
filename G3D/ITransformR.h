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
#pragma once

#include <glm/vec3.hpp>

namespace g3d {

/**
 * @brief 接口 Transform 旋转：用于可旋转的可变换物体。
*/
class ITransformR {
protected:
	ITransformR() :
		m_rotation(),
		m_rotationChanged(false) {}
	ITransformR(const ITransformR&) = default;
	ITransformR(ITransformR&&) = default;
	~ITransformR() = default;

public:
	void setRotation(const glm::vec3& r) {
		m_rotation = r;
		m_rotationChanged = true;
	}
	void setRotation(glm::vec3&& r) {
		m_rotation = r;
		m_rotationChanged = true;
	}
	void setRotation(float rx, float ry, float rz) {
		m_rotation = { rx, ry, rz };
		m_rotationChanged = true;
	}

	void rotate(float drx, float dry, float drz) {
		m_rotation.x += drx;
		m_rotation.y += dry;
		m_rotation.z += drz;
		m_rotationChanged = true;
	}

	void normalizeRotation() {
		while (m_rotation.x < 0.0f) m_rotation.x += 360.0f;
		while (m_rotation.x >= 360.0f) m_rotation.x -= 360.0f;
		while (m_rotation.y < 0.0f) m_rotation.y += 360.0f;
		while (m_rotation.y >= 360.0f) m_rotation.y -= 360.0f;
		while (m_rotation.z < 0.0f) m_rotation.z += 360.0f;
		while (m_rotation.z >= 360.0f) m_rotation.z -= 360.0f;
	}

protected:
	mutable bool m_rotationChanged;
	mutable glm::vec3 m_rotation;
};

} // namespace g3d
