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
*    ʵ���˴��� Carnival ʵ���ķ�����
*/
#include <memory>

#include "FactoryCarnival.h"

#include "CarnivalWin32.h"

namespace AppWin32::Factory {

std::unique_ptr<GUI::ICarnival> crateCarnival(HWND hWnd) {
	// ֱ�Ӵ�����ûʲô��˵�ġ�
	return std::make_unique<GUI::CarnivalWin32>(hWnd);
}

} // namespace AppWin32