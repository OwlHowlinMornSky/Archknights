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
#include "RootLoader.h"
#include "GameGlobal.h"
#include "Char001_Myrtle.h"

namespace game {

RootLoader::RootLoader() {}

void RootLoader::OnJoined(size_t id, size_t location) {
	Parent::OnJoined(id, location);

	// for test
	Global::data.board->AddFactory(std::make_shared<Char001_Myrtle_Factory>());
	Global::data.board->JoinEntityFromFactory(1);
	Global::data.board->JoinEntityFromFactory(1);
	Global::data.board->JoinEntityFromFactory(1);

	return;
}

void RootLoader::OnUpdate(float dt) {}

void RootLoader::OnKicking() {
	return Parent::OnKicking();
}

} // namespace game
