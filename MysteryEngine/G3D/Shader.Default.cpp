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
#include <GL/glew.h>
#include <MysteryEngine/G3D/Shader.Default.h>
#include <MysteryEngine/G3D/Vertex.h>
#include <MysteryEngine/G3D/glCheck.h>

namespace {

const char g_defaultVertexShader[] =
"#version 330\n"\
"attribute vec3 aVertexPos;"\
"attribute vec2 aVertexTexCoord;" \
"attribute vec4 aVertexColor;" \
"uniform mat4 uMatPVM;" \
"varying vec4 vTint;" \
"varying vec2 vUv;" \
"void main() {"\
"    vTint = aVertexColor;"\
"    vUv = aVertexTexCoord;"\
"    gl_Position = uMatPVM * vec4(aVertexPos, 1.0);"\
"}";

const char g_defaultFragShader[] =
"#version 330\n" \
"uniform sampler2D uTex;"\
"varying vec4 vTint;"\
"varying vec2 vUv;"\
"void main() {"\
"    gl_FragColor = texture2D(uTex, vUv) * vTint;"\
"}";

} // namespace

namespace ME {

DefaultShader::DefaultShader() :
	m_uniform(0) {}

void DefaultShader::setup() {
	clear();
	loadFromMemory(g_defaultVertexShader, ShaderType::Vertex);
	loadFromMemory(g_defaultFragShader, ShaderType::Fragment);
	glCheck(glBindAttribLocation(m_program, static_cast<GLuint>(VertexAttribute::Position), "aVertexPos"));
	glCheck(glBindAttribLocation(m_program, static_cast<GLuint>(VertexAttribute::TexCoord), "aVertexTexCoord"));
	glCheck(glBindAttribLocation(m_program, static_cast<GLuint>(VertexAttribute::Color), "aVertexColor"));
	linkShader();
	Bind(this);
	m_uniform = getUniformLocation("uMatPVM");
	updateUniform1iName("uTex", 0);
	Bind(nullptr);
	return;
}

void DefaultShader::updatePVM(GLfloat* pvm) const {
	updateUniformMat4fv(m_uniform, pvm);
}

} // namespace ME
