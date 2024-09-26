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

void Enemy_1002_nsabr::onJoined() {
	Parent::onJoined();

	setAttributeOringalValue(AttributeType::MoveSpd, 0.6f * 20.0f);
	float spd = m_attributes[AttributeType::MoveSpd].effective;
	m_body->setMove(spd, spd * 9.8f);
}

void Enemy_1002_nsabr::onKicking() {
	Parent::onKicking();
}

bool Enemy_1002_nsabr::fixedUpdate() {
	return Parent::fixedUpdate();
}

Game::MsgResultType Enemy_1002_nsabr::receiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	return Parent::receiveMessage(msg, wparam, lparam);
}

bool Enemy_1002_nsabr::tryToAttack() {
	return true;
}

bool Enemy_1002_nsabr::isStillCanAttack() {
	return false;
}

void Enemy_1002_nsabr::onAttack() {}

}
