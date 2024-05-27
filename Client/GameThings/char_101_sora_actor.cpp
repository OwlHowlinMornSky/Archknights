﻿/*
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
#include "char_101_sora_actor.h"

#ifdef ARCHKNIGHTS_LIMITED

Game::Char_101_Sora_Actor_Vanilla::Char_101_Sora_Actor_Vanilla(
	std::shared_ptr<ME::IModel> _f
) : Actor(_f) {
	_f->setScale(0.725f);
}

void Game::Char_101_Sora_Actor_Vanilla::TriggerAnimation(AnimationEvent type, Direction direction) {
	switch (type) {
	case AnimationEvent::Begin:
		cnt_StartOver = 0;
		cnt_OnStart = 0;
		m_current->setAnimation(0, "Start", false);
		m_current->addAnimation(0, "Idle", true, 0.0f);
		break;
	case AnimationEvent::Idle:
		/*if (m_direction != Direction::NotCare && direction != m_direction) {
			m_targetDirection = direction;
			if (m_currentFBDirection != ((static_cast<char>(m_targetDirection) & 0x02) != 0)) {
				m_target = GetAnimation(!m_currentFBDirection);
				m_target->setAnimation(0, "Idle", true);
			}
			else {
				m_current->setAnimation(0, "Idle", true);
			}
			m_isRolling = true;
		}
		else {
			m_current->setAnimation(0, "Idle", true);
		}*/
		break;
	case AnimationEvent::Die:
		cnt_DieOver = 0;
		InitDirection((static_cast<char>(m_targetDirection) & 0x01) ? Direction::FL : Direction::FR);
		m_current->setAnimation(0, "Die", false);
		break;
	}
}

#endif // ARCHKNIGHTS_LIMITED
