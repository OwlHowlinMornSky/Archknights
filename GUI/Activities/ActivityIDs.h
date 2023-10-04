/*
*    Archknights
*
*    Copyright (C) 2023  Tyler Parret True
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

namespace GUI {

/**
 * @brief Activity 标识。
*/
enum IDs : size_t {
	ID_None = 0ull, //------------// 空。
	ID_DefaultEntry, //-----------// 默认入口。
	ID_Load, //-------------------// 加载界面。
	ID_Title, //------------------// 标题界面。
	ID_Main, //-------------------// 主界面。
	ID_Panel, //------------------// 终端。
	ID_Construction, //-----------// 基建。

	ID_CTRL_BASE_COUNT, //--------// [标记] 基本界面 数量。

	ID_DynamicBlock = 0x0080, //--// [标记] 动态界面区块 起始标记。

	ID_RESERVED_COUNT = 0x0400 //-// [标记] 保留区 数量。在这之上的 ID 任意使用。
};

}
