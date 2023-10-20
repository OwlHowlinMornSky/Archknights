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


namespace g3d {

class Camera :
	public ICamera {
public:
	Camera() = default;
	virtual ~Camera() override = default;

public:
	void setZFar(float z) {
		if (z > m_zNear && z <= 65536.0f) {
			m_zFar = z;
			m_matP_needUpdate = true;
		}
		return;
	}

	void setZNear(float z) {
		m_zNear = z;
		m_matP_needUpdate = true;
		return;
	}

protected:
	virtual void ensureMatVUpdated() override;

protected:
	bool m_matP_needUpdate;
};

} // namespace g3d
