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
#include "enemy_1002_nsabr_actor.h"

Game::Enemy_1002_nsabr_Actor_Vanilla::Enemy_1002_nsabr_Actor_Vanilla(
	std::shared_ptr<ME::IModel> _f
) : Actor(_f) {
	_f->setScale(0.65f);
}

void Game::Enemy_1002_nsabr_Actor_Vanilla::ChangeStatus(AnimationStatus status) {
	AnimationInfo* face = &m_info;
	auto animf = m_current;

	AnimationInfo* c = face;
	auto a = animf;

	switch (status) {
	default:
	case AnimationStatus::Normal:
		c->Default = a->findAnimation("Default");
		c->Begin = nullptr;
		c->Idle = a->findAnimation("Idle");
		c->AttackIn = nullptr;
		c->AttackLoop = a->findAnimation("Attack");
		c->AttackOut = nullptr;
		c->Die = a->findAnimation("Die");
		c->StunIn = c->Default;
		c->StunLoop = nullptr;
		c->StunOut = nullptr;
		c->MoveLoop = a->findAnimation("Run_Loop_02");
		break;
	}
}
