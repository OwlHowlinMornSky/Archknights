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
#include "../Main/MsgId.h"
#include "../Game/Global.h"
#include "../Game/Board.h"
#include "../Game/AtkDmgHeal.h"

Unit::Char_151_Myrtle::Char_151_Myrtle() {
	m_blockTotal = 8;
}

Unit::Char_151_Myrtle::~Char_151_Myrtle() {}

void Unit::Char_151_Myrtle::onJoined() {
	Parent::onJoined();

	Physics::Rows rows{};
	rows.length = 2;
	uint32_t wd[2] = { 1, 1 };
	rows.widths = wd;

	m_detector = Game::Global::board->m_world->createDetectorRows(Physics::EnemyStand, m_position[0], m_position[1], &rows);
	m_detector->setHolder(m_myself);
	m_detector->setId(m_id);
	m_hp = 1.0f;

	m_blocker = Game::Global::board->m_world->createBlockerCircle(m_position[0], m_position[1], 0.60710678118654752440084436210485f);
	m_blocker->setHolder(m_myself);
	m_blocker->setId(m_id);

	m_abilities[AbilityType::Attack].setOriginal(1);

	setAttributeOringalValue(AttributeType::Atk, 420.0f);

	printf_s("HP: %f\n", m_hp);
	printf_s("MaxHP: %f\n", m_attributes[AttributeType::MaxHp].effective);
	printf_s("Def: %f\n", m_attributes[AttributeType::Def].effective);
	printf_s("MDef: %f\n", m_attributes[AttributeType::MagDef].effective);
}

void Unit::Char_151_Myrtle::onKicking() {
	m_blocker.reset();

	for (auto& i : m_blockedMovers) {
		auto t = i.second.lock();
		if (t == nullptr)
			continue;
		t->EntityProc(Main::MsgId::BlockCleared, 0, 0);
	}

	Parent::onKicking();
}

bool Unit::Char_151_Myrtle::fixedUpdate() {
	if (m_blockTotal > m_blockedMovers.size()) {
		for (
			auto it = m_blocker->listBegin(), n = m_blocker->listEnd();
			m_blockTotal > m_blockedMovers.size() && it != n;
			++it
			) {
			if (m_blockedMovers.contains(it->first))
				continue;
			auto& ref = it->second.entity;
			auto target = ref.lock();
			if (target == nullptr)
				continue;
			if (target->EntityProc(Main::MsgId::OnBlocking, 0, 0) != Game::MsgResult::OK)
				continue;
			m_blockedMovers.emplace(it->first, ref);
			target->EntityProc(Main::MsgId::Blocked, m_id, (Game::MsgLparamType)&m_myself);
		}
	}
	return Parent::fixedUpdate();
}

Game::MsgResultType Unit::Char_151_Myrtle::receiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	switch (msg) {
	case Game::MsgId::OnHpChanged:
		printf_s("HP: %f\n", m_hp);
		return DefTowerProc(msg, wparam, lparam);
		break;
	case Main::MsgId::CancelBlock:
		m_blockedMovers.erase((Game::EntityIdType)wparam);
		break;
	default:
		return DefTowerProc(msg, wparam, lparam);
	}
	return Game::MsgResult::OK;
}

bool Unit::Char_151_Myrtle::tryToAttack() {
	if (!m_blockedMovers.empty()) {
		bool isLeft = m_blockedMovers.begin()->second.lock()->getPosition()[0] < m_position[0];
		bool isBack = m_blockedMovers.begin()->second.lock()->getPosition()[1] > m_position[1];
		setStatusToAttack(
			isLeft ?
			(isBack ? Game::IActor::Direction::BL : Game::IActor::Direction::FL) :
			(isBack ? Game::IActor::Direction::BR : Game::IActor::Direction::FR)
		);
		return true;
	}
	for (auto it = m_detector->listBegin(), n = m_detector->listEnd(); it != n; ++it) {
		auto& ref = it->second.entity;
		auto target = ref.lock();
		if (target == nullptr)
			continue;
		if (target.get() == (Entity*)this)
			continue;
		if (target->EntityProc(Main::MsgId::OnSelecting, 0, 0) != Game::MsgResult::OK)
			continue;
		bool isLeft = target->getPosition()[0] < m_position[0];
		bool isBack = target->getPosition()[1] > m_position[1];
		setStatusToAttack(
			isLeft ?
			(isBack ? Game::IActor::Direction::BL : Game::IActor::Direction::FL) :
			(isBack ? Game::IActor::Direction::BR : Game::IActor::Direction::FR)
		);
		return true;
	}
	m_target.reset();
	return false;
}

bool Unit::Char_151_Myrtle::isStillCanAttack() {
	//if (Game::GameGlobal::board->TellMsg(m_targetAd, m_targetId, Game::MsgId::OnSelecting, 0, 0) == Game::MsgResult::OK)
	//	return true;
	if (!m_blockedMovers.empty()) {
		m_target = m_blockedMovers.begin()->second;
		return true;
	}
	for (auto it = m_detector->listBegin(), n = m_detector->listEnd(); it != n; ++it) {
		auto& ref = it->second.entity;
		auto target = ref.lock();
		if (target == nullptr)
			continue;
		if (target.get() == (Entity*)this)
			continue;
		m_target = ref;
		if (target->EntityProc(Main::MsgId::OnSelecting, 0, 0) != Game::MsgResult::OK)
			continue;
		return true;
	}
	m_target.reset();
	return false;
}

void Unit::Char_151_Myrtle::onAttack() {
	if (m_target.expired())
		return;
	Game::AttackData data;
	data.source = m_myself;
	data.distType = data.Near;
	data.damage.type = data.damage.Normal;
	data.damage.dmgValue = m_attributes[AttributeType::Atk].effective;
	data.damage.minValue = 0.05f;

	auto target = m_target.lock();
	target->EntityProc(Game::MsgId::OnGetAttack, 0, (intptr_t)&data);

}
