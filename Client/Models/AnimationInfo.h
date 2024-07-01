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
#pragma once

#ifdef ARCHKNIGHTS_LIMITED
#include "AnimationSpine.h"
#else
#include "AnimationFrames.h"
#endif // ARCHKNIGHTS_LIMITED

namespace Model {

#ifdef ARCHKNIGHTS_LIMITED
using CurrentAnimationClass = SpineAnimation;
#else
using CurrentAnimationClass = AnimationFrames;
#endif // ARCHKNIGHTS_LIMITED

struct AnimationInfo {
#ifdef ARCHKNIGHTS_LIMITED
	using InfoType = spine::Animation*;
#endif // ARCHKNIGHTS_LIMITED
	InfoType Default;
	InfoType Begin;
	InfoType Idle;
	InfoType AttackIn;
	InfoType AttackLoop;
	InfoType AttackOut;
	InfoType StunIn;
	InfoType StunLoop;
	InfoType StunOut;
	InfoType Die;
	InfoType MoveIn;
	InfoType MoveLoop;
	InfoType MoveOut;

	AnimationInfo() :
		Default(nullptr),
		Begin(nullptr),
		Idle(nullptr),
		AttackIn(nullptr),
		AttackLoop(nullptr),
		AttackOut(nullptr),
		StunIn(nullptr),
		StunLoop(nullptr),
		StunOut(nullptr),
		Die(nullptr),
		MoveIn(nullptr),
		MoveLoop(nullptr),
		MoveOut(nullptr) {}
};

}
