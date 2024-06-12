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

#include "Tower.h"

namespace Units {

class Char_128_Plosis final :
	public Tower {
	using Parent = Tower;
public:
	Char_128_Plosis();
	virtual ~Char_128_Plosis();

public:
	virtual void OnJoined();
	virtual void OnKicking();

	virtual void FixedUpdate(float dt);

	virtual Game::MsgResultType ReceiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam);

protected:
	virtual bool TryAttack() override;
	virtual bool StillCanAttack() override;
	virtual void OnAttack() override;

public:
	Game::ModifySwitch m_m;
	Game::ModifySwitch m_addDef;
	Game::ModifySwitch m_addMsgDef;
	Game::EntityLocationType m_targetAd;
	Game::EntityIdType m_targetId;
};

}
