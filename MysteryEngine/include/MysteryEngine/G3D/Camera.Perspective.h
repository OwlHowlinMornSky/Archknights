/*
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

class ME_API PerspectiveCamera final :
	public Camera {
public:
	PerspectiveCamera() = default;
	virtual ~PerspectiveCamera() override = default;

public:
	void setFOV(float degree);
	void setAspectRatio(float ratio);

	float getFOV() const;
	float getAspectRatio() const;

protected:
	virtual void updateMatP() override;

protected:
	float m_fov; // 纵向视场角
	float m_aspectRatio; // 宽高比
};

} // namespace g3d
