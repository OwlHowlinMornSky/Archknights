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
#include "char_128_plosis.h"

#include "../Game/MsgResult.h"
#include "MsgId.h"
#include "../Game/AtkDmgHeal.h"

Units::Char_128_Plosis::Char_128_Plosis() {}

Units::Char_128_Plosis::~Char_128_Plosis() {}

void Units::Char_128_Plosis::OnJoined() {
	Parent::OnJoined();

	Physics::Rows rows{};
	rows.offset = -1;
	rows.length = 4;
	uint32_t wd[4] = { 2, 2, 2, 2 };
	rows.widths = wd;

	m_detector = Game::GameGlobal::board->m_world->CreateDetectorRows(Physics::ArmyStand, m_position[0], m_position[1], &rows);
	m_detector->SetId(m_id);
	m_detector->SetLocation(m_location);

	SetAttributeOringalValue(AttributeType::MaxHp, 1402.0f);

	m_m.SetTarget(m_location, m_id, AttributeType::MaxHp);
	m_m.SetValue(123.0f, 1.0f, 321.0f, 0.5f);
	m_m.SetEnabled(true);

	m_addDef.SetTarget(m_location, m_id, AttributeType::Def);
	m_addDef.SetValue(320.0f, 0.0f, 0.0f, 0.0f);
	m_addDef.SetEnabled(true);

	m_addMsgDef.SetTarget(m_location, m_id, AttributeType::MagDef);
	m_addMsgDef.SetValue(1.0f, 0.0f, 0.0f, 0.0f);
	m_addMsgDef.SetEnabled(true);

	m_hp = attributes[AttributeType::MaxHp].effective;

	printf_s("HP: %f\n", m_hp);
	printf_s("MaxHP: %f\n", attributes[AttributeType::MaxHp].effective);
	printf_s("Def: %f\n", attributes[AttributeType::Def].effective);
	printf_s("MDef: %f\n", attributes[AttributeType::MagDef].effective);

	abilities[AbilityType::Attack].SetOriginal(1);
}

void Units::Char_128_Plosis::OnKicking() {
	m_addMsgDef.SetEnabled(false);
	m_addDef.SetEnabled(false);
	m_m.SetEnabled(false);

	Parent::OnKicking();
}

void Units::Char_128_Plosis::FixedUpdate() {
	//switch (m_status) {
	//default:
	return Parent::FixedUpdate();
	//}
}

bool Units::Char_128_Plosis::TryAttack() {
	for (auto it = m_detector->ListBegin(), n = m_detector->ListEnd(); it != n; ++it) {
		m_targetAd = it->second.location;
		m_targetId = it->first;
		if (Game::GameGlobal::board->TellMsg(m_targetAd, m_targetId, Game::MsgId::OnSelecting, 0, 0) != Game::MsgResult::OK)
			continue;
		auto t = Game::GameGlobal::board->EntityAt(m_targetAd);
		if (t->GetHp() >= t->GetAttribute(AttributeType::MaxHp))
			continue;
		return false;
	}
	return true;
}

bool Units::Char_128_Plosis::StillCanAttack() {
	if (Game::GameGlobal::board->TellMsg(m_targetAd, m_targetId, Game::MsgId::OnSelecting, 0, 0) == Game::MsgResult::OK) {
		auto t = Game::GameGlobal::board->EntityAt(m_targetAd);
		if (t->GetHp() < t->GetAttribute(AttributeType::MaxHp))
			return true;
	}
	for (auto it = m_detector->ListBegin(), n = m_detector->ListEnd(); it != n; ++it) {
		m_targetAd = it->second.location;
		m_targetId = it->first;
		if (Game::GameGlobal::board->TellMsg(m_targetAd, m_targetId, Game::MsgId::OnSelecting, 0, 0) != Game::MsgResult::OK)
			continue;
		auto t = Game::GameGlobal::board->EntityAt(m_targetAd);
		if (t->GetHp() >= t->GetAttribute(AttributeType::MaxHp))
			continue;
		return true;
	}
	return false;
}
void Units::Char_128_Plosis::OnAttack() {
	Game::HealData data;
	data.healValue = 120.0f;
	Game::GameGlobal::board->TellMsg(m_targetAd, m_targetId, Game::MsgId::OnGetHeal, 0, (intptr_t)&data);
}

Game::MsgResultType Units::Char_128_Plosis::ReceiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	if (msg == Game::MsgId::OnHpChanged) {
		printf_s("HP: %f\n", m_hp);
	}
	return DefTowerProc(msg, wparam, lparam);
}
