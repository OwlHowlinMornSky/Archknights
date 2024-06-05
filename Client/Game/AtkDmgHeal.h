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
	EntityLocationType sourceAd;
	EntityIdType sourceId;

	AttackData() :
		distType(DistanceType::Near),
		damage(),
		sourceAd(0),
		sourceId(0) {}
};

struct HealData {
	Attribute::ValueType healValue;

	HealData() :
		healValue(0.0f) {}
};

}
