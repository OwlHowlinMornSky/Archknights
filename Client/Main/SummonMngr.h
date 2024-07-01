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

#include "ISummonMngr.h"

#include "EntityFactory.h"
#include <vector>

namespace Main {

struct SummonData {
	size_t id;
	std::unique_ptr<EntityFactory> factory;
};

class SummonMngr final :
	public ISummonMngr {
public:
	SummonMngr();
	virtual ~SummonMngr();

public:
	virtual void beginAdd() override;
	virtual bool addEntity(size_t id, bool isEnemy = false) override;
	virtual void endAdd() override;

public:
	virtual void onJoined();
	virtual void onKicking();

	virtual void fixedUpdate();

	virtual Game::MsgResultType receiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam);

protected:
	std::vector<SummonData> m_data;
};

}
