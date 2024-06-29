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

namespace Main {

Tower::Tower() :
	m_active(false),
	m_died(false),
	m_atked(false),
	m_defaultDirection(Game::IActor::Direction::FR),
	m_status(Status::Default) {}

Tower::~Tower() {}

void Tower::OnJoined() {
	if (m_actor) {
		m_actor->m_note = &m_note;
	}
	// 触发动画
	ToStart(m_defaultDirection);
	// 创建主体
	m_body = Game::Global::board->m_world->CreateBodyTowerCircle(m_position[0], m_position[1], Physics::ArmyStand);
	m_body->SetId(m_id);
	m_body->SetLocation(m_location);
	m_active = false;
	m_died = false;
}

void Tower::OnKicking() {
	m_detector.reset();
	m_body.reset();
	if (m_actor)
		m_actor->SetInOut(false);
	m_actor.reset();
}

void Tower::FixedUpdate() {
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
	default:
		break;
	}
}

Game::MsgResultType Tower::ReceiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	return DefTowerProc(msg, wparam, lparam);
}

void Tower::OnPositionChanged() {
	if (m_actor)
		m_actor->SetPosition(m_position[0], m_position[1], 0.0f);
	if (m_body)
		m_body->SetPosition(m_position[0], m_position[1]);
	if (m_detector)
		m_detector->SetPosition(m_position[0], m_position[1]);
}

Game::MsgResultType Tower::DefTowerProc(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	switch (msg) {
	case Game::MsgId::OnHpDropToZero:
		ToDying(m_defaultDirection);
		m_died = true;
		m_body.reset();
		m_detector.reset();
		break;
	case Game::MsgId::OnGetAttack:
		if (m_died)
			break;
		if (m_actor)
			m_actor->SetHit();
		return DefEntityProc(msg, wparam, lparam);
	case Main::MsgId::OnSelecting:
		if (!m_active || m_died)
			return Game::MsgResult::MethodNotAllowed;
		break;
	default:
		return DefEntityProc(msg, wparam, lparam);
	}
	return Game::MsgResult::OK;
}

void Tower::ToStart(Game::IActor::Direction d) {
	if (m_actor)
		m_actor->ChangeStatus(
			Game::IActor::AnimationStatus::Normal
		);
	m_actor->SetInOut(true, true);
	ToBegin(d);
}

void Tower::ToBegin(Game::IActor::Direction d) {
	m_status = Status::Begin;
	if (m_actor)
		m_actor->TriggerAnimation(
			Game::IActor::AnimationEvent::Begin, d
		);
}

void Tower::ToIdle(Game::IActor::Direction d) {
	m_status = Status::Idle;
	if (m_actor)
		m_actor->TriggerAnimation(
			Game::IActor::AnimationEvent::Idle, d
		);
}

void Tower::ToAttack(Game::IActor::Direction d) {
	m_status = Status::Attaking;
	if (m_actor)
		m_actor->TriggerAnimation(
			Game::IActor::AnimationEvent::Attack, d
		);
}

void Tower::ToStun(Game::IActor::Direction d) {
	m_status = Status::Stun;
	if (m_actor)
		m_actor->TriggerAnimation(
			Game::IActor::AnimationEvent::Stun, d
		);
}

void Tower::ToDying(Game::IActor::Direction d) {
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

void Tower::ToReturn(Game::IActor::Direction d) {
	m_status = Status::Returning;
	if (m_actor)
		m_actor->TriggerAnimation(
			Game::IActor::AnimationEvent::Die, d
		);
}

bool Tower::TryAttack() {
	return true;
}

bool Tower::StillCanAttack() {
	return false;
}

void Tower::BasicOnAttack() {
	StillCanAttack();
	ReceiveMessage(Game::MsgId::OnAttackBegin, 0, 0);
	OnAttack();
	ReceiveMessage(Game::MsgId::OnAttackEnd, 0, 0);
}

void Tower::OnAttack() {}

}
