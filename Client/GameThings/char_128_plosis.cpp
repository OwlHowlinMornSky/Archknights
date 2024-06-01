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
#include "char_128_plosis.h"

#include "../Game/MsgResult.h"
#include "MsgId.h"

Units::Char_128_Plosis::Char_128_Plosis() {}

Units::Char_128_Plosis::~Char_128_Plosis() {}

void Units::Char_128_Plosis::OnJoined() {
	Parent::OnJoined();
	printf_s("HP: %f\n", m_hp);
}

void Units::Char_128_Plosis::OnKicking() {
	Parent::OnKicking();
}

void Units::Char_128_Plosis::FixedUpdate(float dt) {
	if (!m_active) {
		if (m_actor->AnimEvent_StartOver()) {
			m_active = true;
		}
		return;
	}
	if (m_died) {
		if (m_actor->AnimEvent_DieOver()) {
			KickSelf();
		}
		return;
	}
}

Game::MsgResultType Units::Char_128_Plosis::ReceiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	if (msg == Game::MsgId::OnHpChanged) {
		printf_s("HP: %f\n", m_hp);
	}
	return DefTowerProc(msg, wparam, lparam);
}
