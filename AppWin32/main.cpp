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
*     main.cpp : 定义应用程序的入口点。
*/
#include "../Global/GlobalAttribute.h"
#include "UniqueInstance.h"
#include "FactoryCarnival.h"
#include "Win32Things.h"

/**
 * @brief 主函数。
*/
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					  _In_opt_ HINSTANCE hPrevInstance,
					  _In_ LPWSTR    lpCmdLine,
					  _In_ int       nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	// 检验唯一程序实例。
	if (!AppWin32::uniqueInstance()) {
		MessageBoxA(NULL,
					"Another instance exists.",
					"Archknights: Information",
					MB_ICONINFORMATION);
		return 0;
	}
	// 注册窗口类。
	if (!SystemThings::MyRegisterClass(hInstance)) {
		MessageBoxA(NULL,
					"Initialization failed:\ncannot register class.",
					"Archknights: Fatal Error",
					MB_ICONERROR);
		return 1;
	}
	// 创建窗口。
	HWND hWnd(NULL);
	if (!SystemThings::MyCreateWindow(hInstance, nCmdShow, hWnd)) {
		MessageBoxA(NULL,
					"Initialization failed:\ncannot create window.",
					"Archknights: Fatal Error",
					MB_ICONERROR);
		SystemThings::MyUnregisterClass(hInstance);
		return 2;
	}
	// 运行 Carnival
	try {
		std::unique_ptr<GUI::ICarnival> carnival = AppWin32::Factory::crateCarnival(hWnd);
		carnival->run();
		carnival.reset();
	}
	catch (std::exception& exp) {
		MessageBoxA(hWnd, exp.what(), "Archknights: Fatal Error", MB_ICONERROR);
	}
	catch (...) {
		MessageBoxA(hWnd, "Unknown error.", "Archknights: Fatal Error", MB_ICONERROR);
	}
	// 清理。
	DestroyWindow(hWnd);
	SystemThings::MyUnregisterClass(hInstance);
	AppWin32::instanceExit();
	return 0;
}
