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
#include "Tower.h"

#include "MsgId.h"
#include "../Game/MsgResult.h"
#include "HostMsgId.h"

#include "TryToOccupyData.h"
#include "SetOccupationData.h"

namespace Main {

Tower::Tower() :
	m_active(false),
	m_died(false),
	m_atked(false),
	m_defaultDirection(Game::IActor::Direction::FR),
	m_status(Status::Default) {}

Tower::~Tower() {}

void Tower::onJoined() {
	TryToOccupyData data{};
	if (m_occupiedPlace.status == OccupiedPlace::HitNull ||
		m_occupiedPlace.subId == -1 ||
		Game::Global::board->
		getHost(Game::HostJob::MapManager)->
		receiveMessage(
			HostMsgId::TryToOccupy,
			m_occupiedPlace.subId,
			(Game::MsgLparamType)&data
		) != Game::MsgResult::OK) {
		m_actor->setColor(0.0f, 0.0f, 0.0f, 0.0f);
		kickSelf();
		return;
	}
	SetOccupationData occData;
	occData.entity = m_myself;
	Game::Global::board->
		getHost(Game::HostJob::MapManager)->
		receiveMessage(
			HostMsgId::SetOccupationAt,
			m_occupiedPlace.subId,
			(Game::MsgLparamType)&occData
		);
	if (m_actor) {
		m_actor->m_note = &m_note;
	}
	m_position[0] = data.position[0];
	m_position[1] = data.position[1];
	m_actor->setPosZ(data.position[2]);
	onPositionChanged();
	// 触发动画
	setStatusToStart(m_defaultDirection);
	// 创建主体
	m_body = Game::Global::board->m_world->createBodyTowerCircle(m_position[0], m_position[1], Physics::ArmyStand);
	m_body->setHolder(m_myself);
	m_body->setId(m_id);
	m_active = false;
	m_died = false;
}

void Tower::onKicking() {
	m_active = false;
	m_died = true;
	m_detector.reset();
	m_body.reset();
	if (m_actor)
		if (m_active)
			m_actor->setInOutEffect(false);
		else
			m_actor->setWaitingForQuit();
	m_actor.reset();
}

bool Tower::fixedUpdate() {
	switch (m_status) {
	case Status::Begin:
		if (m_note.StartOver) {
			m_active = true;
			setStatusToIdle();
		}
		break;
	case Status::Idle:
		if (m_abilities[AbilityType::Attack].isAbled()) {
			if (tryToAttack()) {
				m_atked = false;
			}
		}
		break;
	case Status::Attaking:
		if (m_note.AttackOver) {
			if (tryToAttack()) {
				m_atked = false;
			}
			else {
				setStatusToIdle();
			}
		}
		{
			int cnt = m_note.OnAttack;
			while (cnt--) {
				onAttackBegin();
				m_atked = true;
			}
			m_note.OnAttack = 0;
		}
		break;
	case Status::Dying:
		if (m_note.DieOver) {
			kickSelf();
		}
		break;
	case Status::Returning:
		if (m_note.DieOver) {
			m_actor->setStatus(Game::IActor::AnimationStatus::Normal);
			setStatusToIdle();
		}
		break;
	default:
		break;
	}
	return m_isNotWaitingForExit;
}

Game::MsgResultType Tower::receiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	return DefTowerProc(msg, wparam, lparam);
}

void Tower::onPositionChanged() {
	if (m_actor)
		m_actor->setXY(m_position[0], m_position[1]);
	if (m_body)
		m_body->setPosition(m_position[0], m_position[1]);
	if (m_detector)
		m_detector->setPosition(m_position[0], m_position[1]);
}

Game::MsgResultType Tower::DefTowerProc(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	switch (msg) {
	case Game::MsgId::OnHpDropToZero:
		setStatusToDying(m_defaultDirection);
		m_died = true;
		m_body.reset();
		m_detector.reset();
		Game::Global::board->
			getHost(Game::HostJob::MapManager)->
			receiveMessage(
				HostMsgId::ClearOccupation,
				m_occupiedPlace.subId,
				0
			);
		break;
	case Game::MsgId::OnGetAttack:
		if (m_died)
			break;
		if (m_actor)
			m_actor->setHitEffect();
		return DefEntityProc(msg, wparam, lparam);
	case Main::MsgId::OnSelecting:
		if (!m_active || m_died)
			return Game::MsgResult::MethodNotAllowed;
		break;
	case Main::MsgId::SetOccupiedPlace:
	{
		OccupiedPlace* place = (OccupiedPlace*)lparam;
		m_occupiedPlace = *place;
		break;
	}
	case Main::MsgId::Retreat:
		m_active = false;
		kickSelf();
		// Not Implemented.
		break;
	case Main::MsgId::UserRetreat:
		switch (m_occupiedPlace.status) {
		case Main::OccupiedPlace::HitNull:
			break;
		case Main::OccupiedPlace::HitGround:
			Game::Global::board->
				getHost(Game::HostJob::MapManager)->
				receiveMessage(
					HostMsgId::ClearOccupation,
					m_occupiedPlace.subId,
					0
				);
			receiveMessage(Main::MsgId::Retreat, 0, 0);
			break;
		case Main::OccupiedPlace::HitExtra:
			// Not Implemented.
			receiveMessage(Main::MsgId::Retreat, 0, 0);
			break;
		default:
			break;
		}
		break;
	default:
		return DefEntityProc(msg, wparam, lparam);
	}
	return Game::MsgResult::OK;
}

void Tower::setStatusToStart(Game::IActor::Direction d) {
	if (m_actor)
		m_actor->setStatus(
			Game::IActor::AnimationStatus::Normal
		);
	m_actor->setInOutEffect(true, true);
	setStatusToBegin(d);
}

void Tower::setStatusToBegin(Game::IActor::Direction d) {
	m_status = Status::Begin;
	if (m_actor)
		m_actor->triggerAnimation(
			Game::IActor::AnimationEvent::Begin, d
		);
}

void Tower::setStatusToIdle(Game::IActor::Direction d) {
	m_status = Status::Idle;
	if (m_actor)
		m_actor->triggerAnimation(
			Game::IActor::AnimationEvent::Idle, m_defaultDirection
		);
}

void Tower::setStatusToAttack(Game::IActor::Direction d) {
	m_status = Status::Attaking;
	if (m_actor)
		m_actor->triggerAnimation(
			Game::IActor::AnimationEvent::Attack, d
		);
}

void Tower::setStatusToStun(Game::IActor::Direction d) {
	m_status = Status::Stun;
	if (m_actor)
		m_actor->triggerAnimation(
			Game::IActor::AnimationEvent::Stun, d
		);
}

void Tower::setStatusToDying(Game::IActor::Direction d) {
	m_status = Status::Dying;
	if (m_actor) {
		m_actor->setStatus(
			Game::IActor::AnimationStatus::Normal
		);
		m_actor->triggerAnimation(
			Game::IActor::AnimationEvent::Die, d
		);
	}
}

void Tower::setStatusToReturn(Game::IActor::Direction d) {
	m_status = Status::Returning;
	if (m_actor)
		m_actor->triggerAnimation(
			Game::IActor::AnimationEvent::Die, d
		);
}

bool Tower::tryToAttack() {
	return false;
}

bool Tower::isStillCanAttack() {
	return false;
}

void Tower::onAttackBegin() {
	if (!isStillCanAttack()) {
		setStatusToIdle();
		return;
	}
	receiveMessage(Game::MsgId::OnAttackBegin, 0, 0);
	onAttack();
	receiveMessage(Game::MsgId::OnAttackEnd, 0, 0);
}

void Tower::onAttack() {}

}
