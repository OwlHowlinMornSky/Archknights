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

#include "ITransformT.h"
#include "ITransformR.h"
#include <glm/mat4x4.hpp>

namespace g3d {

/**
 * @brief Camera¡£
*/
class Camera :
	public ITransformR,
	public ITransformT {
public:
	Camera() :
		m_matPVChanged(false),
		m_matP_needUpdate(false),
		m_zNear(0.5f),
		m_zFar(128.0f),
		m_matP(),
		m_matV(),
		m_matPV() {}
	virtual ~Camera() = default;

public:
	void setZFar(float z) {
		if (z > m_zNear && z <= 65536.0f) {
			m_zFar = z;
			m_matP_needUpdate = true;
		}
		return;
	}
	float getZFar() const {
		return m_zFar;
	}
	void setZNear(float z) {
		m_zNear = z;
		m_matP_needUpdate = true;
		return;
	}
	float getZNear() const {
		return m_zNear;
	}

	const glm::mat4& getMatP() {
		if (m_matP_needUpdate) {
			updateMatP();
		}
		return m_matP;
	}
	const glm::mat4& getMatV() {
		if (m_positionChanged || m_rotationChanged) {
			updateMatV();
		}
		return m_matV;
	}
	const glm::mat4& getMatPV() {
		ensureMatPVUpdated();
		return m_matPV;
	}

protected:
	void ensureMatPVUpdated() {
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
	void updateMatV();
	virtual void updateMatP() = 0;

protected:
	bool m_matPVChanged;
	bool m_matP_needUpdate;
	float m_zNear;
	float m_zFar;
	glm::mat4 m_matP;
	glm::mat4 m_matV;
	glm::mat4 m_matPV;
};

} // namespace g3d
