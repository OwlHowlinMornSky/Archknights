﻿/*
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
	m_status(Status::Default) {}

Mover::~Mover() {}

void Mover::OnJoined() {
	if (m_actor) {
		m_actor->m_note = &m_note;
	}
	m_checkpointTarget = 0;
	// 创建主体
	m_body = Game::Global::board->m_world->CreateBodyMoverCircle(m_position[0], m_position[1], Physics::EnemyStand);
	m_body->SetId(m_id);
	m_body->SetLocation(m_location);
	// 触发动画
	ToStart();
	m_active = true;
	m_died = false;
}

void Mover::OnKicking() {
	m_detector.reset();
	m_body.reset();
	if (m_actor)
		m_actor->SetInOut(false);
	m_actor.reset();
}

void Mover::FixedUpdate() {
	switch (m_status) {
	case Status::Begin:
		if (m_note.StartOver) {
			m_active = true;
			ToIdle();
		}
		break;
	case Status::Idle:
		if (abilities[AbilityType::Attack].IsAbled()) {
			if (!TryAttack()) {
				m_atked = false;
				ToAttack();
				break;
			}
		}
		break;
	case Status::Attaking:
		if (m_note.AttackOver) {
			if (!TryAttack()) {
				m_atked = false;
				ToAttack();
			}
			else {
				ToIdle();
			}
		}
		else {
			int cnt = m_note.OnAttack;
			while (cnt--) {
				BasicOnAttack();
				m_atked = true;
			}
			m_note.OnAttack = 0;
		}
		break;
	case Status::Dying:
		float velocity[2];
		m_body->GetPositionVelocity(m_position, velocity);
		OnPositionChanged();
		if (m_note.DieOver) {
			KickSelf();
		}
		break;
	case Status::Returning:
		if (m_note.DieOver) {
			m_actor->ChangeStatus(Game::IActor::AnimationStatus::Normal);
			ToIdle();
		}
		break;
	case Status::Moving:
	{
		/*float velocity[2];
		m_body->GetPositionVelocity(m_position, velocity);
		OnPositionChanged(); // 内部有多余的设置body位置，考虑更改。
		float mx = m_position[0] - m_moveTargetPos[0];
		float my = m_position[1] - m_moveTargetPos[1];
		if ((m_tempMoveTarget && std::abs(mx) < 0.5f && std::abs(my) < 0.5f) // 到达临时目标
			|| (velocity[0] * velocity[0] + velocity[1] * velocity[1] < 0.0001f) // 卡住了
			) {
			if (TryMove()) {
				m_body->MoveTo(m_moveTargetPos[0], m_moveTargetPos[1]);
				m_actor->TurnDirection(m_moveTargetPos[0] < m_position[0]);
			}
			else { // 不再继续
				m_body->ClearSpeed();
				ToIdle();
			}
		}
		else if (mx * mx + my * my < 0.0225f) { // 到达非临时目标
			if (TryTakeNextMoveCmd() && TryMove()) {
				m_body->MoveTo(m_moveTargetPos[0], m_moveTargetPos[1]);
				m_actor->TurnDirection(m_moveTargetPos[0] < m_position[0]);
			}
			else {
				m_body->ClearSpeed();
				ToIdle();
			}
		}
		else {
			m_body->MoveTo(m_moveTargetPos[0], m_moveTargetPos[1]);
			m_actor->TurnDirection(m_moveTargetPos[0] < m_position[0]);
		}*/
		float dx = m_moveTargetPos[0] - m_position[0];
		float dy = m_moveTargetPos[1] - m_position[1];
		if (m_tempMoveTarget && std::abs(dx) < 0.3f && std::abs(dy) < 0.3f) { // 到达临时目标
			if (TryMove()) {
				dx = m_moveTargetPos[0] - m_position[0];
				dy = m_moveTargetPos[1] - m_position[1];
				float l = std::sqrtf(dx * dx + dy * dy);
				float spd = attributes[AttributeType::MoveSpd].effective;
				if (l <= spd) {
					m_position[0] = m_moveTargetPos[0];
					m_position[1] = m_moveTargetPos[1];
					OnPositionChanged();
				}
				else {
					m_position[0] += dx * spd / l;
					m_position[1] += dy * spd / l;
					OnPositionChanged();
					m_actor->TurnDirection(m_moveTargetPos[0] < m_position[0]);
				}
			}
			else { // 不再继续
				m_body->ClearSpeed();
				ToIdle();
			}
		}
		else if (dx * dx + dy * dy < 0.0225f) { // 到达非临时目标
			if (TryTakeNextMoveCmd() && TryMove()) {
				dx = m_moveTargetPos[0] - m_position[0];
				dy = m_moveTargetPos[1] - m_position[1];
				float l = std::sqrtf(dx * dx + dy * dy);
				float spd = attributes[AttributeType::MoveSpd].effective;
				if (l <= spd) {
					m_position[0] = m_moveTargetPos[0];
					m_position[1] = m_moveTargetPos[1];
					OnPositionChanged();
				}
				else {
					m_position[0] += dx * spd / l;
					m_position[1] += dy * spd / l;
					OnPositionChanged();
					m_actor->TurnDirection(m_moveTargetPos[0] < m_position[0]);
				}
			}
			else {
				m_body->ClearSpeed();
				ToIdle();
			}
		}
		else {
			float l = std::sqrtf(dx * dx + dy * dy);
			float spd = attributes[AttributeType::MoveSpd].effective;
			if (l <= spd) {
				m_position[0] = m_moveTargetPos[0];
				m_position[1] = m_moveTargetPos[1];
				OnPositionChanged();
			}
			else {
				m_position[0] += dx * spd / l;
				m_position[1] += dy * spd / l;
				OnPositionChanged();
				m_actor->TurnDirection(m_moveTargetPos[0] < m_position[0]);
			}
		}
		break;
	}
	case Status::Unbalance:
	{
		float velocity[2];
		m_body->GetPositionVelocity(m_position, velocity);
		OnPositionChanged();
		if (velocity[0] * velocity[0] + velocity[1] * velocity[1] < 0.01f) {
			if (TryMove()) {
				m_body->MoveTo(m_moveTargetPos[0], m_moveTargetPos[1]);
				m_actor->TurnDirection(m_moveTargetPos[0] < m_position[0]);
			}
			else { // 不再继续
				m_body->ClearSpeed();
				ToIdle();
			}
		}
		break;
	}
	default:
		break;
	}
}

Game::MsgResultType Mover::ReceiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	return DefMoverProc(msg, wparam, lparam);
}

void Mover::OnPositionChanged() {
	if (m_actor)
		m_actor->SetPosition(m_position[0], m_position[1], 0.0f);
	if (m_body)
		m_body->SetPosition(m_position[0], m_position[1]);
	if (m_detector)
		m_detector->SetPosition(m_position[0], m_position[1]);
}

Game::MsgResultType Mover::DefMoverProc(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	switch (msg) {
	case Game::MsgId::OnHpDropToZero:
		if (m_status == Status::Moving || m_status == Status::Unbalance) {
			m_body->BeginUnbalance();
			float dx = m_moveTargetPos[0] - m_position[0];
			float dy = m_moveTargetPos[1] - m_position[1];
			float l = std::sqrtf(dx * dx + dy * dy);
			float spd = attributes[AttributeType::MoveSpd].effective * 30.0f;
			m_body->SetVelocity(dx * spd / l, dy * spd / l);
		}
		ToDying();
		m_died = true;
		m_detector.reset();
		break;
	case Game::MsgId::OnGetAttack:
		if (m_died)
			break;
		if (m_actor)
			m_actor->SetHit();
		return DefEntityProc(msg, wparam, lparam);
	case Main::MsgId::OnSelecting:
		if (!m_active || m_died) {
			return Game::MsgResult::MethodNotAllowed;
		}
		break;
	default:
		return DefEntityProc(msg, wparam, lparam);
	}
	return Game::MsgResult::OK;
}

void Mover::ToStart(Game::IActor::Direction d) {
	if (m_actor) {
		m_actor->ChangeStatus(
			Game::IActor::AnimationStatus::Normal
		);
		m_actor->SetInOut(true);
	}
	if (TryMove()) {
		m_actor->InitDirection(
			m_moveTargetPos[0] < m_position[0] ? Game::IActor::Direction::FL : Game::IActor::Direction::FR
		);
		//m_body->MoveTo(m_moveTargetPos[0], m_moveTargetPos[1]);
		ToMoving(Game::IActor::Direction::NotCare);
	}
	else {
		ToIdle(d);
	}
}

void Mover::ToBegin(Game::IActor::Direction d) {
	m_status = Status::Begin;
	if (m_actor)
		m_actor->TriggerAnimation(
			Game::IActor::AnimationEvent::Begin, d
		);
}

void Mover::ToIdle(Game::IActor::Direction d) {
	if (TryMove()) {
		//m_body->MoveTo(m_moveTargetPos[0], m_moveTargetPos[1]);
		ToMoving(
			m_moveTargetPos[0] < m_position[0] ? Game::IActor::Direction::FL : Game::IActor::Direction::FR
		);
		return;
	}
	m_status = Status::Idle;
	if (m_actor)
		m_actor->TriggerAnimation(
			Game::IActor::AnimationEvent::Idle, d
		);
}

void Mover::ToAttack(Game::IActor::Direction d) {
	m_status = Status::Attaking;
	if (m_actor)
		m_actor->TriggerAnimation(
			Game::IActor::AnimationEvent::Attack, d
		);
}

void Mover::ToStun(Game::IActor::Direction d) {
	m_status = Status::Stun;
	if (m_actor)
		m_actor->TriggerAnimation(
			Game::IActor::AnimationEvent::Stun, d
		);
}

void Mover::ToDying(Game::IActor::Direction d) {
	m_status = Status::Dying;
	if (m_actor) {
		m_actor->ChangeStatus(
			Game::IActor::AnimationStatus::Normal
		);
		m_actor->TriggerAnimation(
			Game::IActor::AnimationEvent::Die, d
		);
	}
}

void Mover::ToReturn(Game::IActor::Direction d) {
	m_status = Status::Returning;
	if (m_actor)
		m_actor->TriggerAnimation(
			Game::IActor::AnimationEvent::Die, d
		);
}

void Mover::ToMoving(Game::IActor::Direction d) {
	m_status = Status::Moving;
	if (m_actor)
		m_actor->TriggerAnimation(
			Game::IActor::AnimationEvent::Move, d
		);
}

void Mover::ToUnbalance(Game::IActor::Direction d) {
	m_status = Status::Unbalance;
	if (m_actor)
		m_actor->TriggerAnimation(
			Game::IActor::AnimationEvent::Default, d
		);
}

bool Mover::TryAttack() {
	return true;
}

bool Mover::StillCanAttack() {
	return false;
}

void Mover::BasicOnAttack() {
	StillCanAttack();
	ReceiveMessage(Game::MsgId::OnAttackBegin, 0, 0);
	OnAttack();
	ReceiveMessage(Game::MsgId::OnAttackEnd, 0, 0);
}

void Mover::OnAttack() {}

bool Mover::TryMove() {
	if (m_position[0] < 0.0f || m_position[1] < 0.0f)
		return false;
	int target[2] = { (int)m_position[0], (int)m_position[1] };
	Game::MsgResultType res =
		Game::Global::board->
		GetHost(Game::HostJob::MapPathManager)->
		ReceiveMessage(Main::HostMsgId::MapLeadQuery, m_checkpointTarget, (intptr_t)target);
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
			return TryTakeNextMoveCmd();
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

bool Mover::TryTakeNextMoveCmd() {
	if (m_checkpointTarget > 0)
		return false;
	m_checkpointTarget++;
	return true;
}

}
