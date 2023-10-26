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

#include "IShader.h"

namespace g3d {

class Shader :
	public IShader {
public:
	Shader();
	virtual ~Shader() override;

public:
	virtual void setup() = 0;

public:
	void clear();
	void loadFromMemory(std::string_view shader, ShaderType type);
	void linkShader();
	GLint getUniformLocation(const char* name) const;
	void updateUniform1iName(const char* name, GLint val) const;
	void updateUniform2fName(const char* name, GLfloat val0, GLfloat val1) const;
	void updateUniform3fName(const char* name, GLfloat val0, GLfloat val1, GLfloat val2) const;
	void updateUniform4fName(const char* name, GLfloat val0, GLfloat val1, GLfloat val2, GLfloat val3) const;
	void updateUniformMat4fvName(const char* name, GLfloat* pvm) const;
	void updateUniformMat4fv(GLint pos, GLfloat* pvm) const;

	static GLuint buildShader(std::string_view l_src, unsigned int l_type);

protected:
	GLuint m_shader[static_cast<size_t>(ShaderType::COUNT)];
};

} // namespace g3d
