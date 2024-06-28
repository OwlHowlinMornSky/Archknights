﻿/*
*    Archknights
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

#include <MysteryEngine/G3D/IModel.h>
#include <MysteryEngine/G3D/Vertex.h>
#include "ActorVertex.h"

namespace Game {

class Shadow final :
	public ME::IModel {
	typedef ME::IModel Parent;
public:
	Shadow();
	~Shadow();

public:
	virtual bool Setup() override;
	virtual void Clear() override;

	virtual void Draw(ME::Camera* camera, ME::Shader* shader) override;
	void DrawInstance(int count);
	virtual void SetColor(float r, float g, float b, float a) override;

protected:
	void UpdateShader(ME::Shader* shader, ME::Camera* camera);

protected:
	unsigned int m_vao;
	unsigned int m_vertexVBO;
	ActorVertex m_vertex[4];
};

}