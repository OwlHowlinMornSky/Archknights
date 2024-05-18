
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

}

namespace Game {

void ActorShader::setup() {
	clear();
	loadFromMemory(vertex_projection, ME::ShaderType::Vertex);
	loadFromMemory(fragment_spine, ME::ShaderType::Fragment);
	glCheck(glBindAttribLocation(m_program, static_cast<GLuint>(Game::ActorVertexAttribute::Position), "position"));
	glCheck(glBindAttribLocation(m_program, static_cast<GLuint>(Game::ActorVertexAttribute::TexCoord), "texCoord"));
	glCheck(glBindAttribLocation(m_program, static_cast<GLuint>(Game::ActorVertexAttribute::Color), "color"));
	linkShader();
	Bind(this);
	m_matPVM = getUniformLocation("pvm");
	m_matM = getUniformLocation("model");
	m_campos = getUniformLocation("camPosition");
	m_offset = getUniformLocation("offset");
	m_cover = getUniformLocation("enableCoverColor");
	m_coverClr = getUniformLocation("coverColor");

	updateUniform1i(m_cover, 0);
	updateUniform2f(m_offset, 0.0f, 0.0f);
	updateUniform4f(m_coverClr, 1.0f, 1.0f, 1.0f, 1.0f);

	updateUniform1iName("texture", 0);
	Bind(nullptr);
}

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
