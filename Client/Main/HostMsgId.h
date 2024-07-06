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

#include "../Game/TypeDef.h"

namespace Main::HostMsgId {

enum Id : Game::MsgIdType {
	Null = 0,

	MapInitOk,            // null.
	MapInitCheckpointCnt, // wParam: Count.
	MapInitCheckpointSet, // wParam: ID; lParam: int* to an int[2] array.
	MapLeadQuery,         // wParam: Checkpoint ID; lParam: int* to an int[2] array.
	MapStatusChanged,     // null.
	MapIncRef,            // wParam: ID.
	MapDecRef,            // wParam: ID.

	HitTest,           // 测试指定直线是否击中. wParam: 未使用; lParam: 指向 HitTestData 结构的指针.
	QueryDeployable,   // 询问某个位置是否可以部署指定类型单位. wParam: 子对象id; lParam: 指向 QueryDeployableData 结构的指针.
	TryToOccupy,       // 尝试占用位置, 若位置已被占用则失败. wParam: 子对象id; lParam: 指向 TryToOccupyData 结构的指针.
	SetOccupationAt,   // 修改位置的占用者. wParam: 子对象id; lParam: 指向 SetOccupationData 结构的指针.
	ClearOccupation,   // 清除占用，不会撤退占用单位. wParam: 子对象id; lParam: 未使用.
	RetreatOccupation, // 清除占用，若有占用单位则撤退之. wParam: 子对象id; lParam: 未使用.

};

}
