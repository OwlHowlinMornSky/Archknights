/*
*    Archknights
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
#include "ActorGroup.h"
#include <MysteryEngine/G3D/Shader.h>
#include <MysteryEngine/G3D/glCheck.h>

namespace {

const std::string fragment_spine =
"#version 330\n"\

"uniform sampler2D uTexture;"\
"uniform bool uEnableCvrClr;"\

"varying vec4 vTint;"\
"varying vec2 vUv;"\

"void main() {"\
" vec4 texColor = texture2D(uTexture, vUv);"\
" gl_FragColor = (uEnableCvrClr ? vec4(texColor.w) : texColor) * vTint;"\
"}";

/// <summary>
/// 将深度变换到竖直面及水平面用于欺骗深度测试的顶点着色器。
/// </summary>
const std::string vertex_projection =
#ifndef _DEBUG
"#version 330\n"\

"attribute vec2 aPosition; "\
"attribute vec4 aColor; "\
"attribute vec2 aTexCoord;"\

"uniform mat4 uMatPV;"\
"uniform mat4 uMatM;"\
"uniform vec3 uVecCamPos;"\
"uniform vec2 uVecOffset;"\
"uniform bool uEnableCvrClr;"\
"uniform vec4 uVecCvrClr;"\

"varying vec4 vTint;"\
"varying vec2 vUv;"\

"void main() {"\
" vTint = uEnableCvrClr ? aColor.w * uVecCvrClr : aColor * uVecCvrClr;"\
" vUv = aTexCoord;"\

" vec2 VertexPosInModel = aPosition / 256.0 + uVecOffset;"\
" vec4 VertexPosInGlobal = uMatM * vec4(VertexPosInModel.xy, 0.0, 1.0);"\
" vec4 OrgPosInGlobal = uMatM * vec4(0.0, 0.0, 0.0, 1.0);"\

" gl_Position = uMatPV * VertexPosInGlobal;"\

" vec3 CamPosInOrg = uVecCamPos - OrgPosInGlobal.xyz;"\
" vec3 VertexPosInOrg = VertexPosInGlobal.xyz - OrgPosInGlobal.xyz;"\

" vec3 VecPositionToCamera = CamPosInOrg - VertexPosInOrg;"\

" vec4 FakePosInGlobal = vec4(VertexPosInGlobal.xyz - VecPositionToCamera * mix(VertexPosInOrg.z / VecPositionToCamera.z, VertexPosInOrg.y / VecPositionToCamera.y, step(0.0, aPosition.y)), 1.0);"\
" vec4 FakeProjection = uMatPV * FakePosInGlobal;"\

" gl_Position.z = gl_Position.w * FakeProjection.z / FakeProjection.w;"\
"}";
#else
"#version 330\n"\

"attribute vec2 aPosition; "\
"attribute vec4 aColor; "\
"attribute vec2 aTexCoord;"\

"uniform mat4 uMatPV;"\
"uniform mat4 uMatM;"\
"uniform vec3 uVecCamPos;"\
"uniform vec2 uVecOffset;"\
"uniform bool uEnableCvrClr;"\
"uniform vec4 uVecCvrClr;"\

"varying vec4 vTint;"\
"varying vec2 vUv;"\

"void main() {"\
" vTint = uEnableCvrClr ? aColor.w * uVecCvrClr : aColor * uVecCvrClr;"\
" vUv = aTexCoord;"\

" vec2 VertexPosInModel = aPosition / 256.0 + uVecOffset;"\
" vec4 VertexPosInGlobal = uMatM * vec4(VertexPosInModel.xy, 0.0, 1.0);"\
" vec4 OrgPosInGlobal = uMatM * vec4(0.0, 0.0, 0.0, 1.0);"\

" gl_Position = uMatPV * VertexPosInGlobal;"\

" vec3 CamPosInOrg = uVecCamPos - OrgPosInGlobal.xyz;"\
" vec3 VertexPosInOrg = VertexPosInGlobal.xyz - OrgPosInGlobal.xyz;"\

" vec3 VecPositionToCamera = CamPosInOrg - VertexPosInOrg;"\

" vec4 FakePosInGlobal = vec4(VertexPosInGlobal.xyz - VecPositionToCamera * mix(VertexPosInOrg.z / VecPositionToCamera.z, VertexPosInOrg.y / VecPositionToCamera.y, step(0.0, aPosition.y)), 1.0);"\
" vec4 FakeProjection = uMatPV * FakePosInGlobal;"\

" gl_Position.z = mix(gl_Position.w * FakeProjection.z / FakeProjection.w, gl_Position.z, step(0.0, min(OrgPosInGlobal.z, (uVecCamPos.y + 0.1))));"\
"}";
#endif

class ActorShader final :
	public ME::Shader {
protected:
	int m_uniforms[Game::ActorShaderUniformId::COUNT];

public:
	ActorShader() :
		m_uniforms() {}
	virtual ~ActorShader() = default;

public:
	virtual void setup() override {
		clear();
		loadFromMemory(vertex_projection, ME::ShaderType::Vertex);
		loadFromMemory(fragment_spine, ME::ShaderType::Fragment);
		glCheck(glBindAttribLocation(m_program, static_cast<GLuint>(Game::ActorVertexAttribute::Position), "aPosition"));
		glCheck(glBindAttribLocation(m_program, static_cast<GLuint>(Game::ActorVertexAttribute::TexCoord), "aTexCoord"));
		glCheck(glBindAttribLocation(m_program, static_cast<GLuint>(Game::ActorVertexAttribute::Color), "aColor"));
		linkShader();
		Bind(this);

		m_uniforms[Game::ActorShaderUniformId::Mat4_PV] = getUniformLocation("uMatPV");
		m_uniforms[Game::ActorShaderUniformId::Mat4_M] = getUniformLocation("uMatM");
		m_uniforms[Game::ActorShaderUniformId::Vec3_CamPos] = getUniformLocation("uVecCamPos");
		m_uniforms[Game::ActorShaderUniformId::Vec2_Offset] = getUniformLocation("uVecOffset");
		m_uniforms[Game::ActorShaderUniformId::Int1_CvrClr] = getUniformLocation("uEnableCvrClr");
		m_uniforms[Game::ActorShaderUniformId::Vec4_CvrClr] = getUniformLocation("uVecCvrClr");

		updateUniform1i(m_uniforms[Game::ActorShaderUniformId::Int1_CvrClr], 0);
		updateUniform2f(m_uniforms[Game::ActorShaderUniformId::Vec2_Offset], 0.0f, 0.0f);
		updateUniform4f(m_uniforms[Game::ActorShaderUniformId::Vec4_CvrClr], 1.0f, 1.0f, 1.0f, 1.0f);

		updateUniform1iName("uTexture", 0);

		Bind(nullptr);
	}

	virtual void UpdateUniform(int id, GLfloat* data) const override {
		switch (id) {
		case Game::ActorShaderUniformId::Mat4_PV:
			updateUniformMat4fv(m_uniforms[Game::ActorShaderUniformId::Mat4_PV], data);
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

	virtual void UpdateUniform1(int id, GLfloat val0) const override {}
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

	m_shader->UpdateUniform(Game::ActorShaderUniformId::Mat4_PV, &(camera.getMatPV()[0][0]));
	m_shader->UpdateUniform3(Game::ActorShaderUniformId::Vec3_CamPos, camera.getPos()[0], camera.getPos()[1], camera.getPos()[2]);

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

namespace {

std::shared_ptr<Game::ActorGroup> g_group;

}

std::shared_ptr<Game::IActorGroup> Game::IActorGroup::Instance() {
	if(g_group == nullptr)
		g_group = std::make_shared<Game::ActorGroup>();
	return g_group;
}

void Game::IActorGroup::Drop() {
	g_group.reset();
	return;
}
