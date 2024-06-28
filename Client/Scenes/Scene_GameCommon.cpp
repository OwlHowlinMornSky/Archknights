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
//#include <GL/glew.h>
//#include <MysteryEngine/G3D/GlCheck.h>

#include <GL/glew.h>
#include <MysteryEngine/G3D/GlCheck.h>
#include <MysteryEngine/G3D/Shader.Default.h>

#include "Scene_GameCommon.h"
#include "../Game/GameGlobal.h"
#include <MysteryEngine/G3D/G3dGlobal.h>
#include <assert.h>


namespace Scene {

GameCommon::GameCommon() {
	m_ds = new ME::DefaultShader();
	m_ds->setup();
}

GameCommon::~GameCommon() {}

int GameCommon::setup() {
	if (Game::GameGlobal::show)
		return 1;
	Game::GameGlobal::show = std::make_unique<GameCommon>();
	return 0;
}

void GameCommon::drop() {
	Game::GameGlobal::show.reset();
}

void GameCommon::update(float dt) {
	m_actors.Update(dt);
}

void GameCommon::AddGround(std::shared_ptr<ME::IModel> ground) {
	m_ground = ground;
}

void GameCommon::AddActor(std::shared_ptr<ME::IModel> actor) {
	return m_actors.AddActor(actor);
}

void GameCommon::onRender() {
	m_shadowTex.setActive(true);

	glCheck(glClear(GL_COLOR_BUFFER_BIT));
	glCheck(glDisable(GL_DEPTH_TEST));


	m_renderTexture.setActive(true);

	glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	glCheck(glEnable(GL_DEPTH_TEST));
	glCheck(glDepthMask(GL_TRUE));

	ME::Shader::Bind(m_ds);

	m_ground->Draw(m_camera, *m_ds); // DRAW

	glCheck(glDisable(GL_CULL_FACE));
	glCheck(glDepthMask(GL_FALSE));

	m_actors.Draw(m_camera, *m_ds); // DRAW

	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glDepthMask(GL_TRUE));

	ME::Shader::Bind(nullptr);
}

void GameCommon::onSizeChanged(sf::Vector2u newsize) {
	m_shadowTex.create(newsize.x, newsize.y);

	ME::G3dGlobal::setActive(true);
	glCheck(glViewport(0, 0, newsize.x, newsize.y));
	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glCullFace(GL_BACK));
	glCheck(glEnable(GL_BLEND));
	glCheck(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));
	//glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	//glCheck(glClearColor(1.0f, 0.0f, 0.0f, 1.0f));
	ME::G3dGlobal::setActive(false);

	switch (m_camera.getType()) {
	case ME::Camera::Type::Perspective:
		m_camera.setAspectRatio(1.0f * newsize.x / newsize.y);
		break;
	default:
		break;
	}
}

} // namespace Scene
