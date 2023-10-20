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
 * @brief ½Ó¿Ú Camera¡£
*/
class ICamera :
	public ITransformT,
	public ITransformR {
public:
	ICamera() :
		m_matPVChanged(false),
		m_zNear(0.5f),
		m_zFar(128.0f),
		m_matP(),
		m_matV() {}
	virtual ~ICamera() = default;

public:
	float getZFar() const {
		return m_zFar;
	}

	float getZNear() const {
		return m_zNear;
	}

	const glm::mat4& getMatP() {
		ensureMatPUpdated();
		return m_matP;
	}

	const glm::mat4& getMatV() {
		ensureMatVUpdated();
		return m_matV;
	}

	const glm::mat4& getMatPV() {
		ensureMatPVUpdated();
		return m_matPV;
	}

protected:
	void ensureMatPVUpdated() {
		ensureMatPUpdated();
		ensureMatVUpdated();
		if (m_matPVChanged) {
			m_matPV = m_matP * m_matV;
			m_matPVChanged = false;
		}
		return;
	}
	virtual void ensureMatVUpdated() = 0;
	virtual void ensureMatPUpdated() = 0;

protected:
	bool m_matPVChanged;
	float m_zNear;
	float m_zFar;
	glm::mat4 m_matP;
	glm::mat4 m_matV;
	glm::mat4 m_matPV;
};

} // namespace g3d
