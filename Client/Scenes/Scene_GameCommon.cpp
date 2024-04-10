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

#include "Scene_GameCommon.h"
#include "../Game/GameGlobal.h"
//#include <MysteryEngine/G3D/G3dGlobal.h>
#include <assert.h>

namespace Scene {

GameCommon::GameCommon() {}

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

void GameCommon::Render() {
	for (auto& i : m_anims) {
		i->Draw(*m_camera);
	}
}

void GameCommon::UpdateModels(float dt) {
	for (auto& i : m_anims) {
		i->Update(dt);
	}
}

} // namespace Scene
