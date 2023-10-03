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

protected:
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
