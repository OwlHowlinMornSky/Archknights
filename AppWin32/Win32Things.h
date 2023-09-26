/*
*                    GNU AFFERO GENERAL PUBLIC LICENSE
*                       Version 3, 19 November 2007
*
*    Copyright (c) 2023  Tyler Parret True
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
*     Tyler Parret True (OwlHowlinMornSky) <mysteryworldgod@outlook.com>
*
* @Description
*     Win32Things.cpp : �����й�Win32�Ļ�����Ϊ��
*/
#pragma once

#include "framework.h"

#include <functional>

namespace SystemThings {

extern std::function<void()> fOnSizing;
extern std::function<void()> fOnIdle;

void WinCheckError(LPCWSTR lpszFunction);

bool MyRegisterClass(HINSTANCE hInstance);

bool MyCreateWindow(HINSTANCE hInstance, int nCmdShow, HWND& hWnd);

void MyUnregisterClass(HINSTANCE hInstance);

} // namespace SystemThings
