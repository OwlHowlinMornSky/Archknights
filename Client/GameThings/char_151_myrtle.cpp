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

	m_active = false;
	m_died = false;
	m_attacking = false;

	hp = 100.0f;

}

void Units::Char_151_Myrtle::OnKicking() {
	Game::GameGlobal::board->UnsubscribeMsg(Game::MsgId::GuiEvent, m_location);

	Parent::OnKicking();
}

void Units::Char_151_Myrtle::FixedUpdate(float dt) {
	if (!m_active) {
		//if (m_actor->AnimEventCnt_OnStart()) {
		if (m_actor->AnimEvent_StartOver()) {
			m_active = true;
		}
		return;
	}
	if (m_died) {
		//m_time += dt;
		//if (m_time >= 1.2f) {
		//	KickSelf();
		//}
		if (m_actor->AnimEvent_DieOver()) {
			KickSelf();
		}
		return;
	}
	if (m_attacking) {
		if (m_actor->AnimEvent_AttackOver()) {
			m_attacking = false;
			m_actor->TriggerAnimation(
				Game::IActor::AnimationEvent::Idle
			);
		}
		else if (!m_attacked && Game::GameGlobal::board->TellMsg(m_targetAd, m_targetId, Game::MsgId::OnSelecting, 0, 0) != Game::MsgResult::OK) {
			m_attacking = false;
			m_actor->TriggerAnimation(
				Game::IActor::AnimationEvent::Idle
			);
		}
		else {
			int cnt = m_actor->AnimEventCnt_OnAttack();
			while (cnt--) {
				//Game::GameGlobal::board->TellMsg(m_targetAd, m_targetId, 114514, 0, 0);
				Game::GameGlobal::board->TellMsg(m_targetAd, m_targetId, Game::MsgId::OnGetAttack, 11, 0);
				m_attacked = true;
			}
		}
	}
	if (!m_attacking) {
		for (auto it = m_detector->ListBegin(), n = m_detector->ListEnd(); it != n; ++it) {
			if (it->first != m_id) {
				m_targetAd = it->second.location;
				m_targetId = it->first;
				if (Game::GameGlobal::board->TellMsg(m_targetAd, m_targetId, Game::MsgId::OnSelecting, 0, 0) == Game::MsgResult::OK) {
					m_attacking = true;
					m_attacked = false;
					m_actor->TriggerAnimation(
						Game::IActor::AnimationEvent::Attack
					);
				}
			}
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
			case sf::Keyboard::Numpad5:
				m_actor->TriggerAnimation(
					Game::IActor::AnimationEvent::Attack
				);
				break;
			}
			break;
		}
		break;
	}
	case Game::MsgId::OnSelecting:
		if (m_active && !m_died) {
			return Game::MsgResult::OK;
		}
		else {
			return Game::MsgResult::MethodNotAllowed;
		}
		break;
	case Game::MsgId::OnGetAttack:
		this->ReceiveMessage(Game::MsgId::OnGetDamage, wparam, 0);
		break;
	case Game::MsgId::OnGetDamage:
		this->hp -= wparam;
		this->ReceiveMessage(Game::MsgId::OnHpChanged, wparam, 0);
		break;
	case Game::MsgId::OnHpChanged:
		if (hp <= 0) {
			this->ReceiveMessage(Game::MsgId::OnHpDropToZero, wparam, 0);
		}
		break;
	case Game::MsgId::OnHpDropToZero:
		m_actor->TriggerAnimation(Game::IActor::AnimationEvent::Die);
		m_died = true;
		m_body.reset();
		m_detector.reset();
		break;
	default:
		return Game::MsgResult::Unsubscribe;
	}
	return Game::MsgResult::OK;
}
