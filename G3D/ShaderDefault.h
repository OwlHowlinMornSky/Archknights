#pragma once

#include "Shader.h"

namespace g3d {

class ShaderDefault final:
	public Shader
{
public:
	virtual void setup() override;

public:
	void updatePVM(GLfloat* pvm) const;

protected:
	GLint m_uniform;
};

}
