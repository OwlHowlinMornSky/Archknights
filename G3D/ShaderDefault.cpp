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
#include "ShaderDefault.h"

namespace {

const char g_defaultVertexShader[] =
"#version 330\n"\
"layout(location = 0) attribute vec3 position;"\
"layout(location = 1) attribute vec4 color;" \
"layout(location = 2) attribute vec2 texCoord;" \
"uniform mat4 matPVM;" \
"varying vec4 tint;" \
"varying vec2 uv;" \
"void main() {"\
"	gl_Position = matPVM * vec4(position, 1.0);"\
"	tint = color;"\
"	uv = texCoord;"\
"}";

const char g_defaultFragShader[] =
"#version 330\n" \
"uniform sampler2D texture;"\
"varying vec4 tint;"\
"varying vec2 uv;"\
"void main() {"\
"	gl_FragColor = texture2D(texture, uv) * tint;"\
"}";

}

namespace g3d {

void ShaderDefault::setup() {
	clear();
	loadFromMemory(g_defaultVertexShader, ShaderType::Vertex);
	loadFromMemory(g_defaultFragShader, ShaderType::Fragment);
	//glCheck(glBindAttribLocation(m_program, static_cast<GLuint>(VertexAttribute::Position), "position"));
	//glCheck(glBindAttribLocation(m_program, static_cast<GLuint>(VertexAttribute::TexCoord), "texCoord"));
	//glCheck(glBindAttribLocation(m_program, static_cast<GLuint>(VertexAttribute::Color), "color"));
	linkShader();
	Bind(this);
	m_uniform = getUniformLocation("matPVM");
	updateUniform1iName("texture", 0);
	Bind(nullptr);
	return;
}

void ShaderDefault::updatePVM(GLfloat* pvm) const {
	updateUniformMat4fv(m_uniform, pvm);
}

} // namespace g3d
