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
#include "char_151_myrtle_factory.h"

#include "char_151_myrtle.h"
#include "char_151_myrtle_actor.h"
#include "../Game/Stage.h"

namespace Game {

bool Char_151_Mytle_Factory::Load() {
	auto fac = Model::IAnimationFactory::Instance();

	char res = fac->CreatePose2(m_pose[0], m_pose[1], "char_151_myrtle");
	if (res != 3)
		return false;

	return true;
}

bool Char_151_Mytle_Factory::CreateEntity(std::shared_ptr<Entity>& ptr) {
	auto unit = std::make_shared<Units::Char_151_Myrtle>();


	auto anim0 = m_pose[0]->CreateAnimation();
	anim0->Setup();
	auto anim1 = m_pose[1]->CreateAnimation();
	anim1->Setup();

	auto actor = std::make_shared<Char_151_Mytle_Actor_Vanilla>(anim0, anim1);

	Game::Global::show->AddActor(actor);

	unit->m_actor = actor;

	ptr = unit;

	return true;
}

}

namespace EntityFactoryLink {

std::unique_ptr<Game::EntityFactory> Create151() {
	std::unique_ptr<Game::EntityFactory> res;
	res = std::make_unique<Game::Char_151_Mytle_Factory>();
	return std::move(res);
}

}
