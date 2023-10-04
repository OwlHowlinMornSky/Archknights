#include "ShaderDefault.h"

//#include <GL/glew.h>

//#include "glCheck.h"
//#include "IVertex.h"

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

}
