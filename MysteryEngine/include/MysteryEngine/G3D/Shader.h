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

#include <string_view>
#include <SFML/OpenGL.hpp>

namespace ME {

/**
 * @brief 着色器类型。
*/
enum class ME_API ShaderType : size_t {
	Vertex = 0, // 顶点。
	Fragment,   // 片元。
	COUNT       // [计数]
}; // enum class ShaderType

class ME_API Shader :
	public NonCopyable {
public:
	Shader();
	virtual ~Shader();

public:
	virtual void setup() = 0;

public:
	/**
	 * @brief 绑定 或 取消绑定 着色器。
	 * @brief "绑定"就是指 在 OpenGL 里 启用(使用) 某个着色器。
	 * @param shader: 要绑定的 着色器。为 nullptr 则是 取消先前的绑定。
	*/
	static void Bind(Shader* shader);

	void clear();
	void loadFromMemory(std::string_view shader, ShaderType type);
	void linkShader();
	GLint getUniformLocation(const char* name) const;
	void updateUniform1i(GLint pos, GLint val) const;
	void updateUniform1iName(const char* name, GLint val) const;
	void updateUniform2f(GLint pos, GLfloat val0, GLfloat val1) const;
	void updateUniform2fName(const char* name, GLfloat val0, GLfloat val1) const;
	void updateUniform3f(GLint pos, GLfloat val0, GLfloat val1, GLfloat val2) const;
	void updateUniform3fName(const char* name, GLfloat val0, GLfloat val1, GLfloat val2) const;
	void updateUniform4f(GLint pos, GLfloat val0, GLfloat val1, GLfloat val2, GLfloat val3) const;
	void updateUniform4fName(const char* name, GLfloat val0, GLfloat val1, GLfloat val2, GLfloat val3) const;
	void updateUniformMat4fv(GLint pos, GLfloat* pvm) const;
	void updateUniformMat4fvName(const char* name, GLfloat* pvm) const;

	virtual void UpdateUniform(int id, GLfloat* data) const;
	virtual void UpdateUniformV(int id, GLsizei count, GLfloat* data) const;

	virtual void UpdateUniform1(int id, GLfloat val0) const;
	virtual void UpdateUniform2(int id, GLfloat val0, GLfloat val1) const;
	virtual void UpdateUniform3(int id, GLfloat val0, GLfloat val1, GLfloat val2) const;
	virtual void UpdateUniform4(int id, GLfloat val0, GLfloat val1, GLfloat val2, GLfloat val3) const;

	virtual void UpdateUniformI1(int id, GLint val) const;

	static GLuint buildShader(std::string_view l_src, unsigned int l_type);

protected:
	GLuint m_program; // 着色器程序。
	GLuint m_shader[static_cast<size_t>(ShaderType::COUNT)];
}; // class Shader

} // namespace ME
