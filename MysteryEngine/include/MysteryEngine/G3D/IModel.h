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
#include <MysteryEngine/Core/NonCopyable.h>
#include <MysteryEngine/Core/GLM.h>

#include <MysteryEngine/G3D/ITransformT.h>
#include <MysteryEngine/G3D/ITransformR.h>
#include <MysteryEngine/G3D/ITransformS.h>
#include <MysteryEngine/G3D/Camera.h>
#include <MysteryEngine/G3D/Shader.h>

namespace ME {

/**
 * @brief 可绘制的对象。
*/
class ME_API IModel :
	public NonCopyable,
	public ITransformR,
	public ITransformS,
	public ITransformT {
public:
	IModel();
	virtual ~IModel();

public:
	virtual bool Setup();
	virtual void Clear();

	virtual void Update(float dt);
	virtual void Draw(ME::Camera* camera, ME::Shader* shader) = 0;

	virtual void SetOutline(bool enabled);
	virtual void SetColor(float r, float g, float b, float a);

	void SetWaitingForQuit();
	bool IsWaitingForQuit() const;

protected:
	void ComputeMatrix();

protected:
	bool m_waitingForQuit;
	glm::mat4 m_matM;
}; // class IModel

} // namespace ME
