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

Unit::Char_151_Myrtle::Char_151_Myrtle() {}

Unit::Char_151_Myrtle::~Char_151_Myrtle() {}

void Unit::Char_151_Myrtle::onJoined() {
	Parent::onJoined();

	Physics::Rows rows{};
	rows.length = 2;
	uint32_t wd[2] = { 1, 1 };
	rows.widths = wd;

	m_detector = Game::Global::board->m_world->createDetectorRows(Physics::EnemyStand, m_position[0], m_position[1], &rows);
	m_detector->SetId(m_id);
	m_detector->SetLocation(m_location);
	m_hp = 1.0f;

	m_abilities[AbilityType::Attack].setOriginal(1);

	setAttributeOringalValue(AttributeType::Atk, 420.0f);

	Game::Global::board->subscribeMsg(Game::MsgId::GuiEvent, m_location);

	printf_s("HP: %f\n", m_hp);
	printf_s("MaxHP: %f\n", m_attributes[AttributeType::MaxHp].effective);
	printf_s("Def: %f\n", m_attributes[AttributeType::Def].effective);
	printf_s("MDef: %f\n", m_attributes[AttributeType::MagDef].effective);
}

void Unit::Char_151_Myrtle::onKicking() {
	Game::Global::board->unsubscribeMsg(Game::MsgId::GuiEvent, m_location);

	Parent::onKicking();
}

void Unit::Char_151_Myrtle::fixedUpdate() {
	//switch (m_status) {
	//default:
	return Parent::fixedUpdate();
	//}
}

#include <SFML/Window/Event.hpp>

Game::MsgResultType Unit::Char_151_Myrtle::receiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	switch (msg) {
	case Game::MsgId::GuiEvent:
	{
		sf::Event& evt = *(sf::Event*)lparam;
		switch (evt.type) {
		case sf::Event::KeyReleased:
			switch (evt.key.code) {
			case sf::Keyboard::A:
				m_actor->setStatus(Game::IActor::AnimationStatus::Skill0);
				setStatusToBegin(Game::IActor::Direction::BR);
				break;
			case sf::Keyboard::Q:
				setStatusToReturn(Game::IActor::Direction::BL);
				break;
			}
			break;
		}
		break;
	}
	case Game::MsgId::OnHpChanged:
		printf_s("HP: %f\n", m_hp);
		return DefTowerProc(msg, wparam, lparam);
		break;
	default:
		return DefTowerProc(msg, wparam, lparam);
	}
	return Game::MsgResult::OK;
}

bool Unit::Char_151_Myrtle::tryToAttack() {
	for (auto it = m_detector->listBegin(), n = m_detector->listEnd(); it != n; ++it) {
		if (it->first == m_id)
			continue;
		if (Game::Global::board->tellMsg(it->second.location, it->first, Main::MsgId::OnSelecting, 0, 0) != Game::MsgResult::OK)
			continue;
		bool isLeft = Game::Global::board->getEntityAt(it->second.location)->getPosition()[0] < m_position[0];
		bool isBack = Game::Global::board->getEntityAt(it->second.location)->getPosition()[1] > m_position[1];
		setStatusToAttack(
			isLeft ?
			(isBack ? Game::IActor::Direction::BL : Game::IActor::Direction::FL) :
			(isBack ? Game::IActor::Direction::BR : Game::IActor::Direction::FR)
		);
		return false;
	}
	m_targetAd = 0;
	m_targetId = 0;
	return true;
}

bool Unit::Char_151_Myrtle::isStillCanAttack() {
	//if (Game::GameGlobal::board->TellMsg(m_targetAd, m_targetId, Game::MsgId::OnSelecting, 0, 0) == Game::MsgResult::OK)
	//	return true;
	for (auto it = m_detector->listBegin(), n = m_detector->listEnd(); it != n; ++it) {
		if (it->first == m_id)
			continue;
		m_targetAd = it->second.location;
		m_targetId = it->first;
		if (Game::Global::board->tellMsg(m_targetAd, m_targetId, Main::MsgId::OnSelecting, 0, 0) != Game::MsgResult::OK)
			continue;
		return true;
	}
	m_targetAd = 0;
	m_targetId = 0;
	return false;
}

void Unit::Char_151_Myrtle::onAttack() {
	if (m_targetId == 0)
		return;
	Game::AttackData data;
	data.sourceAd = m_location;
	data.sourceId = m_id;
	data.distType = data.Near;
	data.damage.type = data.damage.Normal;
	data.damage.dmgValue = m_attributes[AttributeType::Atk].effective;
	data.damage.minValue = 0.05f;

	Game::Global::board->tellMsg(m_targetAd, m_targetId, Game::MsgId::OnGetAttack, 0, (intptr_t)&data);

}
