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
#include "enemy_1002_nsabr.h"

#include "../Game/MsgResult.h"
#include "../Game/GameGlobal.h"
#include "../Game/GameBoard.h"
#include "HostMsgId.h"

namespace Units {

Units::Enemy_1002_nsabr::Enemy_1002_nsabr() {}

Enemy_1002_nsabr::~Enemy_1002_nsabr() {}

void Enemy_1002_nsabr::OnJoined() {
	Parent::OnJoined();

	m_body->SetMove(10.0f, 15.0f);
}

void Enemy_1002_nsabr::OnKicking() {
	Parent::OnKicking();
}

void Enemy_1002_nsabr::FixedUpdate() {
	switch (m_status) {
	case Status::Idle:
	{
		int target[2] = { (int)m_position[0], (int)m_position[1] };
		auto res =
			Game::GameGlobal::board->
			GetHost(Game::HostJob::MapPathManager)->
			ReceiveMessage(Game::HostMsgId::MapLeadQuery, 0, (intptr_t)target);
		if (res == Game::MsgResult::Leader_TempRes)
			m_tempMoveTarget = true;
		else if (res == Game::MsgResult::Leader_FinalRes)
			m_tempMoveTarget = false;
		else if (res == Game::MsgResult::Leader_AtInvalidBlock) {
			m_body->MoveTo(target[0] + 0.5f, target[1] + 0.5f, m_position, nullptr);
			OnPositionChanged();
			break;
		}
		else if (res == Game::MsgResult::Leader_NoAvailablePath) {
			m_body->MoveTo(target[0] + 0.5f, target[1] + 0.5f, m_position, nullptr);
			OnPositionChanged();
			break;
		}
		else
			break;
		m_t[0] = target[0] + 0.5f;
		m_t[1] = target[1] + 0.5f;
		m_body->MoveTo(m_t[0], m_t[1], m_position, nullptr);
		ToMoving(m_t[0] < m_position[0] ? Game::IActor::Direction::FL : Game::IActor::Direction::FR);
		break;
	}
	case Status::Moving:
	{
		float spd;
		if (m_body->MoveTo(m_t[0], m_t[1], m_position, &spd) < (m_tempMoveTarget ? 0.5f : 0.1f)) {
			OnPositionChanged();
			int target[2] = { (int)m_position[0], (int)m_position[1] };
			auto res =
				Game::GameGlobal::board->
				GetHost(Game::HostJob::MapPathManager)->
				ReceiveMessage(Game::HostMsgId::MapLeadQuery, 0, (intptr_t)target);
			if (res == Game::MsgResult::Leader_TempRes)
				m_tempMoveTarget = true;
			else if (res == Game::MsgResult::Leader_FinalRes)
				m_tempMoveTarget = false;
			else {
				m_body->ClearSpeed();
				ToIdle();
				break;
			}
			m_t[0] = target[0] + 0.5f;
			m_t[1] = target[1] + 0.5f;
			m_body->MoveTo(m_t[0], m_t[1], m_position, nullptr);
			m_actor->TurnDirection(m_t[0] < m_position[0]);
		}
		else {
			OnPositionChanged();

			if (spd < 0.01f) {
				int target[2] = { (int)m_position[0], (int)m_position[1] };
				auto res =
					Game::GameGlobal::board->
					GetHost(Game::HostJob::MapPathManager)->
					ReceiveMessage(Game::HostMsgId::MapLeadQuery, 0, (intptr_t)target);
				if (res == Game::MsgResult::Leader_TempRes)
					m_tempMoveTarget = true;
				else if (res == Game::MsgResult::Leader_FinalRes)
					m_tempMoveTarget = false;
				else {
					m_body->ClearSpeed();
					ToIdle();
					break;
				}
				m_t[0] = target[0] + 0.5f;
				m_t[1] = target[1] + 0.5f;
				m_body->MoveTo(m_t[0], m_t[1], m_position, nullptr);
				m_actor->TurnDirection(m_t[0] < m_position[0]);
			}
		}
		break;
	}
	default:
		return Parent::FixedUpdate();
	}
	return;
}

Game::MsgResultType Enemy_1002_nsabr::ReceiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	return Parent::ReceiveMessage(msg, wparam, lparam);
}

bool Enemy_1002_nsabr::TryAttack() {
	return true;
}

bool Enemy_1002_nsabr::StillCanAttack() {
	return false;
}

void Enemy_1002_nsabr::OnAttack() {}

}
