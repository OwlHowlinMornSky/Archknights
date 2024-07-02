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

#include "enemy_1002_nsabr.h"
#include "../Game/Stage.h"
#include "../Models/Actor.h"

Unit::Enemy_1002_nsabr_Factory::Enemy_1002_nsabr_Factory() :
	m_pool(),
	m_alloc(&m_pool) {}

Unit::Enemy_1002_nsabr_Factory::~Enemy_1002_nsabr_Factory() {}

bool Unit::Enemy_1002_nsabr_Factory::load() {
	auto fac = Model::IAnimationFactory::Instance();

	bool res = fac->createEnemyPose(m_pose, "enemy_1002_nsabr");
	if (!res)
		return false;

	setAnimationInfoStorage();

	return true;
}

bool Unit::Enemy_1002_nsabr_Factory::createEntity(std::shared_ptr<Game::Entity>& ptr) {
	auto unit = std::allocate_shared<Unit::Enemy_1002_nsabr>(m_alloc);

	auto anim0 = m_pose->createAnimation();
	anim0->setup();

	//anim0->setScale(0.65f);
	anim0->setParameter(233, 0.65f);

	auto actor = std::make_shared<Model::Actor>(anim0);
	actor->setInfoStorage(m_info);
	actor->m_shadowRadius = 0.4f;

	Game::Global::stage->addActor(actor);

	unit->m_actor = actor;

	ptr = unit;

	return true;
}

void Unit::Enemy_1002_nsabr_Factory::setAnimationInfoStorage() {
	Model::AnimationInfo* info;
	Model::IAnimationPose* pose;

	info = m_info + Game::IActor::AnimationStatus::Normal;
	pose = m_pose.get();

	info->Default = (Model::AnimationInfo::InfoType)pose->getAnimation("Default");
	info->Idle = (Model::AnimationInfo::InfoType)pose->getAnimation("Idle");
	info->AttackLoop = (Model::AnimationInfo::InfoType)pose->getAnimation("Attack");
	info->Die = (Model::AnimationInfo::InfoType)pose->getAnimation("Die");
	info->StunIn = info->Default;
	info->MoveLoop = (Model::AnimationInfo::InfoType)pose->getAnimation("Run_Loop_02");
}

namespace EntityFactoryLink {

std::unique_ptr<Main::EntityFactory> CreateEnemy1002() {
	std::unique_ptr<Main::EntityFactory> res;
	res = std::make_unique<Unit::Enemy_1002_nsabr_Factory>();
	return std::move(res);
}

}
