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
*
* @Description
*    实现了创建 Carnival 实例的方法。
*/
#include <memory>

#include "ToCarnival.h"

#include "CarnivalWin32.h"

namespace AppWin32 {

std::unique_ptr<GUI::ICarnival> crateCarnival(HWND hwnd, sf::RenderWindow* r_window) {
	// 直接创建，没什么好说的。
	return std::make_unique<GUI::CarnivalWin32>(hwnd, r_window);
}

}
