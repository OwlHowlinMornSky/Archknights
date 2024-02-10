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
#include "Activity_Game.h"
#include "../Scenes/Scene_GameCommon.h"
#include "../GameThings/Creator.h"
#include "../Game/GameGlobal.h"
#include "../Game/GameBoard.h"

namespace {

Scene::GameCommon* r_scene = nullptr;

}

namespace Activity {

Activity_Game::Activity_Game() {}

Activity_Game::~Activity_Game() noexcept {}

bool Activity_Game::start(ME::Window& wnd) noexcept {
	r(wnd);
	int res = Game::Creator::setup();
	r_scene = Scene::GameCommon::instance();
	return res == 0;
}

void Activity_Game::stop() noexcept {
	r_scene = nullptr;
	Game::Creator::drop();
	r();
	return;
}

bool Activity_Game::handleEvent(const sf::Event& evt) {
	if (evt.type == sf::Event::Closed) {
		r->setWaitingForStop();
		return true;
	}
	sf::Event e = evt;
	Game::GameGlobal::board->DistributeMsg(1, 0, (intptr_t)&e);
	return false;
}

void Activity_Game::update(sf::Time dtime) {
#ifdef _DEBUG
	r->clear(sf::Color::White);
#endif // _DEBUG
	Game::GameGlobal::board->Update(dtime.asSeconds());
	r_scene->update(dtime.asSeconds());
	r->draw(*r_scene);
	r->display();
	return;
}

void Activity_Game::OnEnterSysloop() noexcept {
	Game::GameGlobal::board->SetPaused(true);
}

void Activity_Game::OnExitSysloop() noexcept {}

} // namespace Activity
