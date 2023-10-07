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
	/**
	 * @brief 绑定 或 取消绑定 着色器。
	 * @brief "绑定"就是指 在 OpenGL 里 启用(使用) 某个着色器。
	 * @brief [注意] 这个方法是在 "Shader.cpp" 里实现的。
	 * @param shader: 要绑定的 着色器。为 nullptr 则是 取消先前的绑定。
	*/
	static void Bind(IShader* shader);

protected:
	GLuint m_program; // 着色器程序（OpenGL）。
};


} // namespace g3d
