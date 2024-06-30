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
#include "enemy_1002_nsabr_factory.h"

#include "enemy_1002_nsabr_actor.h"
#include "enemy_1002_nsabr.h"
#include "../Game/Stage.h"

Unit::Enemy_1002_nsabr_Factory::Enemy_1002_nsabr_Factory() :
	m_pool(),
	m_alloc(&m_pool) {}

Unit::Enemy_1002_nsabr_Factory::~Enemy_1002_nsabr_Factory() {}

bool Unit::Enemy_1002_nsabr_Factory::Load() {
	auto fac = Model::IAnimationFactory::Instance();

	bool res = fac->CreateEnemyPose(m_pose[0], "enemy_1002_nsabr");
	if (!res)
		return false;

	return true;
}

bool Unit::Enemy_1002_nsabr_Factory::CreateEntity(std::shared_ptr<Game::Entity>& ptr) {
	auto unit = std::allocate_shared<Unit::Enemy_1002_nsabr>(m_alloc);

	auto anim0 = m_pose[0]->CreateAnimation();
	anim0->Setup();

	auto actor = std::make_shared<Enemy_1002_nsabr_Actor_Vanilla>(anim0);

	Game::Global::stage->AddActor(actor);

	unit->m_actor = actor;

	ptr = unit;

	return true;
}

namespace EntityFactoryLink {

std::unique_ptr<Main::EntityFactory> CreateEnemy1002() {
	std::unique_ptr<Main::EntityFactory> res;
	res = std::make_unique<Unit::Enemy_1002_nsabr_Factory>();
	return std::move(res);
}

}
