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
#include "char_151_myrtle.h"

#include "../Game/MsgResult.h"
#include <SFML/Window/Event.hpp>
#include "MsgId.h"
#include "../Game/GameGlobal.h"
#include "../Game/GameBoard.h"

Units::Char_151_Myrtle::Char_151_Myrtle() {}

Units::Char_151_Myrtle::~Char_151_Myrtle() {}

void Units::Char_151_Myrtle::OnJoined() {
	Parent::OnJoined();

	Game::GameGlobal::board->SubscribeMsg(Game::MsgId::GuiEvent, m_location);

	Physics::Rows rows{};
	rows.length = 2;
	uint32_t wd[2] = { 0, 0 };
	rows.widths = wd;

	//m_inde = Game::GameGlobal::board->m_world->CreateDetectorCircle(m_position[0], m_position[1], 1.0f);
	m_detector = Game::GameGlobal::board->m_world->CreateDetectorRows(m_position[0], m_position[1], rows);
	m_detector->SetId(m_id);
	m_detector->SetLocation(m_location);

	m_died = false;
}

void Units::Char_151_Myrtle::OnKicking() {
	Game::GameGlobal::board->UnsubscribeMsg(Game::MsgId::GuiEvent, m_location);

	Parent::OnKicking();
}

void Units::Char_151_Myrtle::FixedUpdate(float dt) {
	if (m_died) {
		m_time += dt;
		if (m_time >= 1.2f) {
			KickSelf();
		}
		return;
	}

	for (auto it = m_detector->ListBegin(), n = m_detector->ListEnd(); it != n; ++it) {
		if (it->first != m_id) {
			Game::GameGlobal::board->TellMsg(it->second.location, it->first, 114514, 0, 0);
		}
	}
}

Game::MsgResultType Units::Char_151_Myrtle::ReceiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	switch (msg) {
	case Game::MsgId::GuiEvent:
	{
		auto e = (sf::Event*)lparam;
		switch (e->type) {
		case sf::Event::KeyPressed:
			switch (e->key.code) {
			case sf::Keyboard::Numpad1:
				m_actor->TriggerAnimation(
					Game::IActor::AnimationEvent::Idle,
					Game::IActor::Direction::FL
				);
				break;
			case sf::Keyboard::Numpad3:
				m_actor->TriggerAnimation(
					Game::IActor::AnimationEvent::Idle,
					Game::IActor::Direction::FR
				);
				break;
			case sf::Keyboard::Numpad7:
				m_actor->TriggerAnimation(
					Game::IActor::AnimationEvent::Idle,
					Game::IActor::Direction::BL
				);
				break;
			case sf::Keyboard::Numpad9:
				m_actor->TriggerAnimation(
					Game::IActor::AnimationEvent::Idle,
					Game::IActor::Direction::BR
				);
				break;
			case sf::Keyboard::Num0:
				m_actor->TriggerAnimation(
					Game::IActor::AnimationEvent::Die,
					Game::IActor::Direction::FR
				);
				break;
			}
			break;
		}
		break;
	}
	case 114514:
		if (m_died)
			break;
		m_actor->TriggerAnimation(Game::IActor::AnimationEvent::Die);
		m_died = true;
		m_time = 0.0f;
		break;
	default:
		return Game::MsgResult::Unsubscribe;
	}
	return Game::MsgResult::OK;
}
