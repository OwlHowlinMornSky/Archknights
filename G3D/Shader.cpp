
#include <GL/glew.h>

#include "Shader.h"

#include "glCheck.h"
//#include "IVertex.h"

namespace {

void checkError(GLuint l_shader, GLuint l_flag, bool l_program, std::string_view l_errorMsg) {
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (l_program) {
		glCheck(glGetProgramiv(l_shader, l_flag, &success));
	}
	else {
		glCheck(glGetShaderiv(l_shader, l_flag, &success));
	}

	if (success)
		return;

	if (l_program) {
		glCheck(glGetProgramInfoLog(l_shader, sizeof(error), nullptr, error));
	}
	else {
		glCheck(glGetShaderInfoLog(l_shader, sizeof(error), nullptr, error));
	}

	fprintf_s(stderr, "%s\n", l_errorMsg.data());
	return;
}

} // namespace

namespace g3d {

void IShader::BindShader(IShader* shader) {
	if (shader) {
		if (!shader->m_program) {
#ifdef _DEBUG
			printf_s("Error Using Empty Shader!\n\tFile: %S\n\tLine: %d\n\n", __FILEW__, __LINE__);
#endif // _DEBUG
		}
		glCheck(glUseProgram(shader->m_program));
	}
	else {
		glCheck(glUseProgram(0));
	}
}

Shader::Shader() {
	memset(m_shader, 0, sizeof(m_shader));
	return;
}

Shader::~Shader() {
	clear();
	return;
}

void Shader::clear() {
	if (m_program) {
		for (unsigned int i = 0; i < static_cast<unsigned int>(ShaderType::COUNT); i++) {
			if (m_shader[i]) {
				glCheck(glDetachShader(m_program, m_shader[i]));
				glCheck(glDeleteShader(m_shader[i]));
				m_shader[i] = 0;
			}
		}
		glCheck(glDeleteProgram(m_program));
		m_program = 0;
	}
	return;
}

void Shader::loadFromMemory(std::string_view shader, ShaderType type) {
	if (shader.empty())
		return;
	size_t f = static_cast<size_t>(type);

	switch (type) {
	case ShaderType::Vertex:
		m_shader[f] = buildShader(shader, GL_VERTEX_SHADER);
		break;
	case ShaderType::Fragment:
		m_shader[f] = buildShader(shader, GL_FRAGMENT_SHADER);
		break;
	default:
#ifdef _DEBUG
		printf_s("Invalid Shader Type.\n\tFile: %S\n\tLine: %d\n\n", __FILEW__, __LINE__);
#endif
		return;
	}
	if (!m_program) {
		glCheck(m_program = glCreateProgram());
	}
	glCheck(glAttachShader(m_program, m_shader[f]));
	return;
}

void Shader::linkShader() {
	glCheck(glLinkProgram(m_program));
	checkError(m_program, GL_LINK_STATUS, true, "Shader link error:");
	glCheck(glValidateProgram(m_program));
	checkError(m_program, GL_VALIDATE_STATUS, true, "Invalid shader:");
}

GLint Shader::getUniformLocation(const char* name) const {
	return glGetUniformLocation(m_program, name);
}

void Shader::updateUniform1iName(const char* name, GLint val) const {
	glCheck(glUniform1i(glGetUniformLocation(m_program, name), val));
}

void Shader::updateUniform2fName(const char* name, GLfloat val0, GLfloat val1) const {
	glCheck(glUniform2f(glGetUniformLocation(m_program, name), val0, val1));
}

void Shader::updateUniform3fName(const char* name, GLfloat val0, GLfloat val1, GLfloat val2) const {
	glCheck(glUniform3f(glGetUniformLocation(m_program, name), val0, val1, val2));
}

void Shader::updateUniform4fName(const char* name, GLfloat val0, GLfloat val1, GLfloat val2, GLfloat val3) const {
	glCheck(glUniform4f(glGetUniformLocation(m_program, name), val0, val1, val2, val3));
}

void Shader::updateUniformMat4fvName(const char* name, GLfloat* pvm) const {
	glCheck(glUniformMatrix4fv(glGetUniformLocation(m_program, name), 1, GL_FALSE, pvm));
}

void Shader::updateUniformMat4fv(GLint pos, GLfloat* pvm) const {
	glCheck(glUniformMatrix4fv(pos, 1, GL_FALSE, pvm));
}

GLuint Shader::buildShader(std::string_view l_src, unsigned int l_type) {
	GLuint shaderID = glCreateShader(l_type);
	if (!shaderID) {
		printf_s("Bad shader type!\n");
		return 0;
	}
	const GLchar* sources[1];
	GLint lengths[1];
	sources[0] = l_src.data();
	lengths[0] = (int)l_src.length();
	glCheck(glShaderSource(shaderID, 1, sources, lengths));
	glCheck(glCompileShader(shaderID));
	checkError(shaderID, GL_COMPILE_STATUS, false, "Shader compile error: ");
	return shaderID;
}

}

