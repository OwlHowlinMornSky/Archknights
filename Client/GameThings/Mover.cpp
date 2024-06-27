#include "Mover.h"

#include "MsgId.h"
#include "../Game/MsgResult.h"
#include "../Game/GameGlobal.h"
#include "../Game/GameBoard.h"
#include "HostMsgId.h"

namespace Units {

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
	// 创建主体
	m_body = Game::GameGlobal::board->m_world->CreateBodyMoverCircle(m_position[0], m_position[1], Physics::EnemyStand);
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
		m_actor->Exit();
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
		float spd;
		if (m_body->MoveTo(m_moveTargetPos[0], m_moveTargetPos[1], m_position, &spd) < (m_tempMoveTarget ? 0.5f : 0.1f)) {
			OnPositionChanged();
			if (!TryMove()) {
				m_body->ClearSpeed();
				ToIdle();
				break;
			}
			m_actor->TurnDirection(m_moveTargetPos[0] < m_position[0]);
		}
		else {
			OnPositionChanged();

			if (spd < 0.01f) {
				TryMove();
				m_actor->TurnDirection(m_moveTargetPos[0] < m_position[0]);
			}
		}
		break;
	}
	case Status::Unbalance:
		if (!m_body->UpdateUnbalance(m_position)) {
			OnPositionChanged();
			ToIdle();
		}
		break;
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
	case Game::MsgId::OnGetAttack:
		if (m_actor)
			m_actor->SetHit();
		return DefEntityProc(msg, wparam, lparam);
	case Game::MsgId::OnSelecting:
		if (!m_active || m_died) {
			return Game::MsgResult::MethodNotAllowed;
		}
		break;
	case Game::MsgId::OnHpDropToZero:
		if (m_died)
			break;
		ToDying();
		m_died = true;
		m_body.reset();
		m_detector.reset();
		break;
	default:
		return DefEntityProc(msg, wparam, lparam);
	}
	return Game::MsgResult::OK;
}

void Mover::ToStart(Game::IActor::Direction d) {
	if (m_actor)
		m_actor->ChangeStatus(
			Game::IActor::AnimationStatus::Normal
		);
	if (TryMove()) {
		m_actor->InitDirection(
			m_moveTargetPos[0] < m_position[0] ? Game::IActor::Direction::FL : Game::IActor::Direction::FR
		);
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
	int target[2] = { (int)m_position[0], (int)m_position[1] };
	Game::MsgResultType res =
		Game::GameGlobal::board->
		GetHost(Game::HostJob::MapPathManager)->
		ReceiveMessage(Game::HostMsgId::MapLeadQuery, 0, (intptr_t)target);
	switch (res) {
	case Game::MsgResult::Leader_TempRes:
		m_tempMoveTarget = true;
		break;
	case Game::MsgResult::Leader_FinalRes:
		m_tempMoveTarget = false;
		break;
	case Game::MsgResult::Leader_AtInvalidBlock:
		m_body->MoveTo(target[0] + 0.5f, target[1] + 0.5f, m_position, nullptr);
		OnPositionChanged();
		return false;
	case Game::MsgResult::Leader_NoAvailablePath:
		m_body->MoveTo(target[0] + 0.5f, target[1] + 0.5f, m_position, nullptr);
		OnPositionChanged();
		return false;
	default:
		return false;
	}
	m_moveTargetPos[0] = target[0] + 0.5f;
	m_moveTargetPos[1] = target[1] + 0.5f;
	m_body->MoveTo(m_moveTargetPos[0], m_moveTargetPos[1], m_position, nullptr);
	return true;
}

}
