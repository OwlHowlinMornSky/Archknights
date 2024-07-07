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
#include "OccupiedPlace.h"
#include <memory>

namespace Main {

class Tower :
	public Game::Entity {
public:
	enum class Status : unsigned char {
		Default = 0,
		Begin,
		Idle,
		Attaking,
		Stun,
		Dying,
		Returning
	};

	Tower();
	virtual ~Tower();

public:
	virtual void onJoined();
	virtual void onKicking();

	virtual void fixedUpdate();

	virtual Game::MsgResultType receiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam);

protected:
	virtual void onPositionChanged();

	Game::MsgResultType DefTowerProc(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam);

	void setStatusToStart(Game::IActor::Direction d = Game::IActor::Direction::NotCare);
	void setStatusToBegin(Game::IActor::Direction d = Game::IActor::Direction::NotCare);
	void setStatusToIdle(Game::IActor::Direction d = Game::IActor::Direction::NotCare);
	void setStatusToAttack(Game::IActor::Direction d = Game::IActor::Direction::NotCare);
	void setStatusToStun(Game::IActor::Direction d = Game::IActor::Direction::NotCare);
	void setStatusToDying(Game::IActor::Direction d = Game::IActor::Direction::NotCare);
	void setStatusToReturn(Game::IActor::Direction d = Game::IActor::Direction::NotCare);

	/**
	 * @brief 尝试攻击。
	 * @return 可开始攻击则为 true。
	 */
	virtual bool tryToAttack();
	virtual bool isStillCanAttack();
	void onAttackBegin();
	virtual void onAttack();

public:
	bool m_active, m_died, m_atked;
	Game::IActor::Direction m_defaultDirection;
	Status m_status;
	std::shared_ptr<Game::IActor> m_actor;
	std::unique_ptr<Physics::IBody> m_body;
	std::unique_ptr<Physics::IDetector> m_detector;
	Game::AnimEventNote m_note;
	Main::OccupiedPlace m_occupiedPlace;
};

}
