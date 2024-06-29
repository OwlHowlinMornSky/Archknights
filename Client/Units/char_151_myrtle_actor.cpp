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
#include "char_151_myrtle_actor.h"

#ifdef ARCHKNIGHTS_LIMITED

Unit::Char_151_Mytle_Actor_Vanilla::Char_151_Mytle_Actor_Vanilla(
	std::shared_ptr<ME::IModel> _f,
	std::shared_ptr<ME::IModel> _b
) : Actor2(_f, _b) {
	_f->setScale(0.575f);
	_b->setScale(0.575f);
}

void Unit::Char_151_Mytle_Actor_Vanilla::ChangeStatus(AnimationStatus status) {
	Model::AnimationInfo* face = GetInfo(false);
	Model::AnimationInfo* back = GetInfo(true);
	auto animf = GetAnimation(false);
	auto animb = GetAnimation(true);

	Model::AnimationInfo* c = face;
	auto a = animf;

	switch (status) {
	default:
	case AnimationStatus::Normal:
		c->Default = a->findAnimation("Default");
		c->Begin = a->findAnimation("Start");
		c->Idle = a->findAnimation("Idle");
		c->AttackIn = nullptr;
		c->AttackLoop = a->findAnimation("Attack");
		c->AttackOut = nullptr;
		c->Die = a->findAnimation("Die");
		c->StunIn = c->Die;
		c->StunLoop = nullptr;
		c->StunOut = nullptr;

		c = back;
		a = animb;
		c->Default = a->findAnimation("Default");
		c->Begin = a->findAnimation("Start");
		c->Idle = a->findAnimation("Idle");
		c->AttackIn = nullptr;
		c->AttackLoop = a->findAnimation("Attack");
		c->AttackOut = nullptr;
		c->Die = a->findAnimation("Die");
		c->StunIn = c->Die;
		c->StunLoop = nullptr;
		c->StunOut = nullptr;

		break;
	case AnimationStatus::Skill0:
		c->Begin = a->findAnimation("Skill_Begin");
		c->Idle = a->findAnimation("Skill_Loop");
		c->Die = a->findAnimation("Skill_End");

		c = back;
		a = animb;
		c->Begin = a->findAnimation("Skill_Begin");
		c->Idle = a->findAnimation("Skill_Loop");
		c->Die = a->findAnimation("Skill_End");
		break;
	}
}

#endif // ARCHKNIGHTS_LIMITED
