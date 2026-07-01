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
#include <MysteryEngine/G3D/GlCheck.h>
#include <MysteryEngine/G3D/G3dGlobal.h>
#include <MysteryEngine/G3D/DefaultShader.h>

#include <MysteryEngine/G3D/GameStage.h>

namespace {

std::unique_ptr<ME::GameStage> g_stage;

}

namespace ME {

GameStage::GameStage() :
	m_ds(nullptr) {
	m_camera.setFOV(45.0f);
	m_camera.setAspectRatio(16.0f / 9.0f);
	m_camera.setZNear(0.25f);
	m_camera.setZFar(128.0f);
}

GameStage::~GameStage() {}

int GameStage::init() {
	if (g_stage)
		return 1;
	g_stage = std::make_unique<GameStage>();
	g_stage->setup();
	return 0;
}

GameStage& GameStage::instance() {
	assert(g_stage != nullptr);
	return *g_stage;
}

void GameStage::drop() {
	g_stage->clear();
	g_stage.reset();
}

void GameStage::addGround(std::shared_ptr<ME::IModel> ground) {
	m_ground = ground;
}

void GameStage::addActor(std::shared_ptr<GameActor> actor) {
	return m_actors.addActor(actor);
}

void GameStage::setGroundSize(float x, float y) {
	m_actors.setScale(x, y, 0.0f);
}

void GameStage::setup(int code, void* data) {
	ME::G3dGlobal::SetActive(true);
	m_ds = new ME::DefaultShader();
	m_ds->setup();
	m_actors.setup();
	ME::G3dGlobal::SetActive(false);
}

void GameStage::clear() {
	ME::G3dGlobal::SetActive(true);
	m_ground->clear();
	m_ground.reset();
	m_actors.clear();
	m_ds->clear();
	delete m_ds;
	ME::G3dGlobal::SetActive(false);
}

void GameStage::update(float dt) {
	m_actors.update(dt);
}

void GameStage::onRender() {
	m_shadowTex.setActive(true);

	glCheck(glClear(GL_COLOR_BUFFER_BIT));
	glCheck(glDisable(GL_DEPTH_TEST));

	m_actors.drawShadow(&m_camera, m_ds); // DRAW

	m_shadowTex.display();

	glCheck(glActiveTexture(GL_TEXTURE0));
	sf::Texture::bind(&m_shadowTex.getTexture());

	m_renderTexture.setActive(true);

	glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	glCheck(glEnable(GL_DEPTH_TEST));
	glCheck(glDepthMask(GL_TRUE));
	glCheck(glActiveTexture(GL_TEXTURE1));

	//ME::Shader::Bind(m_ds);

	m_ground->draw(&m_camera, m_ds); // DRAW

	glCheck(glDisable(GL_CULL_FACE));
	glCheck(glDepthMask(GL_FALSE));
	glCheck(glActiveTexture(GL_TEXTURE0));

	m_actors.draw(&m_camera, m_ds); // DRAW

	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glDepthMask(GL_TRUE));

	ME::Shader::Bind(nullptr);
}

void GameStage::onSizeChanged(sf::Vector2u newsize) {
	m_shadowTex.resize(newsize);
	m_shadowTex.setSmooth(true);

	ME::G3dGlobal::SetActive(true);
	glCheck(glViewport(0, 0, newsize.x, newsize.y));
	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glCullFace(GL_BACK));
	glCheck(glEnable(GL_BLEND));
	glCheck(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));
	//glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	//glCheck(glClearColor(1.0f, 0.0f, 0.0f, 0.5f));
	glCheck(glActiveTexture(GL_TEXTURE0));

	sf::Texture::bind(&m_shadowTex.getTexture());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 0.0f, 0.0, 0.0f, 0.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	sf::Texture::bind(nullptr);

	ME::G3dGlobal::SetActive(false);

	switch (m_camera.getType()) {
	case ME::Camera::Type::Perspective:
	case ME::Camera::Type::TEST:
		m_camera.setAspectRatio(1.0f * newsize.x / newsize.y);
		break;
	case ME::Camera::Type::Oblique:
	case ME::Camera::Type::Orthographic:
		m_camera.setDim(9.0f * newsize.x / newsize.y, 9.0f);
		break;
	default:
		break;
	}
}

}
