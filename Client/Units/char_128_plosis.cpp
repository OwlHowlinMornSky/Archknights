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
#include "../Main/MsgId.h"
#include "../Game/AtkDmgHeal.h"

Unit::Char_128_Plosis::Char_128_Plosis() {}

Unit::Char_128_Plosis::~Char_128_Plosis() {}

void Unit::Char_128_Plosis::onJoined() {
	Parent::onJoined();

	Physics::Rows rows{};
	rows.offset = -1;
	rows.length = 4;
	uint32_t wd[4] = { 2, 2, 2, 2 };
	rows.widths = wd;

	m_detector = Game::Global::board->m_world->createDetectorRows(Physics::ArmyStand, m_position[0], m_position[1], &rows);
	m_detector->SetId(m_id);
	m_detector->SetLocation(m_location);

	setAttributeOringalValue(AttributeType::MaxHp, 1402.0f);

	m_m.setTarget(m_location, m_id, AttributeType::MaxHp);
	m_m.setValue(123.0f, 1.0f, 321.0f, 0.5f);
	m_m.setEnabled(true);

	m_addDef.setTarget(m_location, m_id, AttributeType::Def);
	m_addDef.setValue(320.0f, 0.0f, 0.0f, 0.0f);
	m_addDef.setEnabled(true);

	m_addMsgDef.setTarget(m_location, m_id, AttributeType::MagDef);
	m_addMsgDef.setValue(1.0f, 0.0f, 0.0f, 0.0f);
	m_addMsgDef.setEnabled(true);

	m_hp = m_attributes[AttributeType::MaxHp].effective;

	printf_s("HP: %f\n", m_hp);
	printf_s("MaxHP: %f\n", m_attributes[AttributeType::MaxHp].effective);
	printf_s("Def: %f\n", m_attributes[AttributeType::Def].effective);
	printf_s("MDef: %f\n", m_attributes[AttributeType::MagDef].effective);

	m_abilities[AbilityType::Attack].setOriginal(1);
}

void Unit::Char_128_Plosis::onKicking() {
	m_addMsgDef.setEnabled(false);
	m_addDef.setEnabled(false);
	m_m.setEnabled(false);

	Parent::onKicking();
}

void Unit::Char_128_Plosis::fixedUpdate() {
	//switch (m_status) {
	//default:
	return Parent::fixedUpdate();
	//}
}

bool Unit::Char_128_Plosis::tryToAttack() {
	for (auto it = m_detector->listBegin(), n = m_detector->listEnd(); it != n; ++it) {
		m_targetAd = it->second.location;
		m_targetId = it->first;
		if (Game::Global::board->tellMsg(m_targetAd, m_targetId, Main::MsgId::OnSelecting, 0, 0) != Game::MsgResult::OK)
			continue;
		auto t = Game::Global::board->getEntityAt(m_targetAd);
		if (t->getHp() >= t->getAttribute(AttributeType::MaxHp))
			continue;
		return false;
	}
	return true;
}

bool Unit::Char_128_Plosis::isStillCanAttack() {
	if (Game::Global::board->tellMsg(m_targetAd, m_targetId, Main::MsgId::OnSelecting, 0, 0) == Game::MsgResult::OK) {
		auto t = Game::Global::board->getEntityAt(m_targetAd);
		if (t->getHp() < t->getAttribute(AttributeType::MaxHp))
			return true;
	}
	for (auto it = m_detector->listBegin(), n = m_detector->listEnd(); it != n; ++it) {
		m_targetAd = it->second.location;
		m_targetId = it->first;
		if (Game::Global::board->tellMsg(m_targetAd, m_targetId, Main::MsgId::OnSelecting, 0, 0) != Game::MsgResult::OK)
			continue;
		auto t = Game::Global::board->getEntityAt(m_targetAd);
		if (t->getHp() >= t->getAttribute(AttributeType::MaxHp))
			continue;
		return true;
	}
	return false;
}
void Unit::Char_128_Plosis::onAttack() {
	Game::HealData data;
	data.healValue = 120.0f;
	Game::Global::board->tellMsg(m_targetAd, m_targetId, Game::MsgId::OnGetHeal, 0, (intptr_t)&data);
}

Game::MsgResultType Unit::Char_128_Plosis::receiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	if (msg == Game::MsgId::OnHpChanged) {
		printf_s("HP: %f\n", m_hp);
	}
	return DefTowerProc(msg, wparam, lparam);
}
