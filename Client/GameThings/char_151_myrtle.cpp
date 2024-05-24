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
#include "char_151_myrtle.h"

#include "../Game/MsgResult.h"
#include <SFML/Window/Event.hpp>
#include "MsgId.h"

Units::Char_151_Myrtle::Char_151_Myrtle() {}

Units::Char_151_Myrtle::~Char_151_Myrtle() {}

void Units::Char_151_Myrtle::OnJoined() {
	m_actor->TriggerAnimation(Game::IActor::AnimationEvent::Begin);
	Game::GameGlobal::board->SubscribeMsg(Game::MsgId::GuiEvent, m_location);

	m_body = Game::GameGlobal::board->m_world->CreateBodyTowerCircle(m_position[0], m_position[1]);
	m_body->SetId(m_id);
	m_body->SetLocation(m_location);

	m_inde = Game::GameGlobal::board->m_world->CreateDetectorCircle(m_position[0], m_position[1], 1.0f);
	m_inde->SetId(m_id);
	m_inde->SetLocation(m_location);

	m_test = false;
}

void Units::Char_151_Myrtle::OnKicking() {
	m_inde.reset();
	m_body.reset();

	Game::GameGlobal::board->UnsubscribeMsg(Game::MsgId::GuiEvent, m_location);
	m_actor->Exit();
}

void Units::Char_151_Myrtle::FixedUpdate(float dt) {
	
	if (m_test)
		return;

	bool atk = false;
	for (auto it = m_inde->ListBegin(), n = m_inde->ListEnd(); it != n; ++it) {
		atk = true;
	}

	if (atk) {
		m_actor->TriggerAnimation(Game::IActor::AnimationEvent::Die);
		m_test = true;
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
	default:
		return Game::MsgResult::Unsubscribe;
	}
	return Game::MsgResult::OK;
}

void Units::Char_151_Myrtle::OnPositionChanged() {
	m_actor->SetPosition(m_position[0], m_position[1], 0.0f);
}
