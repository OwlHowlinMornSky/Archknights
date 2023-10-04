/*
*                    GNU AFFERO GENERAL PUBLIC LICENSE
*                       Version 3, 19 November 2007
*
*    Copyright (c) 2023  Tyler Parret True
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
*     Tyler Parret True (OwlHowlinMornSky) <mysteryworldgod@outlook.com>
*/
#pragma once

#include <glm/vec3.hpp>

namespace g3d {

class ITransformR {
protected:
	ITransformR() = default;
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

protected:
	mutable bool m_rotationChanged;
	mutable glm::vec3 m_rotation;
};

} // namespace g3d