
#include <GL/glew.h>
#include "ActorGroup.h"
#include <MysteryEngine/G3D/Shader.h>
#include <MysteryEngine/G3D/glCheck.h>

namespace {

const std::string fragment_spine =
"#version 330\n"\
"uniform sampler2D texture;"\
"uniform bool enableCoverColor;"\
"varying vec4 tint;"\
"varying vec2 uv;"\
"void main() {"\
" vec4 texColor = texture2D(texture, uv);"\
" gl_FragColor = (enableCoverColor ? vec4(texColor.w) : texColor) * tint;"\
"}";

/// <summary>
/// 将深度变换到竖直面及水平面用于欺骗深度测试的顶点着色器。
/// </summary>
const std::string vertex_projection =
#ifndef _DEBUG
"#version 330\n"\
"attribute vec2 position; "\
"attribute vec4 color; "\
"attribute vec2 texCoord;"\
"uniform mat4 pvm;"\
"uniform vec3 camPosition;"\
"uniform vec2 offset;"\
"uniform bool enableCoverColor;"\
"uniform vec4 coverColor;"\
"varying vec4 tint;"\
"varying vec2 uv;"\
"void main() {"\
" tint = enableCoverColor ? color.w * coverColor : color * coverColor;"\
" uv = texCoord;"\
" vec2 tmpPos = position / 128.0;"\
" vec3 truePos = vec3(tmpPos.x + offset.x, (tmpPos.y + offset.y) * 0.86602540378443864676372317075294, (tmpPos.y + offset.y) * 0.5);"\
" gl_Position = pvm * vec4(truePos, 1.0);"\
" vec3 vectorPositionToCamera = camPosition - truePos;"\
" vec4 finalProjection = pvm * vec4(truePos - vectorPositionToCamera * mix(truePos.z / vectorPositionToCamera.z, truePos.y / vectorPositionToCamera.y, step(0.0, truePos.z)), 1.0);"\
" gl_Position.z = gl_Position.w * finalProjection.z / finalProjection.w;"\
"}";
#else
"#version 330\n"\
"attribute vec2 position; "\
"attribute vec4 color; "\
"attribute vec2 texCoord;"\
"uniform mat4 pvm;"\
"uniform mat4 model;"\
"uniform vec3 camPosition;"\
"uniform vec2 offset;"\
"uniform bool enableCoverColor;"\
"uniform vec4 coverColor;"\
"varying vec4 tint;"\
"varying vec2 uv;"\
"void main() {"\
" tint = enableCoverColor ? color.w * coverColor : color * coverColor;"\
" uv = texCoord;"\
" vec2 tmpPos = position / 128.0;"\
" vec3 truePos = vec3(tmpPos.x + offset.x, (tmpPos.y + offset.y) * 0.86602540378443864676372317075294, (tmpPos.y + offset.y) * 0.5);"\
" gl_Position = pvm * vec4(truePos, 1.0);"\
" vec3 vectorPositionToCamera = camPosition - truePos;"\
" vec4 projPos = vec4(truePos - vectorPositionToCamera * mix(truePos.z / vectorPositionToCamera.z, truePos.y / vectorPositionToCamera.y, step(0.0, truePos.z)), 1.0);"\
" vec4 finalProjection = pvm * projPos;"\
" gl_Position.z = mix(gl_Position.w * finalProjection.z / finalProjection.w, gl_Position.z, step(0.0, min(truePos.z, (camPosition.y + 0.1))));"\
"}";
#endif

class ActorShader final :
	public ME::Shader {
protected:
	int m_uniforms[Game::ActorShaderUniformId::COUNT];

public:
	virtual void setup() override {
		clear();
		loadFromMemory(vertex_projection, ME::ShaderType::Vertex);
		loadFromMemory(fragment_spine, ME::ShaderType::Fragment);
		glCheck(glBindAttribLocation(m_program, static_cast<GLuint>(Game::ActorVertexAttribute::Position), "position"));
		glCheck(glBindAttribLocation(m_program, static_cast<GLuint>(Game::ActorVertexAttribute::TexCoord), "texCoord"));
		glCheck(glBindAttribLocation(m_program, static_cast<GLuint>(Game::ActorVertexAttribute::Color), "color"));
		linkShader();
		Bind(this);

		m_uniforms[Game::ActorShaderUniformId::Mat4_PVM] = getUniformLocation("pvm");
		m_uniforms[Game::ActorShaderUniformId::Mat4_M] = getUniformLocation("model");
		m_uniforms[Game::ActorShaderUniformId::Vec3_CamPos] = getUniformLocation("camPosition");
		m_uniforms[Game::ActorShaderUniformId::Vec2_Offset] = getUniformLocation("offset");
		m_uniforms[Game::ActorShaderUniformId::Int1_CvrClr] = getUniformLocation("enableCoverColor");
		m_uniforms[Game::ActorShaderUniformId::Vec4_CvrClr] = getUniformLocation("coverColor");

		updateUniform1i(m_uniforms[Game::ActorShaderUniformId::Int1_CvrClr], 0);
		updateUniform2f(m_uniforms[Game::ActorShaderUniformId::Vec2_Offset], 0.0f, 0.0f);
		updateUniform4f(m_uniforms[Game::ActorShaderUniformId::Vec4_CvrClr], 1.0f, 1.0f, 1.0f, 1.0f);

		updateUniform1iName("texture", 0);
		Bind(nullptr);
	}

	virtual void UpdateUniform(int id, GLfloat* data) const override {
		switch (id) {
		case Game::ActorShaderUniformId::Mat4_PVM:
			updateUniformMat4fv(m_uniforms[Game::ActorShaderUniformId::Mat4_PVM], data);
			break;
		case Game::ActorShaderUniformId::Mat4_M:
			updateUniformMat4fv(m_uniforms[Game::ActorShaderUniformId::Mat4_M], data);
			break;
		case Game::ActorShaderUniformId::Vec3_CamPos:
			updateUniform3f(m_uniforms[Game::ActorShaderUniformId::Vec3_CamPos], data[0], data[1], data[2]);
			break;
		case Game::ActorShaderUniformId::Vec2_Offset:
			updateUniform2f(m_uniforms[Game::ActorShaderUniformId::Vec2_Offset], data[0], data[1]);
			break;
		case Game::ActorShaderUniformId::Vec4_CvrClr:
			updateUniform4f(m_uniforms[Game::ActorShaderUniformId::Vec4_CvrClr], data[0], data[1], data[2], data[3]);
			break;
		}
	}

	virtual void UpdateUniform1(int id, GLfloat val0) const override {
	}
	virtual void UpdateUniform2(int id, GLfloat val0, GLfloat val1) const override {
		updateUniform2f(m_uniforms[id], val0, val1);
	}
	virtual void UpdateUniform3(int id, GLfloat val0, GLfloat val1, GLfloat val2) const override {
		updateUniform3f(m_uniforms[id], val0, val1, val2);
	}
	virtual void UpdateUniform4(int id, GLfloat val0, GLfloat val1, GLfloat val2, GLfloat val3) const override {
		updateUniform4f(m_uniforms[id], val0, val1, val2, val3);
	}

	virtual void UpdateUniformI1(int id, GLint val) const override {
		updateUniform1i(m_uniforms[id], val);
	}
};

}

namespace Game {

ActorGroup::ActorGroup() {
	m_shader = std::make_unique<ActorShader>();
	m_shader->setup();
}

void ActorGroup::AddActor(std::shared_ptr<IModel> actor) {
	m_actors.push_back(actor);
}

void ActorGroup::Update(float dt) {
	std::list<std::shared_ptr<IModel>>::iterator i, n;
	i = m_actors.begin();
	n = m_actors.end();
	for (; i != n;) {
		if ((*i)->IsWaitingForQuit()) {
			i = m_actors.erase(i);
		}
		else {
			(*i)->Update(dt);
			i++;
		}
	}
	m_actors.sort(
		[](std::shared_ptr<IModel>& l, std::shared_ptr<IModel>& r) {
			return l->getPos().y > r->getPos().y;
		}
	);
}

void ActorGroup::Draw(ME::Camera& camera, ME::Shader& shader) {
	//glCheck(glEnable(GL_BLEND));
	glCheck(glDisable(GL_CULL_FACE));
	glCheck(glDepthMask(GL_FALSE));

	glCheck(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));

	ME::Shader::Bind(m_shader.get());
	for (std::shared_ptr<IModel>& i : m_actors) {
		i->Draw(camera, *m_shader);
	}
	ME::Shader::Bind(&shader);

	//glCheck(glEnable(GL_BLEND));
	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glDepthMask(GL_TRUE));

	//glCheck(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));
}

}

std::shared_ptr<Game::IActorGroup> Game::IActorGroup::Create() {
	return std::make_shared<Game::ActorGroup>();
}
