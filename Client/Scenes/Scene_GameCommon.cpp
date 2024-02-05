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
#include "Scene_GameCommon.h"
#include "../Game/GameGlobal.h"
#include <assert.h>

namespace {

std::shared_ptr<Scene::GameCommon> g_gamecommon;

}

namespace Scene {

GameCommon::GameCommon() {}

GameCommon::~GameCommon() {}

int GameCommon::setup() {
	if (g_gamecommon)
		return 1;
	g_gamecommon = std::make_unique<GameCommon>();
	Game::GameGlobal::show = g_gamecommon;
	return 0;
}

GameCommon* GameCommon::instance() {
	assert(g_gamecommon != nullptr);
	return g_gamecommon.get();
}

void GameCommon::drop() {
	Game::GameGlobal::show.reset();
	g_gamecommon.reset();
}

void GameCommon::Render() {


}

void GameCommon::update(float dt) {
	Update(dt);
	Render();
}

void GameCommon::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	return target.draw(m_sp, states);
}

} // namespace Scene
