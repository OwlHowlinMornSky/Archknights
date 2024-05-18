﻿/*
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
#include "../Activities/Act01_DefaultEntrance.h"

namespace Activity {

Activity_Game::Activity_Game() {}

Activity_Game::~Activity_Game() noexcept {}

bool Activity_Game::start(ME::Window& wnd) noexcept {
	r(wnd);
	int res = Game::Creator::setup();
	Game::GameGlobal::board->SetExitCallback(std::bind(&Activity_Game::ExitGame, this, std::placeholders::_1));


	
	mngr = new ohms::SpineManager();
	auto set = mngr->addPose("char_151_myrtle", 0);

	//ME::G3dGlobal::setActive(true);
	anim = set->runOneEntity();
	//ME::G3dGlobal::setActive(false);

	anim->setPosition(0.0f, 0.0f, 0.0f);
	anim->setRotation(0.0f, 0.0f, 0.0f);
	anim->setOrigin(0.0f, 0.0f, 0.0f);
	anim->setScale(1.0f, 1.0f, 1.0f);

	//anim->addAnimation(0, "")
	//anim->addEmptyAnimation(0, true, 0.0f);

	//std::shared_ptr<ohms::SpineEntity> ani(anim);
	//Game::GameGlobal::show->AddAnimation(ani);

	UpdateSize();
	return res == 0;
}

void Activity_Game::stop() noexcept {
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
	//r->clear(sf::Color::White);
	r->clear(sf::Color(0x333333FF));
#endif // _DEBUG
	Game::GameGlobal::board->Update(dtime.asSeconds());
	Game::GameGlobal::show->Update(dtime.asSeconds());

	anim->Update(dtime.asSeconds());

	r->draw(*Game::GameGlobal::show);

	r->draw(*anim);

	r->display();
	return;
}

void Activity_Game::OnEnterSysloop() noexcept {
	Game::GameGlobal::board->SetPaused(true);
}

void Activity_Game::OnExitSysloop() noexcept {}

void Activity_Game::ExitGame(int code) {
	switch (code) {
	case 4321:
		r->changeActivity(std::make_unique<Act01_DefaultEntrance>());
		break;
	}
	return;
}

void Activity_Game::UpdateSize() {
	Game::GameGlobal::show->SetSize(r->getSize());

	//auto& view = r->getView();
	//sf::Vector2f size = view.getSize();
}

} // namespace Activity
