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
#include "char_101_sora_factory.h"

#include "char_101_sora.h"
#include "char_101_sora_actor.h"
#include "../Game/Stage.h"

bool Game::Char_101_Sora_Factory::Load() {
	auto fac = Model::IAnimationFactory::Instance();

	bool res = fac->CreatePose(m_pose[0], "char_101_sora", 0);
	if (!res)
		return false;

	return true;
}

bool Game::Char_101_Sora_Factory::CreateEntity(std::shared_ptr<Entity>& ptr) {
	auto unit = std::make_shared<Units::Char_101_Sora>();

	auto anim0 = m_pose[0]->CreateAnimation();
	anim0->Setup();

	auto actor = std::make_shared<Char_101_Sora_Actor_Vanilla>(anim0);

	Game::Global::show->AddActor(actor);

	unit->m_actor = actor;

	ptr = unit;

	return true;
}


namespace EntityFactoryLink {

std::unique_ptr<Game::EntityFactory> Create101() {
	std::unique_ptr<Game::EntityFactory> res;
	res = std::make_unique<Game::Char_101_Sora_Factory>();
	return std::move(res);
}

}
