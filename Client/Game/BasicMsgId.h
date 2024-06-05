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

namespace Game {
namespace MsgId {
enum Basic : MsgIdType {
	Null = 0,
	GuiEvent,  // GUI消息。 wparam：未使用； lparam：指向 sf::Event 的指针。
	Summon,    // 召唤实体。 wparam：实体编号； lparam：float 指针，指向 float[2]。

	//ENTITY_CHANGES = 0x00010000,
	OnHpChanging,   // 生命值改变（尚未限制）。 wparam：未使用； lparam：未使用。
	OnHpChanged,    // 生命值已改变（已限制）。 wparam：未使用； lparam：未使用。
	OnHpDropToZero, // 生命值降至零。 wparam：未使用； lparam：未使用。

	OnAttackBegin,
	OnAttackEnd,
	OnGetAttack, // 受到攻击。 wparam：未使用； lparam：指向 AttackData 结构体的指针。
	OnGetHeal,   // 受到治疗。 wparam：未使用； lparam：指向 HealData 结构体的指针。
	OnGetDamage, // 受到伤害。 wparam：未使用； lparam：指向 DamageData 结构体的指针。

	EXTERN_START,// = 0x01000000
};
}
}
