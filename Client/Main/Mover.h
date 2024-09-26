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
#pragma once

#include "../Game/Entity.h"
#include "../Game/IActor.h"
#include "../Game/Global.h"
#include "../Game/Board.h"
#include <memory>

namespace Main {

class Mover :
	public Game::Entity {
public:
	enum class Status : unsigned char {
		Default = 0,
		Begin,
		Idle,
		Attaking,
		Stun,
		Dying,
		Returning,
		Moving,
		Unbalance
	};

	Mover();
	virtual ~Mover();

public:
	virtual void onJoined();
	virtual void onKicking();

	virtual bool fixedUpdate();

	virtual Game::MsgResultType receiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam);

protected:
	virtual void onPositionChanged();
	void onPositionVaried();

	Game::MsgResultType DefMoverProc(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam);

	void setStatusToStart(Game::IActor::Direction d = Game::IActor::Direction::NotCare);
	void setStatusToBegin(Game::IActor::Direction d = Game::IActor::Direction::NotCare);
	void setStatusToIdle(Game::IActor::Direction d = Game::IActor::Direction::NotCare);
	void setStatusToAttack(Game::IActor::Direction d = Game::IActor::Direction::NotCare);
	void setStatusToStun(Game::IActor::Direction d = Game::IActor::Direction::NotCare);
	void setStatusToDying(Game::IActor::Direction d = Game::IActor::Direction::NotCare);
	void setStatusToReturn(Game::IActor::Direction d = Game::IActor::Direction::NotCare);
	void setStatusToMoving(Game::IActor::Direction d = Game::IActor::Direction::NotCare);
	void setStatusToUnbalance(Game::IActor::Direction d = Game::IActor::Direction::NotCare);

	/**
	 * @brief 尝试攻击。
	 * @return 可开始攻击则为 false。
	 */
	virtual bool tryToAttack();
	virtual bool isStillCanAttack();
	void onAttackBegin();
	virtual void onAttack();

	bool tryToMove();
	bool tryToTakeNextMoveCmd();

public:
	bool m_active, m_died, m_atked;
	bool m_tempMoveTarget;
	bool m_isBlocked;
	float m_moveTargetPos[2];
	Status m_status;
	std::shared_ptr<Game::IActor> m_actor;
	std::unique_ptr<Physics::IBody> m_body;
	std::unique_ptr<Physics::IDetector> m_detector;
	Game::AnimEventNote m_note;
	int m_checkpointTarget;

	std::weak_ptr<Game::Entity> m_blocker;
};

}
