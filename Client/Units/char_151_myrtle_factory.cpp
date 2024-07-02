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
#include "../Game/Stage.h"
#include "../Models/Actor2.h"

namespace Unit {

bool Char_151_Mytle_Factory::load() {
	auto fac = Model::IAnimationFactory::Instance();

	char res = fac->createPose2(m_pose[0], m_pose[1], "char_151_myrtle");
	if (res != 3)
		return false;

	setAnimationInfoStorage();

	return true;
}

bool Char_151_Mytle_Factory::createEntity(std::shared_ptr<Game::Entity>& ptr) {
	auto unit = std::make_shared<Unit::Char_151_Myrtle>();


	auto anim0 = m_pose[0]->createAnimation();
	anim0->setup();
	auto anim1 = m_pose[1]->createAnimation();
	anim1->setup();

	//anim0->setScale(0.575f);
	//anim1->setScale(0.575f);
	anim0->setParameter(233, 0.575f);
	anim1->setParameter(233, 0.575f);

	auto actor = std::make_shared<Model::Actor2>(anim0, anim1);
	actor->setInfoStorage(m_info[0], m_info[1]);

	Game::Global::stage->addActor(actor);

	unit->m_actor = actor;

	ptr = unit;

	return true;
}

void Char_151_Mytle_Factory::setAnimationInfoStorage() {
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
		m_info[0][Game::IActor::AnimationStatus::Skill0] =
			m_info[0][Game::IActor::AnimationStatus::Normal];
		info = m_info[0] + Game::IActor::AnimationStatus::Skill0;
		{
			info->Begin = (Model::AnimationInfo::InfoType)pose->getAnimation("Skill_Begin");
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
		m_info[1][Game::IActor::AnimationStatus::Skill0] =
			m_info[1][Game::IActor::AnimationStatus::Normal];
		info = m_info[1] + Game::IActor::AnimationStatus::Skill0;
		{
			info->Begin = (Model::AnimationInfo::InfoType)pose->getAnimation("Skill_Begin");
			info->Idle = (Model::AnimationInfo::InfoType)pose->getAnimation("Skill_Loop");
			info->Die = (Model::AnimationInfo::InfoType)pose->getAnimation("Skill_End");
		}
	}
}

}

namespace EntityFactoryLink {

std::unique_ptr<Main::EntityFactory> Create151() {
	std::unique_ptr<Main::EntityFactory> res;
	res = std::make_unique<Unit::Char_151_Mytle_Factory>();
	return std::move(res);
}

}
