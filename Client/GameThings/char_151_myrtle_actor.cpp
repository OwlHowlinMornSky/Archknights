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

Game::Char_151_Mytle_Actor_Vanilla::Char_151_Mytle_Actor_Vanilla(
	std::shared_ptr<ME::IModel> _f,
	std::shared_ptr<ME::IModel> _b
) : Actor2(_f, _b) {
	_f->setScale(0.575f);
	_b->setScale(0.575f);
}

void Game::Char_151_Mytle_Actor_Vanilla::TriggerAnimation(AnimationEvent type, Direction direction) {
	CurrentAnimationClass* modify = m_current;
	switch (type) {
	case AnimationEvent::Begin:
	case AnimationEvent::Idle:
		if (m_direction != Direction::NotCare && direction != m_direction) {
			m_targetDirection = direction;
			if (m_currentFBDirection != ((static_cast<char>(m_targetDirection) & 0x02) != 0)) {
				m_target = GetAnimation(!m_currentFBDirection);
				modify = m_target;
			}
			m_isRolling = true;
		}
		break;
	case AnimationEvent::Die:
	default:
		direction = (static_cast<char>(m_targetDirection) & 0x01) ? Direction::FL : Direction::FR;
		if (m_direction != Direction::NotCare && direction != m_direction) {
			m_targetDirection = direction;
			if (m_currentFBDirection != ((static_cast<char>(m_targetDirection) & 0x02) != 0)) {
				m_target = GetAnimation(!m_currentFBDirection);
				modify = m_target;
			}
			m_isRolling = true;
		}
		break;
	}
	switch (type) {
	case AnimationEvent::Begin:
		cnt_OnStart = 0;
		cnt_StartOver = 0;
		modify->setAnimation(0, "Start", false);
		modify->addAnimation(0, "Idle", true, 0.0f);
		break;
	case AnimationEvent::Idle:
		modify->setAnimation(0, "Idle", true);
		break;
	case AnimationEvent::Attack:
		cnt_OnAttack = 0;
		cnt_AttackOver = 0;
		modify->setAnimation(0, "Attack", false);
		//modify->addAnimation(0, "Default", false, 0.0f);
		break;
	case AnimationEvent::Stun:
	case AnimationEvent::Die:
		cnt_DieOver = 0;
		modify->setAnimation(0, "Die", false);
		break;
	case AnimationEvent::Skill0:
		modify->setAnimation(0, "Skill_Begin", false);
		modify->addAnimation(0, "Skill_Loop", false, 0.0f);
		break;
	default:
		modify->setAnimation(0, "Default", false);
	}
}

#endif // ARCHKNIGHTS_LIMITED
