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
#include "../GUI_Activities/DefaultEntryDebug.h"
#else
#include "../GUI_Activities/DefaultEntry.h"
#endif

#include "../GUI_Activities/TestActivity.h"

#include "../GUI_Activities/ActivityIDs.h"

namespace GUI {

std::unique_ptr<IActivity> CarnivalWin32::createActivity(size_t id) const {
	switch (id) {
	case Activity::ID_DefaultEntry:
#ifdef _DEBUG
		return std::make_unique<Activity::DefaultEntryDebug>();
#else
		return std::make_unique<Activity::DefaultEntry>();
#endif
		break;
	default:
//#ifdef _DEBUG
		return std::make_unique<Activity::TestActivity>(id);
//#endif
		break;
	}
	return nullptr;
}

}
