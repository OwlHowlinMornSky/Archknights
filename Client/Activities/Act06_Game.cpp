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
#include "Act06_Game.h"
#include "../Game/Global.h"
#include "../Game/Board.h"
#include "../Game/Stage.h"
#include "../Main/Creator.h"
#include "../Activities/Act01_DefaultEntrance.h"

namespace Activity {

Act06_Game::Act06_Game() :
	m_paused(false) {}

Act06_Game::~Act06_Game() noexcept {}

bool Act06_Game::prepare(ME::Window& wnd) noexcept {
	r(wnd);
	int res = Main::Creator::setup();
	return res == 0;
}

void Act06_Game::start() noexcept {
	Game::Global::board->SetExitCallback(std::bind(&Act06_Game::ExitGame, this, std::placeholders::_1));
	UpdateSize(r->getRealtimeSize());
}

void Act06_Game::stop() noexcept {
	Main::Creator::drop();
	r();
	return;
}

bool Act06_Game::handleEvent(const sf::Event& evt) {
	switch (evt.type) {
	case sf::Event::Closed:
		r->setWaitingForStop();
		return true;
	case sf::Event::Resized:
		UpdateSize({ evt.size.width, evt.size.height });
		break;
	}
	sf::Event e = evt;
	Game::Global::board->PostMsg(1, 0, (intptr_t)&e);
	return false;
}

void Act06_Game::update(sf::Time dtime) {
#ifdef _DEBUG
	r->clear(sf::Color(0x333333FF));
#endif // _DEBUG
	if (m_paused)
		dtime = sf::Time::Zero;
	Game::Global::board->Update(dtime.asMicroseconds());
	Game::Global::stage->update(dtime.asSeconds());
	Game::Global::stage->render();
	r->draw(*Game::Global::stage);
	r->display();
	return;
}

void Act06_Game::OnEnterSysloop() noexcept {
	m_paused = true;
}

void Act06_Game::OnExitSysloop() noexcept {
	m_paused = false;
}

void Act06_Game::ExitGame(int code) {
	switch (code) {
	case 4321:
		r->changeActivity(std::make_unique<Act01_DefaultEntrance>());
		break;
	}
	return;
}

void Act06_Game::UpdateSize(sf::Vector2u size) {
	Game::Global::stage->resize(size);
}

} // namespace Activity
