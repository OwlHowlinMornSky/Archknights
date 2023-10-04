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

#include "NonCopyable.h"

#include <string>
#include <SFML/OpenGL.hpp>

namespace g3d {

/**
 * @brief 着色器类型。
*/
enum class ShaderType : size_t {
	Vertex = 0, // 顶点。
	Fragment,   // 片元。
	COUNT       // [计数]
};

/**
 * @brief 接口 Shader。
*/
class IShader :
	public NonCopyable {
public:
	IShader() :
		m_program(0) {}
	virtual ~IShader() = default;

public:
	// 绑定着色器。
	static void Bind(IShader* shader);

protected:
	GLuint m_program; // 着色器程序（OpenGL）。
};


}
