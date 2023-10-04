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

/**
 * @brief 接口 Transform 平移：用以可平移的可变换物体。
*/
class ITransformT {
protected:
	ITransformT() = default;
	ITransformT(const ITransformT&) = default;
	ITransformT(ITransformT&&) = default;
	~ITransformT() = default;

public:
	void setPosition(const glm::vec3& p) {
		m_position = p;
		m_positionChanged = true;
	}
	void setPosition(glm::vec3&& p) {
		m_position = p;
		m_positionChanged = true;
	}
	void setPosition(float px, float py, float pz) {
		m_position = { px, py, pz };
		m_positionChanged = true;
	}

	void setPosX(float px) {
		m_position.x = px;
		m_positionChanged = true;
	}
	void setPosY(float py) {
		m_position.y = py;
		m_positionChanged = true;
	}
	void setPosZ(float pz) {
		m_position.z = pz;
		m_positionChanged = true;
	}

	void setPosXY(float px, float py) {
		m_position.x = px;
		m_position.y = py;
		m_positionChanged = true;
	}
	void setPosXZ(float px, float pz) {
		m_position.x = px;
		m_position.z = pz;
		m_positionChanged = true;
	}
	void setPosYZ(float py, float pz) {
		m_position.y = py;
		m_position.z = pz;
		m_positionChanged = true;
	}

	void translate(float dpx, float dpy, float dpz) {
		m_position.x += dpx;
		m_position.y += dpy;
		m_position.z += dpz;
		m_positionChanged = true;
	}

protected:
	mutable bool m_positionChanged;
	mutable glm::vec3 m_position;
};

} // namespace g3d
