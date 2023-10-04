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
*     Win32Things.h : 声明有关Win32的基本行为。
*/
#pragma once

#include "framework.h"

namespace SystemThings {

/**
 * @brief 检查系统错误（GetLastError）。
 * @param lpszFunction: 提示词。调试的时候就知道什么东西出了什么错。
*/
void WinCheckError(LPCWSTR lpszFunction) noexcept;

/**
 * @brief 注册窗口类。创建窗口前先调用这个。
 * @param hInstance: 进程句柄。
 * @return 创建是否成功。
*/
bool MyRegisterClass(HINSTANCE hInstance) noexcept;

/**
 * @brief 创建窗口。调用前请先注册窗口类。
 * @brief 注意：只能创建一个窗口，多个窗口的表现未知。
 * @param hInstance: 进程句柄。
 * @param nCmdShow: ShowWindow 用的那个。WinMain 的形参肯定有。
 * @param hWnd: [Out] 窗口句柄。
 * @return 创建是否成功。
*/
bool MyCreateWindow(HINSTANCE hInstance, int nCmdShow, HWND& hWnd) noexcept;

/**
 * @brief 注销窗口类。
 * @param hInstance: 进程句柄。
*/
void MyUnregisterClass(HINSTANCE hInstance) noexcept;

} // namespace SystemThings
