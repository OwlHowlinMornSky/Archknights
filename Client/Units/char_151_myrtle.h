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
#pragma once

#include "../Main/Tower.h"

namespace Unit {

class Char_151_Myrtle final :
	public Main::Tower {
	using Parent = Tower;
public:
	Char_151_Myrtle();
	virtual ~Char_151_Myrtle();

public:
	virtual void onJoined();
	virtual void onKicking();

	virtual bool fixedUpdate();

	virtual Game::MsgResultType receiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam);

protected:
	virtual bool tryToAttack() override;
	virtual bool isStillCanAttack() override;
	virtual void onAttack() override;

public:
	std::weak_ptr<Game::Entity> m_target;
	std::unique_ptr<Physics::IDetector> m_blocker;

	int m_blockTotal;
	std::map<Game::EntityIdType, std::weak_ptr<Game::Entity>> m_blockedMovers;
};

}
