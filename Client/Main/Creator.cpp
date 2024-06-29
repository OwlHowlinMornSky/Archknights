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
#include "Creator.h"

#include "../Scenes/Scene_GameCommon.h"
#include "../Game/Board.h"
#include "../Game/Global.h"

#include "Initalizator.h"

namespace Game::Creator {

int setup() {
	Scene::GameCommon::setup();
	Game::GameBoard::setup();
	Game::GameGlobal::board->JoinEntity(std::make_shared<Game::Initalizator>());
	return 0;
}

void drop() {
	Game::GameBoard::drop();
	Scene::GameCommon::drop();
}
}
