/*
*    Mystery Engine
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
#include <MysteryEngine/Game/IGameShow.h>

#include <MysteryEngine/Game/Animation.h>
#include <MysteryEngine/Game/Architecture.h>

namespace ME {

void IGameShow::AddAnimation(std::shared_ptr<Animation> a) {
	m_anims.push_back(a);
}

void IGameShow::AddModel(std::shared_ptr<Architecture> m) {
	m_archs.push_back(m);
}

void IGameShow::Update(float dt) {
	for (auto& i : m_anims) {
		i->Update(dt);
	}
}

} // namespace game
