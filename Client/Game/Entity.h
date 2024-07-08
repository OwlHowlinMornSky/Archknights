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
#include "Hook.h"

#include <list>
#include <memory>

namespace Game {

struct Modifier;

class Entity {
	friend class Board;

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
	void basicOnJoined(EntityIdType id, EntityLocationType location);
	void basicOnKicking();
	virtual void onJoined();
	virtual void onKicking();

	virtual void physicsUpdate();
	virtual void fixedUpdate();

	MsgResultType EntityProc(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam);
	virtual MsgResultType receiveMessage(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam);

	std::list<Modifier>::iterator setModifier(AttributeType attribute, Modifier& data);
	void eraseModifier(AttributeType attribute, std::list<Modifier>::iterator iterator);
	virtual void onModifierChanged(AttributeType attribute);

	void changeAbility(AbilityType type, Ability::ValueType val);
	Ability::ValueType getAbility(AbilityType type) const;

	Attribute::ValueType getHp() const;
	Attribute::ValueType getAttribute(AttributeType type) const;

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
	virtual void onPositionChanged();
	virtual void onRotationChanged();
	virtual void onScaleChanged();

	void kickSelf() const;

	MsgResultType DefEntityProc(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam);

	void setAttributeOringalValue(AttributeType type, Attribute::ValueType val);

protected:
	Attribute::ValueType m_hp;
	float m_rotation;
	float m_position[2];
	float m_scale[2];
	EntityIdType m_id;
	EntityLocationType m_location;

	Attribute m_attributes[AttributeType::COUNT];
	Ability m_abilities[AbilityType::ABCNT];
	std::list<Modifier> m_modifiers[AttributeType::COUNT];
	std::list<std::shared_ptr<Hook>> m_hooks;
};

} // namespace Game

#include "Modifier.h"
