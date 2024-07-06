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

#include <MysteryEngine/G3D/IModel.h>

namespace Game {

struct AnimEventNote {
	int OnStart = 0;
	int OnAttack = 0;
	int DieOver = 0;
	int StartOver = 0;
	int AttackOver = 0;
};

class IActor :
	public ME::IModel {
public:
	enum class AnimationEvent : char {
		Default = 0,
		Begin,
		Idle,
		Attack,
		Stun,
		Die,
		Move
	};

	enum AnimationStatus : char {
		Normal = 0,
		AttackDown,
		Skill0,
		Skill1,
		Skill2,
		Skill3,
		Skill4,
		Skill5,
		Skill6,
		Skill7,

		COUNT
	};

	enum Direction : char {
		FR = 0,
		FL,
		BR,
		BL,
		NotCare,

		BackBit = 0x02,
		LeftBit = 0x01
	};

public:
	IActor() = default;
	virtual ~IActor() = default;

	virtual void setDirection(Direction direction) = 0;

	virtual void triggerAnimation(AnimationEvent type, Direction direction = Direction::NotCare) = 0;
	virtual void triggerAnimationEx(int excode, void* data) = 0;

	virtual void turnLeftRight(bool isLeft) = 0;

	virtual void setStatus(AnimationStatus status) = 0;

	virtual void setPosition(float x, float y, float z) = 0;
	virtual void setXY(float x, float y) = 0;

	virtual void setHitEffect() = 0;
	virtual void setInOutEffect(bool in, bool onlyShadow = false) = 0;

public:
	AnimEventNote* m_note;
	float m_shadowRadius;
	float m_shadowAlpha;
};

}
