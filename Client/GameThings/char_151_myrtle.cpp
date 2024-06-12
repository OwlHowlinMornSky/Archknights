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
#include "MsgId.h"
#include "../Game/GameGlobal.h"
#include "../Game/GameBoard.h"
#include "../Game/AtkDmgHeal.h"

Units::Char_151_Myrtle::Char_151_Myrtle() {}

Units::Char_151_Myrtle::~Char_151_Myrtle() {}

void Units::Char_151_Myrtle::OnJoined() {
	Parent::OnJoined();

	Physics::Rows rows{};
	rows.length = 2;
	uint32_t wd[2] = { 1, 1 };
	rows.widths = wd;

	//m_inde = Game::GameGlobal::board->m_world->CreateDetectorCircle(m_position[0], m_position[1], 1.0f);
	m_detector = Game::GameGlobal::board->m_world->CreateDetectorRows(Physics::ArmyStand, m_position[0], m_position[1], &rows);
	m_detector->SetId(m_id);
	m_detector->SetLocation(m_location);
	m_hp = 1.0f;

	m_active = false;
	m_died = false;
	//m_attacking = false;

	Game::GameGlobal::board->SubscribeMsg(Game::MsgId::GuiEvent, m_location);
}

void Units::Char_151_Myrtle::OnKicking() {
	Game::GameGlobal::board->UnsubscribeMsg(Game::MsgId::GuiEvent, m_location);

	Parent::OnKicking();
}

void Units::Char_151_Myrtle::FixedUpdate(float dt) {
	switch (m_status) {
	case Status::Idle:
		for (auto it = m_detector->ListBegin(), n = m_detector->ListEnd(); it != n; ++it) {
			if (it->first == m_id)
				continue;
			m_targetAd = it->second.location;
			m_targetId = it->first;
			if (Game::GameGlobal::board->TellMsg(m_targetAd, m_targetId, Game::MsgId::OnSelecting, 0, 0) != Game::MsgResult::OK)
				continue;
			m_attacked = false;
			ToAttack();
			break;
		}
		break;
	case Status::Attaking:
		if (m_actor->AnimEvent_AttackOver()) {
			ToIdle();
		}
		else if (!m_attacked && Game::GameGlobal::board->TellMsg(m_targetAd, m_targetId, Game::MsgId::OnSelecting, 0, 0) != Game::MsgResult::OK) {
			ToIdle();
		}
		else {
			int cnt = m_actor->AnimEventCnt_OnAttack();
			while (cnt--) {
				Game::AttackData data;
				data.sourceAd = m_location;
				data.sourceId = m_id;
				data.distType = data.Near;
				data.damage.type = data.damage.Normal;
				data.damage.dmgValue = 420.0f;
				data.damage.minValue = 0.05f;

				Game::GameGlobal::board->TellMsg(m_targetAd, m_targetId, Game::MsgId::OnGetAttack, 0, (intptr_t)&data);

				m_attacked = true;
			}
		}
		break;
	default:
		return Parent::FixedUpdate(dt);
	}
}

#include <SFML/Window/Event.hpp>

Game::MsgResultType Units::Char_151_Myrtle::ReceiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	switch (msg) {
	case Game::MsgId::GuiEvent:
	{
		sf::Event& evt = *(sf::Event*)lparam;
		switch (evt.type) {
		case sf::Event::KeyReleased:
			switch (evt.key.code) {
			case sf::Keyboard::A:
				m_actor->ChangeStatus(Game::IActor::AnimationStatus::Skill0);
				ToBegin(Game::IActor::Direction::BR);
				break;
			case sf::Keyboard::Q:
				ToReturn(Game::IActor::Direction::BL);
				break;
			}
			break;
		}
		break;
	}
	default:
		return DefTowerProc(msg, wparam, lparam);
	}
	return Game::MsgResult::OK;
}
