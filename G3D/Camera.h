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

#include "ICamera.h"

#include <glm/mat4x4.hpp>

namespace g3d {

class Camera :
	public ICamera {
public:
	Camera() = default;
	virtual ~Camera() override = default;

public:
	void setFF(float distance) {
		m_ff = distance;
		m_matP_needUpdate = true;
		return;
	}

	void setFN(float distance) {
		m_fn = distance;
		m_matP_needUpdate = true;
		return;
	}

	const glm::mat4& getMatP() {
		ensureMatPUpdated();
		return m_matP;
	}

	const glm::mat4& getMatV() {
		ensureMatVUpdated();
		return m_matV;
	}

protected:
	void ensureMatVUpdated() const;
	virtual void ensureMatPUpdated() const = 0;

protected:
	mutable bool m_matP_needUpdate;
	mutable glm::mat4 m_matP;
	mutable glm::mat4 m_matV;
};

} // namespace g3d
