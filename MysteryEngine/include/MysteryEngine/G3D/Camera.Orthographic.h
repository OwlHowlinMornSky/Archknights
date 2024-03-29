﻿/*
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
#pragma once

#include <MysteryEngine/Core/MEAPI.h>
#include <MysteryEngine/G3D/Camera.h>

namespace ME {

class ME_API OrthographicCamera final :
	public Camera {
public:
	OrthographicCamera() = default;
	virtual ~OrthographicCamera() override = default;

public:
	void setDim(float x, float y) {
		m_dimX = x;
		m_dimY = y;
		m_matP_needUpdate = true;
	}

	float getDimX() const {
		return m_dimX;
	}

	float getDimY() const {
		return m_dimY;
	}

protected:
	virtual void updateMatP() override;

public:
	float m_dimX; // 画面宽度（不是半宽）
	float m_dimY; // 画面高度（不是半高）
};

} // namespace g3d
