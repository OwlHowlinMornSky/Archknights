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
#include "enemy_1002_nsabr.h"


namespace Unit {

Unit::Enemy_1002_nsabr::Enemy_1002_nsabr() {}

Enemy_1002_nsabr::~Enemy_1002_nsabr() {}

void Enemy_1002_nsabr::OnJoined() {
	Parent::OnJoined();

	SetAttributeOringalValue(AttributeType::MoveSpd, 1.0f);
	//m_body->SetMove(100.0f, 50.0f);
}

void Enemy_1002_nsabr::OnKicking() {
	Parent::OnKicking();
}

void Enemy_1002_nsabr::FixedUpdate() {
	return Parent::FixedUpdate();
}

Game::MsgResultType Enemy_1002_nsabr::ReceiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	return Parent::ReceiveMessage(msg, wparam, lparam);
}

bool Enemy_1002_nsabr::TryAttack() {
	return true;
}

bool Enemy_1002_nsabr::StillCanAttack() {
	return false;
}

void Enemy_1002_nsabr::OnAttack() {}

}
