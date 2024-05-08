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
#include <MysteryEngine/Core/GLM.h>

#include <MysteryEngine/G3D/ITransformT.h>
#include <MysteryEngine/G3D/ITransformR.h>

namespace ME {

/**
 * @brief Camera 接口，具体实现有 透视相机 和 正交相机。
*/
class ME_API Camera :
	public ITransformR,
	public ITransformT {
public:
	Camera();
	virtual ~Camera() = default;

public:
	void setZFar(float z);
	float getZFar() const;
	void setZNear(float z);
	float getZNear() const;

	glm::mat4& getMatP();
	glm::mat4& getMatV();
	glm::mat4& getMatPV();

protected:
	void ensureMatPVUpdated();
	void updateMatV();
	virtual void updateMatP() = 0;

protected:
	bool m_matPVChanged;
	bool m_matP_needUpdate;
	float m_zNear; // z of near plane
	float m_zFar;  // z of far plane
	glm::mat4 m_matP;
	glm::mat4 m_matV;
	glm::mat4 m_matPV;
}; // class Camera

} // namespace ME
