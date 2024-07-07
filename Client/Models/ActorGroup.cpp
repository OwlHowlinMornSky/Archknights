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
"#version 330\n"

"uniform sampler2D uTexture;"
"uniform bool uEnableCvrClr;"

"varying vec4 vTint;"
"varying vec2 vUv;"

"void main() {"
" vec4 texColor = texture2D(uTexture, vUv);"
" gl_FragColor = (uEnableCvrClr ? vec4(texColor.w) : texColor) * vTint;"
"}";

/// <summary>
/// 将深度变换到竖直面及水平面用于欺骗深度测试的顶点着色器。
/// </summary>
const std::string vertex_projection =
#ifndef _DEBUG
"#version 330\n"

"attribute vec2 aPosition; "
"attribute vec4 aColor; "
"attribute vec2 aTexCoord;"

"uniform mat4 uMatPV;"
"uniform mat4 uMatM;"
"uniform vec3 uVecCamPos;"
"uniform vec2 uVecOffset;"
"uniform bool uEnableCvrClr;"
"uniform vec4 uVecCvrClr;"

"varying vec4 vTint;"
"varying vec2 vUv;"

"void main() {"
" vTint = uEnableCvrClr ? aColor.w * uVecCvrClr : aColor * uVecCvrClr;"
" vUv = aTexCoord;"

" vec2 VertexPosInModel = aPosition / 512.0 + uVecOffset;"
" vec4 VertexPosInGlobal = uMatM * vec4(VertexPosInModel.xy, 0.0, 1.0);"
" vec4 OrgPosInGlobal = uMatM * vec4(0.0, 0.0, 0.0, 1.0);"

" gl_Position = uMatPV * VertexPosInGlobal;"

" vec3 CamPosInOrg = uVecCamPos - OrgPosInGlobal.xyz;"
" vec3 VertexPosInOrg = VertexPosInGlobal.xyz - OrgPosInGlobal.xyz;"

" vec3 VecPositionToCamera = CamPosInOrg - VertexPosInOrg;"

" vec4 FakePosInGlobal = vec4(VertexPosInGlobal.xyz - VecPositionToCamera * mix(VertexPosInOrg.z / VecPositionToCamera.z, VertexPosInOrg.y / VecPositionToCamera.y, step(0.0, aPosition.y)), 1.0);"
" vec4 FakeProjection = uMatPV * FakePosInGlobal;"

" gl_Position.z = gl_Position.w * FakeProjection.z / FakeProjection.w;"
"}";
#else
"#version 330\n"

"attribute vec2 aPosition; "
"attribute vec4 aColor; "
"attribute vec2 aTexCoord;"

"uniform mat4 uMatPV;"
"uniform mat4 uMatM;"
"uniform vec3 uVecCamPos;"
"uniform vec2 uVecOffset;"
"uniform bool uEnableCvrClr;"
"uniform vec4 uVecCvrClr;"

"varying vec4 vTint;"
"varying vec2 vUv;"

"void main() {"
" vTint = uEnableCvrClr ? aColor.w * uVecCvrClr : aColor * uVecCvrClr;"
" vUv = aTexCoord;"

" vec2 VertexPosInModel = aPosition / 512.0 + uVecOffset;"
" vec4 VertexPosInGlobal = uMatM * vec4(VertexPosInModel.xy, 0.0, 1.0);"
" vec4 OrgPosInGlobal = uMatM * vec4(0.0, 0.0, 0.0, 1.0);"

" gl_Position = uMatPV * VertexPosInGlobal;"

" vec3 CamPosInOrg = uVecCamPos - OrgPosInGlobal.xyz;"
" vec3 VertexPosInOrg = VertexPosInGlobal.xyz - OrgPosInGlobal.xyz;"

" vec3 VecPositionToCamera = CamPosInOrg - VertexPosInOrg;"

" vec4 FakePosInGlobal = vec4(VertexPosInGlobal.xyz - VecPositionToCamera * mix(VertexPosInOrg.z / VecPositionToCamera.z, VertexPosInOrg.y / VecPositionToCamera.y, step(0.0, aPosition.y)), 1.0);"
" vec4 FakeProjection = uMatPV * FakePosInGlobal;"

" gl_Position.z = mix(gl_Position.w * FakeProjection.z / FakeProjection.w, gl_Position.z, step(0.0, min(OrgPosInGlobal.z, (uVecCamPos.y - OrgPosInGlobal.y + 0.1))));"
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

	virtual void update(int id, GLfloat* data) const override {
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

	virtual void update1f(int id, GLfloat val0) const override {}
	virtual void update2f(int id, GLfloat val0, GLfloat val1) const override {
		updateUniform2f(m_uniforms[id], val0, val1);
	}
	virtual void update3f(int id, GLfloat val0, GLfloat val1, GLfloat val2) const override {
		updateUniform3f(m_uniforms[id], val0, val1, val2);
	}
	virtual void update4f(int id, GLfloat val0, GLfloat val1, GLfloat val2, GLfloat val3) const override {
		updateUniform4f(m_uniforms[id], val0, val1, val2, val3);
	}

	virtual void update1i(int id, GLint val) const override {
		updateUniform1i(m_uniforms[id], val);
	}
};


const std::string fragment_shadow =
"#version 330\n"

"uniform sampler2D uTexture;"

"varying vec4 vTint;"
"varying vec2 vUv;"

"void main() {"
" vec4 texColor = texture2D(uTexture, vUv);"
" gl_FragColor = vTint;"
"}";

const std::string vertex_shadow =
"#version 330\n"

"attribute vec2 aPosition;"
"attribute vec4 aColor;"
"attribute vec2 aTexCoord;"

"uniform vec2 uGroundSz;"
"uniform vec4 uVecOffset[256];"

"varying vec4 vTint;"
"varying vec2 vUv;"

"void main() {"
" vTint = aColor * uVecOffset[gl_InstanceID].w;"
" vUv = aTexCoord;"

" gl_Position = vec4((aPosition * uVecOffset[gl_InstanceID].z + uVecOffset[gl_InstanceID].xy) / uGroundSz * 2.0 - 1.0, 0.0, 1.0);"

"}";

class ShadowShader final :
	public ME::Shader {
protected:
	int m_uniforms[2];

public:
	ShadowShader() :
		m_uniforms() {}
	virtual ~ShadowShader() = default;

public:
	virtual void setup() override {
		clear();
		loadFromMemory(vertex_shadow, ME::ShaderType::Vertex);
		loadFromMemory(fragment_shadow, ME::ShaderType::Fragment);
		glCheck(glBindAttribLocation(m_program, static_cast<GLuint>(Game::ActorVertexAttribute::Position), "aPosition"));
		glCheck(glBindAttribLocation(m_program, static_cast<GLuint>(Game::ActorVertexAttribute::TexCoord), "aTexCoord"));
		glCheck(glBindAttribLocation(m_program, static_cast<GLuint>(Game::ActorVertexAttribute::Color), "aColor"));
		linkShader();
		Bind(this);

		m_uniforms[0] = getUniformLocation("uVecOffset");
		m_uniforms[1] = getUniformLocation("uGroundSz");

		updateUniform1iName("uTexture", 0);

		Bind(nullptr);
	}

	virtual void update(int id, GLfloat* data) const override {
		updateUniform2f(m_uniforms[id], data[0], data[1]);
	}

	virtual void updateV(int id, GLsizei count, GLfloat* data) const override {
		glCheck(glUniform4fv(m_uniforms[0], count, data));
	}
};

}

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace Model {

ActorGroup::ActorGroup() {}

ActorGroup::~ActorGroup() {
	clear();
}

void ActorGroup::addActor(std::shared_ptr<Game::IActor> actor) {
	m_actors.push_back(actor);
}

bool ActorGroup::setup() {
	m_shader = std::make_unique<ActorShader>();
	m_shader->setup();
	m_shadowShader = std::make_unique<ShadowShader>();
	m_shadowShader->setup();
	m_shadow.setup();
	return true;
}

void ActorGroup::clear() {
	for (auto& i : m_actors)
		i->clear();
	m_actors.clear();
	m_shadow.clear();
	m_shadowShader.reset();
	m_shader.reset();
}

void ActorGroup::update(float dt) {
	std::list<std::shared_ptr<Game::IActor>>::iterator i, n;
	i = m_actors.begin();
	n = m_actors.end();
	for (; i != n;) {
		if ((*i)->isWaitingForQuit()) {
			i = m_actors.erase(i);
		}
		else {
			(*i)->update(dt);
			i++;
		}
	}
	m_actors.sort(
		[](std::shared_ptr<Game::IActor>& l, std::shared_ptr<Game::IActor>& r) {
			return l->getPosition().y > r->getPosition().y;
		}
	);
}

void ActorGroup::draw(ME::Camera* camera, ME::Shader* shader) {
	ME::Shader::Bind(m_shader.get());

	m_shader->update(Game::ActorShaderUniformId::Mat4_PV, &(camera->getMatPV()[0][0]));
	m_shader->update3f(Game::ActorShaderUniformId::Vec3_CamPos, camera->getPosition()[0], camera->getPosition()[1], camera->getPosition()[2]);

	for (std::shared_ptr<Game::IActor>& i : m_actors) {
		i->draw(camera, m_shader.get());
	}

	ME::Shader::Bind(shader);
}

void ActorGroup::drawShadow(ME::Camera* camera, ME::Shader* shader) {
	if (m_actors.empty())
		return;
	ME::Shader::Bind(m_shadowShader.get());

	m_shadowShader->update(1, &(m_scale[0]));

	std::vector<glm::vec4> u(256);
	int i = 0;
	for (auto it = m_actors.begin(), ed = m_actors.end(); it != ed; ++it, ++i) {
		if (i >= 256) {
			m_shadowShader->updateV(0, 256, &(u[0][0]));
			m_shadow.drawInstance(256);
			i = 0;
		}
		Game::IActor& a = **it;
		auto& p = a.getPosition();
		u[i] = { p.x, p.y, a.m_shadowRadius, a.m_shadowAlpha };
	}
	m_shadowShader->updateV(0, i, &(u[0][0]));
	m_shadow.drawInstance(i);
	i = 0;

	ME::Shader::Bind(shader);
}

}
