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

#include "Camera.h"

namespace g3d {

class CameraPerspective final :
	public Camera {
public:
	CameraPerspective() = default;
	virtual ~CameraPerspective() override = default;

public:
	void setFOV(float degree) {
		if (degree < 1.0f)
			degree = 1.0f;
		else if (degree > 179.0f)
			degree = 179.0f;
		m_fov = degree;
		m_matP_needUpdate = true;
		return;
	}
	void setAspectRatio(float ratio) {
		m_aspect = ratio;
		m_matP_needUpdate = true;
		return;
	}

	float getFOV() const {
		return m_fov;
	}
	float getAspectRatio() const {
		return m_aspect;
	}

protected:
	virtual void ensureMatPUpdated() const override;

protected:
	float m_fov;
	float m_aspect;
};

}
