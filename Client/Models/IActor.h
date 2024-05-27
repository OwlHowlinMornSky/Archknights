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

namespace Game {

class IActor {
public:
	enum class AnimationEvent : char {
		Default = 0,
		Begin,
		Idle,
		Attack,
		AttackDown,
		Stun,
		Die,
		Skill0,
		Skill1,
		Skill2,
		Skill3,
		Skill4,
		Skill5,
		Skill6,
		Skill7
	};

	enum class Direction : char {
		FR = 0,
		FL,
		BR,
		BL,
		NotCare
	};

public:
	IActor() = default;
	virtual ~IActor() = default;
	
	virtual void Exit() = 0;

	virtual void InitDirection(Direction direction) = 0;

	virtual void TriggerAnimation(AnimationEvent type, Direction direction = Direction::NotCare) = 0;
	virtual void TriggerAnimationEx(int excode, void* data) {}

	virtual void SetPosition(float x, float y, float z) = 0;
	virtual void SetOutline(bool enabled) {}

public:
	virtual int AnimEventCnt_OnStart() = 0;
	virtual int AnimEventCnt_OnAttack() = 0;
	virtual int AnimEvent_DieOver() = 0;
	virtual int AnimEvent_StartOver() = 0;
	virtual int AnimEvent_AttackOver() = 0;
};

}
