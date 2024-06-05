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
	uint32_t wd[2] = { 0, 0 };
	rows.widths = wd;

	//m_inde = Game::GameGlobal::board->m_world->CreateDetectorCircle(m_position[0], m_position[1], 1.0f);
	m_detector = Game::GameGlobal::board->m_world->CreateDetectorRows(m_position[0], m_position[1], rows);
	m_detector->SetId(m_id);
	m_detector->SetLocation(m_location);

	m_active = false;
	m_died = false;
	m_attacking = false;
}

void Units::Char_151_Myrtle::OnKicking() {
	Parent::OnKicking();
}

void Units::Char_151_Myrtle::FixedUpdate(float dt) {
	if (!m_active) {
		if (m_actor->AnimEvent_StartOver()) {
			m_active = true;
		}
		return;
	}
	if (m_died) {
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
				Game::AttackData data;
				data.sourceAd = m_location;
				data.sourceId = m_id;
				data.distType = data.Near;
				data.damage.type = data.damage.Normal;
				data.damage.dmgValue = 52.0f;
				data.damage.minValue = 0.05f;

				Game::GameGlobal::board->TellMsg(m_targetAd, m_targetId, Game::MsgId::OnGetAttack, 0, (intptr_t)&data);
				
				//Game::HealData hdata;
				//hdata.healValue = 74.0f;
				//Game::GameGlobal::board->TellMsg(m_targetAd, m_targetId, Game::MsgId::OnGetHeal, 0, (intptr_t)&hdata);
				
				m_attacked = true;
			}
		}
	}
	if (!m_attacking) {
		for (auto it = m_detector->ListBegin(), n = m_detector->ListEnd(); it != n; ++it) {
			if (it->first == m_id)
				continue;
				m_targetAd = it->second.location;
				m_targetId = it->first;
			if (Game::GameGlobal::board->TellMsg(m_targetAd, m_targetId, Game::MsgId::OnSelecting, 0, 0) != Game::MsgResult::OK)
				continue;
					m_attacking = true;
					m_attacked = false;
					m_actor->TriggerAnimation(
						Game::IActor::AnimationEvent::Attack
					);
			break;
		}
	}
}

Game::MsgResultType Units::Char_151_Myrtle::ReceiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	switch (msg) {
	default:
		return DefTowerProc(msg, wparam, lparam);
	}
	return Game::MsgResult::OK;
}
