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
#include "UniqueInstance.h"

#include <mutex>

// 共享数据段。
#pragma data_seg("OHMS_EUI")
std::mutex ___OHMS_EnsureUniqueInstance_Mutex; // 互斥体。
bool ___OHMS_EnsureUniqueInstance_instanceExist = false; // 是否存在实例。
#pragma data_seg()
// R: Read, W: Write, S: Shared.
#pragma comment(linker, "/SECTION:OHMS_EUI,RWS")
/**
 * 简单来说就是通过共享数据段来标记实例。
*/

namespace AppWin32 {

bool AppWin32::uniqueInstance() noexcept {
	std::lock_guard<std::mutex> guard(___OHMS_EnsureUniqueInstance_Mutex);
	if (___OHMS_EnsureUniqueInstance_instanceExist) {
		return false;
	}
	___OHMS_EnsureUniqueInstance_instanceExist = true;
    return true;
}

void AppWin32::instanceExit() noexcept {
	std::lock_guard<std::mutex> guard(___OHMS_EnsureUniqueInstance_Mutex);
	___OHMS_EnsureUniqueInstance_instanceExist = false;
	return;
}

} // namespace AppWin32
