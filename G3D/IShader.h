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
 * @brief ��ɫ�����͡�
*/
enum class ShaderType : size_t {
	Vertex = 0, // ���㡣
	Fragment,   // ƬԪ��
	COUNT       // [����]
};

/**
 * @brief �ӿ� Shader��
*/
class IShader :
	public NonCopyable {
public:
	IShader() :
		m_program(0) {}
	virtual ~IShader() = default;

public:
	/**
	 * @brief �� �� ȡ���� ��ɫ����
	 * @brief "��"����ָ �� OpenGL �� ����(ʹ��) ĳ����ɫ����
	 * @brief [ע��] ����������� "Shader.cpp" ��ʵ�ֵġ�
	 * @param shader: Ҫ�󶨵� ��ɫ����Ϊ nullptr ���� ȡ����ǰ�İ󶨡�
	*/
	static void Bind(IShader* shader);

protected:
	GLuint m_program; // ��ɫ������OpenGL����
};


} // namespace g3d
