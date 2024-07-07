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
#include "Mover.h"

#include "MsgId.h"
#include "../Game/MsgResult.h"
#include "../Game/Global.h"
#include "../Game/Board.h"
#include "HostMsgId.h"

namespace Main {

Mover::Mover() :
	m_active(false),
	m_died(false),
	m_atked(false),
	m_isBlocked(false),
	m_status(Status::Default),
	m_blockerAd(0),
	m_blockerId(0),
	m_checkpointTarget(0),
	m_moveTargetPos{},
	m_tempMoveTarget(false) {}

Mover::~Mover() {}

void Mover::onJoined() {
	if (m_actor) {
		m_actor->m_note = &m_note;
	}
	m_checkpointTarget = 0;
	// 创建主体
	m_body = Game::Global::board->m_world->createBodyMoverCircle(m_position[0], m_position[1], Physics::EnemyStand);
	m_body->SetId(m_id);
	m_body->SetLocation(m_location);
	// 触发动画
	setStatusToStart();
	m_active = true;
	m_died = false;
}

void Mover::onKicking() {
	m_detector.reset();
	m_body.reset();
	if (m_actor)
		m_actor->setInOutEffect(false);
	m_actor.reset();
}

void Mover::fixedUpdate() {
	static int test = 0;
	switch (m_status) {
	case Status::Begin:
		if (m_note.StartOver) {
			m_active = true;
			setStatusToIdle();
		}
		break;
	case Status::Idle:
	{
		float v[2];
		m_body->getPositionVelocity(m_position, v);
		onPositionVaried();
		//m_body->clearSpeed();
		if (m_abilities[AbilityType::Attack].isAbled()) {
			if (!tryToAttack()) {
				m_atked = false;
				setStatusToAttack();
				break;
			}
		}
		break;
	}
	case Status::Attaking:
		if (m_note.AttackOver) {
			if (!tryToAttack()) {
				m_atked = false;
				setStatusToAttack();
			}
			else {
				setStatusToIdle();
			}
		}
		else {
			int cnt = m_note.OnAttack;
			while (cnt--) {
				onAttackBegin();
				m_atked = true;
			}
			m_note.OnAttack = 0;
		}
		break;
	case Status::Dying:
		float velocity[2];
		m_body->getPositionVelocity(m_position, velocity);
		onPositionVaried();
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
	case Status::Moving:
	{
		float velocity[2];
		m_body->getPositionVelocity(m_position, velocity);
		onPositionVaried();
		if (m_isBlocked) {
			setStatusToIdle();
			break;
		}
		float mx = m_position[0] - m_moveTargetPos[0];
		float my = m_position[1] - m_moveTargetPos[1];
		if ((m_tempMoveTarget && std::abs(mx) < 0.5f && std::abs(my) < 0.5f) // 到达临时目标
			|| (velocity[0] * velocity[0] + velocity[1] * velocity[1] < 0.0001f) // 卡住了
			) {
			if (tryToMove()) {
				m_body->moveTo(m_moveTargetPos[0], m_moveTargetPos[1]);
				m_actor->turnLeftRight(m_moveTargetPos[0] < m_position[0]);
			}
			else { // 不再继续
				m_body->clearSpeed();
				setStatusToIdle();
			}
		}
		else if (mx * mx + my * my < 0.0225f) { // 到达非临时目标
			if (tryToTakeNextMoveCmd() && tryToMove()) {
				m_body->moveTo(m_moveTargetPos[0], m_moveTargetPos[1]);
				m_actor->turnLeftRight(m_moveTargetPos[0] < m_position[0]);
			}
			else {
				m_body->clearSpeed();
				setStatusToIdle();
			}
		}
		else {
			m_body->moveTo(m_moveTargetPos[0], m_moveTargetPos[1]);
			m_actor->turnLeftRight(m_moveTargetPos[0] < m_position[0]);
		}
		break;
	}
	case Status::Unbalance:
	{
		float velocity[2];
		m_body->getPositionVelocity(m_position, velocity);
		onPositionVaried();
		if (velocity[0] * velocity[0] + velocity[1] * velocity[1] < 0.01f) {
			if (tryToMove()) {
				m_body->moveTo(m_moveTargetPos[0], m_moveTargetPos[1]);
				m_actor->turnLeftRight(m_moveTargetPos[0] < m_position[0]);
			}
			else { // 不再继续
				m_body->clearSpeed();
				setStatusToIdle();
			}
		}
		break;
	}
	default:
		break;
	}
}

Game::MsgResultType Mover::receiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	return DefMoverProc(msg, wparam, lparam);
}

void Mover::onPositionChanged() {
	if (m_body)
		m_body->setPosition(m_position[0], m_position[1]);
	return onPositionVaried();
}

void Mover::onPositionVaried() {
	if (m_actor)
		m_actor->setPosition(m_position[0], m_position[1], 0.0f);
	if (m_detector)
		m_detector->setPosition(m_position[0], m_position[1]);
}

Game::MsgResultType Mover::DefMoverProc(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	switch (msg) {
	case Game::MsgId::OnHpDropToZero:
		if (m_status == Status::Moving || m_status == Status::Unbalance) {
			m_body->setStatusUnbalance();
		}
		if (m_isBlocked) {
			m_isBlocked = false;
			Game::Global::board->tellMsg(m_blockerAd, m_blockerId, Main::MsgId::CancelBlock, m_id, 0);
			m_blockerId = 0;
			m_blockerAd = 0;
		}
		setStatusToDying();
		m_died = true;
		m_detector.reset();
		break;
	case Game::MsgId::OnGetAttack:
		if (m_died)
			break;
		if (m_actor)
			m_actor->setHitEffect();
		return DefEntityProc(msg, wparam, lparam);
	case Main::MsgId::OnSelecting:
		if (!m_active || m_died) {
			return Game::MsgResult::MethodNotAllowed;
		}
		break;
	case Main::MsgId::OnBlocking:
		if (!m_active || m_died || m_isBlocked) {
			return Game::MsgResult::MethodNotAllowed;
		}
		break;
	case Main::MsgId::Blocked:
		m_blockerId = wparam;
		m_blockerAd = lparam;
		m_isBlocked = true;
		m_body->clearSpeed();
		break;
	case Main::MsgId::BlockCleared:
		m_blockerId = 0;
		m_blockerAd = 0;
		m_isBlocked = false;
		setStatusToIdle();
		break;
	default:
		return DefEntityProc(msg, wparam, lparam);
	}
	return Game::MsgResult::OK;
}

void Mover::setStatusToStart(Game::IActor::Direction d) {
	if (m_actor) {
		m_actor->setStatus(
			Game::IActor::AnimationStatus::Normal
		);
		m_actor->setInOutEffect(true);
	}
	if (tryToMove()) {
		m_actor->setDirection(
			m_moveTargetPos[0] < m_position[0] ? Game::IActor::Direction::FL : Game::IActor::Direction::FR
		);
		m_body->moveTo(m_moveTargetPos[0], m_moveTargetPos[1]);
		setStatusToMoving(Game::IActor::Direction::NotCare);
	}
	else {
		setStatusToIdle(d);
	}
}

void Mover::setStatusToBegin(Game::IActor::Direction d) {
	m_status = Status::Begin;
	if (m_actor)
		m_actor->triggerAnimation(
			Game::IActor::AnimationEvent::Begin, d
		);
}

void Mover::setStatusToIdle(Game::IActor::Direction d) {
	if (!m_isBlocked && tryToMove()) {
		m_body->moveTo(m_moveTargetPos[0], m_moveTargetPos[1]);
		setStatusToMoving(
			m_moveTargetPos[0] < m_position[0] ? Game::IActor::Direction::FL : Game::IActor::Direction::FR
		);
		return;
	}
	m_body->clearSpeed();
	m_status = Status::Idle;
	if (m_actor)
		m_actor->triggerAnimation(
			Game::IActor::AnimationEvent::Idle, d
		);
}

void Mover::setStatusToAttack(Game::IActor::Direction d) {
	m_status = Status::Attaking;
	if (m_actor)
		m_actor->triggerAnimation(
			Game::IActor::AnimationEvent::Attack, d
		);
}

void Mover::setStatusToStun(Game::IActor::Direction d) {
	m_status = Status::Stun;
	if (m_actor)
		m_actor->triggerAnimation(
			Game::IActor::AnimationEvent::Stun, d
		);
}

void Mover::setStatusToDying(Game::IActor::Direction d) {
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

void Mover::setStatusToReturn(Game::IActor::Direction d) {
	m_status = Status::Returning;
	if (m_actor)
		m_actor->triggerAnimation(
			Game::IActor::AnimationEvent::Die, d
		);
}

void Mover::setStatusToMoving(Game::IActor::Direction d) {
	m_status = Status::Moving;
	if (m_actor)
		m_actor->triggerAnimation(
			Game::IActor::AnimationEvent::Move, d
		);
}

void Mover::setStatusToUnbalance(Game::IActor::Direction d) {
	m_status = Status::Unbalance;
	if (m_actor)
		m_actor->triggerAnimation(
			Game::IActor::AnimationEvent::Default, d
		);
}

bool Mover::tryToAttack() {
	return true;
}

bool Mover::isStillCanAttack() {
	return false;
}

void Mover::onAttackBegin() {
	isStillCanAttack();
	receiveMessage(Game::MsgId::OnAttackBegin, 0, 0);
	onAttack();
	receiveMessage(Game::MsgId::OnAttackEnd, 0, 0);
}

void Mover::onAttack() {}

bool Mover::tryToMove() {
	if (m_position[0] < 0.0f || m_position[1] < 0.0f)
		return false;
	RetryQuery:
	int target[2] = { (int)m_position[0], (int)m_position[1] };
	Game::MsgResultType res =
		Game::Global::board->
		getHost(Game::HostJob::MapManager)->
		receiveMessage(Main::HostMsgId::MapLeadQuery, m_checkpointTarget, (intptr_t)target);
	switch (res) {
	case Game::MsgResult::Leader_TempRes:
		m_tempMoveTarget = true;
		break;
	case Game::MsgResult::Leader_FinalRes:
		m_tempMoveTarget = false;
		break;
	case Game::MsgResult::Leader_AlreadyReached:
	{
		float mx = m_position[0] - m_moveTargetPos[0];
		float my = m_position[1] - m_moveTargetPos[1];
		if (mx * mx + my * my < 0.0225f) {
			tryToTakeNextMoveCmd();
			goto RetryQuery;
		}
		break;
	}
	case Game::MsgResult::Leader_AtInvalidBlock:
		return false;
	case Game::MsgResult::Leader_NoAvailablePath:
		return false;
	default:
		return false;
	}
	m_moveTargetPos[0] = target[0] + 0.5f;
	m_moveTargetPos[1] = target[1] + 0.5f;
	return true;
}

bool Mover::tryToTakeNextMoveCmd() {
	if (m_checkpointTarget > 4)
		return false;
	m_checkpointTarget++;
	return true;
}

}
