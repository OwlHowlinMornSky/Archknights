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
#include "../Models/IActor.h"
#include <memory>
#include "../Game/GameGlobal.h"
#include "../Game/GameBoard.h"

namespace Units {

class Char_151_Myrtle final :
	public Game::Entity {
public:
	Char_151_Myrtle();
	virtual ~Char_151_Myrtle();

public:
	virtual void OnJoined();
	virtual void OnKicking();

	virtual void FixedUpdate(float dt);

	virtual Game::MsgResultType ReceiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam);

protected:
	virtual void OnPositionChanged();

public:
	std::shared_ptr<Game::IActor> m_actor;
	std::unique_ptr<Physics::IBody> m_body;
	std::unique_ptr<Physics::IDetector> m_inde;
	bool m_test;
};

}
