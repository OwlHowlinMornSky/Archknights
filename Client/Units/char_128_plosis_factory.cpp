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
#include "../Game/Stage.h"
#include "../Models/Actor2.h"

bool Unit::Char_128_Plosis_Factory::load() {
	auto fac = Model::IAnimationFactory::Instance();

#ifdef ARCHKNIGHTS_LIMITED
	test = true;

	char res = fac->createPose2(m_pose[0], m_pose[1], test ? "char_128_plosis_epoque#3" : "char_128_plosis");
	if (res != 3)
		return false;
#else
	char res = fac->CreatePose2(m_pose[0], m_pose[1], "char_128_plosis");
	if (res != 3)
		return false;
#endif // ARCHKNIGHTS_LIMITED

	setAnimationInfoStorage();

	return true;
}

bool Unit::Char_128_Plosis_Factory::createEntity(std::shared_ptr<Game::Entity>& ptr) {
	auto unit = std::make_shared<Unit::Char_128_Plosis>();

	auto anim0 = m_pose[0]->createAnimation();
	anim0->setup();
	auto anim1 = m_pose[1]->createAnimation();
	anim1->setup();

	anim0->setParameter(233, 0.9f);
	anim1->setParameter(233, 0.9f);

	auto actor = std::make_shared<Model::Actor2>(anim0, anim1);
	actor->setInfoStorage(m_info[0], m_info[1]);
	Game::Global::stage->addActor(actor);
	unit->m_actor = actor;

	ptr = unit;

	return true;
}

void Unit::Char_128_Plosis_Factory::setAnimationInfoStorage() {
	Model::AnimationInfo* info;
	Model::IAnimationPose* pose;
	pose = m_pose[0].get();
	{
		info = m_info[0] + Game::IActor::AnimationStatus::Normal;
		{
			info->Default = (Model::AnimationInfo::InfoType)pose->getAnimation("Default");
			info->Begin = (Model::AnimationInfo::InfoType)pose->getAnimation("Start");
			info->Idle = (Model::AnimationInfo::InfoType)pose->getAnimation("Idle");
			info->AttackLoop = (Model::AnimationInfo::InfoType)pose->getAnimation("Attack");
			info->Die = (Model::AnimationInfo::InfoType)pose->getAnimation("Die");
			info->StunIn = info->Die;
		}
		info = m_info[0] + Game::IActor::AnimationStatus::Skill0;
		{
			info->Begin = (Model::AnimationInfo::InfoType)pose->getAnimation("Skill_Start");
			info->Idle = (Model::AnimationInfo::InfoType)pose->getAnimation("Skill_Loop");
			info->Die = (Model::AnimationInfo::InfoType)pose->getAnimation("Skill_End");
		}
	}
	pose = m_pose[1].get();
	{
		info = m_info[1] + Game::IActor::AnimationStatus::Normal;
		{
			info->Default = (Model::AnimationInfo::InfoType)pose->getAnimation("Default");
			info->Begin = (Model::AnimationInfo::InfoType)pose->getAnimation("Start");
			info->Idle = (Model::AnimationInfo::InfoType)pose->getAnimation("Idle");
			info->AttackLoop = (Model::AnimationInfo::InfoType)pose->getAnimation("Attack");
			info->Die = (Model::AnimationInfo::InfoType)pose->getAnimation("Die");
			info->StunIn = info->Die;
		}
		info = m_info[1] + Game::IActor::AnimationStatus::Skill0;
		{
			info->Begin = (Model::AnimationInfo::InfoType)pose->getAnimation("Skill_Start");
			info->Idle = (Model::AnimationInfo::InfoType)pose->getAnimation("Skill_Loop");
			info->Die = (Model::AnimationInfo::InfoType)pose->getAnimation("Skill_End");
		}
	}
}

namespace EntityFactoryLink {

std::unique_ptr<Main::EntityFactory> Create128() {
	std::unique_ptr<Main::EntityFactory> res;
	res = std::make_unique<Unit::Char_128_Plosis_Factory>();
	return std::move(res);
}

}
