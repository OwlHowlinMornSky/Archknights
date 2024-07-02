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
#include "../Game/Stage.h"
#include "../Models/Actor.h"

bool Unit::Char_101_Sora_Factory::load() {
	auto fac = Model::IAnimationFactory::Instance();

	bool res = fac->createPose(m_pose, "char_101_sora", 0);
	if (!res)
		return false;

	setAnimationInfoStorage();

	return true;
}

bool Unit::Char_101_Sora_Factory::createEntity(std::shared_ptr<Game::Entity>& ptr) {
	auto unit = std::make_shared<Unit::Char_101_Sora>();

	auto anim0 = m_pose->createAnimation();
	anim0->setup();

	//anim0->setScale(0.725f);
	anim0->setParameter(233, 0.725f);

	auto actor = std::make_shared<Model::Actor>(anim0);
	actor->setInfoStorage(m_info);

	Game::Global::stage->addActor(actor);

	unit->m_actor = actor;

	ptr = unit;

	return true;
}

void Unit::Char_101_Sora_Factory::setAnimationInfoStorage() {
	Model::AnimationInfo* info;
	Model::IAnimationPose* pose;

	info = m_info + Game::IActor::AnimationStatus::Normal;
	pose = m_pose.get();

	info->Default = (Model::AnimationInfo::InfoType)pose->getAnimation("Default");
	info->Begin = (Model::AnimationInfo::InfoType)pose->getAnimation("Start");
	info->Idle = (Model::AnimationInfo::InfoType)pose->getAnimation("Idle");
	info->Die = (Model::AnimationInfo::InfoType)pose->getAnimation("Die");
	info->StunIn = info->Die;
}


namespace EntityFactoryLink {

std::unique_ptr<Main::EntityFactory> Create101() {
	std::unique_ptr<Main::EntityFactory> res;
	res = std::make_unique<Unit::Char_101_Sora_Factory>();
	return std::move(res);
}

}
