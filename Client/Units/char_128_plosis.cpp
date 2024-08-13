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
	m_detector->setHolder(m_myself);
	m_detector->setId(m_id);

	setAttributeOringalValue(AttributeType::MaxHp, 1402.0f);

	m_m.setTarget(m_myself, AttributeType::MaxHp);
	m_m.setValue(123.0f, 1.0f, 321.0f, 0.5f);
	m_m.setEnabled(true);

	m_addDef.setTarget(m_myself, AttributeType::Def);
	m_addDef.setValue(320.0f, 0.0f, 0.0f, 0.0f);
	m_addDef.setEnabled(true);

	m_addMsgDef.setTarget(m_myself, AttributeType::MagDef);
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

bool Unit::Char_128_Plosis::fixedUpdate() {
	//switch (m_status) {
	//default:
	return Parent::fixedUpdate();
	//}
}

bool Unit::Char_128_Plosis::tryToAttack() {
	for (auto it = m_detector->listBegin(), n = m_detector->listEnd(); it != n; ++it) {
		m_target = it->second.entity;
		auto target = m_target.lock();
		if (target == nullptr)
			continue;
		if (target->EntityProc(Main::MsgId::OnSelecting, 0, 0) != Game::MsgResult::OK)
			continue;
		if (target->getHp() >= target->getAttribute(AttributeType::MaxHp))
			continue;
		setStatusToAttack();
		return true;
	}
	return false;
}

bool Unit::Char_128_Plosis::isStillCanAttack() {
	auto target = m_target.lock();
	if (target && target->EntityProc(Main::MsgId::OnSelecting, 0, 0) == Game::MsgResult::OK) {
		if (target->getHp() < target->getAttribute(AttributeType::MaxHp))
			return true;
	}
	for (auto it = m_detector->listBegin(), n = m_detector->listEnd(); it != n; ++it) {
		m_target = it->second.entity;
		target = m_target.lock();
		if (target == nullptr)
			continue;
		if (target->EntityProc(Main::MsgId::OnSelecting, 0, 0) != Game::MsgResult::OK)
			continue;
		if (target->getHp() >= target->getAttribute(AttributeType::MaxHp))
			continue;
		return true;
	}
	return false;
}
void Unit::Char_128_Plosis::onAttack() {
	Game::HealData data;
	data.healValue = 120.0f;
	auto target = m_target.lock();
	if (target)
		target->EntityProc(Game::MsgId::OnGetHeal, 0, (intptr_t)&data);
}

Game::MsgResultType Unit::Char_128_Plosis::receiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	if (msg == Game::MsgId::OnHpChanged) {
		printf_s("HP: %f\n", m_hp);
	}
	return DefTowerProc(msg, wparam, lparam);
}
