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
#include <SFML/Graphics.hpp>
#include "framework.h"

#include "CarnivalWin32.h"

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

#include "../GUI_Activities/ActivityIDs.h"

#include "../GUI_Activities/Act00_TestActivity.h"

#include "../GUI_Activities/Act01_DefaultEntryDebug.h"
#include "../GUI_Activities/Act01_DefaultEntry.h"

#include "../GUI_Activities/Act02_Load.h"

namespace GUI {

std::unique_ptr<IActivity> CarnivalWin32::createActivity(uint32_t id) const noexcept {
	try {
		switch (id) {
		case Activity::ID_DefaultEntry:// 默认入口。
#ifdef _DEBUG
			return std::make_unique<Activity::DefaultEntryDebug>();
#else
			return std::make_unique<Activity::DefaultEntry>();
#endif
		case Activity::ID_Load: // 加载界面。
			return std::make_unique<Activity::Act02_Load>();
		case Activity::ID_Title: // 标题界面。
		case Activity::ID_Main: // 主界面。
		case Activity::ID_Panel: // 终端。
		case Activity::ID_Construction: // 基建。
		default:
	//#ifdef _DEBUG
			return std::make_unique<Activity::TestActivity>(id);
	//#endif
			break;
		}
	}
#ifdef _DEBUG
	catch (std::exception& e) {
		std::cerr << "Exception: GUI::CarnivalWin32: createActivity failed!" << std::endl
			<< "    " << e.what() << std::endl;
	}
#endif // _DEBUG
	catch (...) {
#ifdef _DEBUG
		std::cerr << "Exception: GUI::CarnivalWin32: createActivity failed!" << std::endl
			<< "    Unknown Exception." << std::endl;
#endif // _DEBUG
	}
	return std::unique_ptr<IActivity>();
}

} // namespace GUI
