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

#include "TypeDef.h"
#include "Attribute.h"
#include "Ability.h"

#include <list>

namespace Game {

struct Modifier;

class Entity {
	friend class GameBoard;

public:
	enum AttributeType : int {
		MaxHp = 0, // 最大HP

		Atk,       // 攻击力
		AtkSpd,    // 攻击速度
		AtkGap,    // 攻击间隔
		Def,       // 防御力
		MagDef,    // 魔抗

		MoveSpd,   // 移动速度
		WightLv,   // 重量等级

		MpGrowSpd, // 回蓝速度

		COUNT
	};
	enum AbilityType : int {
		Attack = 0,
		StartSkill,

		Block, // 最大阻挡数

		ABCNT
	};

public:
	Entity();
	virtual ~Entity() = default;

public:
	void BasicOnJoined(EntityIdType id, EntityLocationType location);
	void BasicOnKicking();
	virtual void OnJoined();
	virtual void OnKicking();

	virtual void FixedUpdate(float dt);

	virtual MsgResultType ReceiveMessage(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam);

	std::list<Modifier>::iterator Modify(AttributeType attribute, Modifier& data);
	void ModifyRemove(AttributeType attribute, std::list<Modifier>::iterator iterator);
	virtual void OnModifierChanged(AttributeType attribute);

	void ChangeAbility(AbilityType type, Ability::ValueType val);
	Ability::ValueType GetAbility(AbilityType type) const;

	Attribute::ValueType GetHp() const;
	Attribute::ValueType GetAttribute(AttributeType type) const;

public:
	EntityIdType getID() const;
	EntityLocationType getLocation() const;

	const float* getPosition() const;
	void setPosition(float x, float y);

	const float* getScale() const;
	void setScale(float x, float y);

	float getRotation() const;
	void setRotation(float rot);

protected:
	virtual void OnPositionChanged();
	virtual void OnRotationChanged();
	virtual void OnScaleChanged();

	void KickSelf() const;

	MsgResultType DefEntityProc(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam);

protected:
	Attribute::ValueType m_hp;
	float m_rotation;
	float m_position[2];
	float m_scale[2];
	EntityIdType m_id;
	EntityLocationType m_location;

	Attribute attributes[AttributeType::COUNT];
	Ability abilities[AbilityType::ABCNT];
	std::list<Modifier> m_modifiers[AttributeType::COUNT];
};

} // namespace Game

#include "Modifier.h"
