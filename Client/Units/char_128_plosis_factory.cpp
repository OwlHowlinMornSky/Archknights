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
#include "char_128_plosis_factory.h"

#include "char_128_plosis.h"
#include "char_128_plosis_actor.h"
#include "../Game/Stage.h"

bool Game::Char_128_Plosis_Factory::Load() {
	auto fac = Model::IAnimationFactory::Instance();

#ifdef ARCHKNIGHTS_LIMITED
	test = true;

	char res = fac->CreatePose2(m_pose[0], m_pose[1], test ? "char_128_plosis_epoque#3" : "char_128_plosis");
	if (res != 3)
		return false;
#else
	char res = fac->CreatePose2(m_pose[0], m_pose[1], "char_128_plosis");
	if (res != 3)
		return false;
#endif // ARCHKNIGHTS_LIMITED

	return true;
}

bool Game::Char_128_Plosis_Factory::CreateEntity(std::shared_ptr<Entity>& ptr) {
	auto unit = std::make_shared<Units::Char_128_Plosis>();

	auto anim0 = m_pose[0]->CreateAnimation();
	anim0->Setup();
	auto anim1 = m_pose[1]->CreateAnimation();
	anim1->Setup();

#ifdef ARCHKNIGHTS_LIMITED
	auto actor = std::make_shared<Char_128_Plosis_Actor_Vanilla>(anim0, anim1);
	Game::Global::show->AddActor(actor);
	unit->m_actor = actor;
#else
	auto actor = std::make_shared<Char_128_Plosis_Actor_Vanilla>(anim0, anim1);
	Game::GameGlobal::show->AddActor(actor);
	unit->m_actor = actor;
#endif // ARCHKNIGHTS_LIMITED

	ptr = unit;

	return true;
}

namespace EntityFactoryLink {

std::unique_ptr<Game::EntityFactory> Create128() {
	std::unique_ptr<Game::EntityFactory> res;
	res = std::make_unique<Game::Char_128_Plosis_Factory>();
	return std::move(res);
}

}
