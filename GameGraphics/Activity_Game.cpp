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
#include "Global.h"
#include "../Game/GameGlobal.h"
#include "../Game/GameBoard.h"

namespace gamegui {

Activity_Game::Activity_Game() :
	r_board(nullptr) {}

Activity_Game::~Activity_Game() noexcept {}

bool Activity_Game::start(GUI::Window& wnd) noexcept {
	r(wnd);
	game::Global::instance()->data.board = std::make_unique<game::GameBoard>();
	r_board = game::Global::instance()->data.board.get();
	r_board->setup();
	m_scene = std::make_unique<SceneCommon>();
	return true;
}

void Activity_Game::stop() noexcept {
	m_scene.reset();
	r();
	return;
}

bool Activity_Game::handleEvent(const sf::Event& evt) {
	if (m_scene->handleEvent(evt)) {
		// 返回
		return true;
	}
	return false;
}

void Activity_Game::update(sf::Time dtime) {
#ifdef _DEBUG
	r->clear(sf::Color::White);
#endif // _DEBUG
	r_board->Update(dtime.asSeconds());
	m_scene->update(dtime.asSeconds());
	r->draw(*m_scene);
	r->display();
	return;
}

void Activity_Game::OnEnterSysloop() noexcept {}

void Activity_Game::OnExitSysloop() noexcept {}

}
