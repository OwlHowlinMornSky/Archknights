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

#include "Attribute.h"
#include "TypeDef.h"

namespace Game {

struct DamageData {
	enum DamageType {
		Normal = 0, // 物理伤害;
		Magical,    // 魔法伤害;
		Direct      // 真实伤害;
	} type;                        // 伤害类型。
	Attribute::ValueType dmgValue; // 伤害量，取值 [0.0, inf)。
	Attribute::ValueType minValue; // 最小作用比例，取值 [0.0, 1.0]。

	DamageData() :
		type(DamageType::Normal),
		dmgValue(0.0f),
		minValue(0.05f) {}
};

struct AttackData {
	enum DistanceType : char {
		Near = 0, // 近战攻击;
		Remote    // 远程攻击;
	} distType; // 攻击类型。
	DamageData damage;
	std::weak_ptr<Entity> source;

	AttackData() :
		distType(DistanceType::Near),
		damage(),
		source() {}
};

struct HealData {
	Attribute::ValueType healValue;

	HealData() :
		healValue(0.0f) {}
};

}
