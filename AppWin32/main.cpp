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

#include "resource.h"

#define MAX_LOADSTRING 100

namespace {

WCHAR g_fatal_error[MAX_LOADSTRING];
WCHAR g_information[MAX_LOADSTRING];
WCHAR g_another_instance[MAX_LOADSTRING];
WCHAR g_register_failed[MAX_LOADSTRING];
WCHAR g_create_failed[MAX_LOADSTRING];
WCHAR g_unknown_exception[MAX_LOADSTRING];

}

/**
 * @brief 主函数。
*/
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					  _In_opt_ HINSTANCE hPrevInstance,
					  _In_ LPWSTR    lpCmdLine,
					  _In_ int       nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_FATAL_ERROR, g_fatal_error, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_INFORMATION, g_information, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_INIT_ANOTHER_INSTANCE, g_another_instance, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_INIT_REGISTER_FAILED, g_register_failed, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_INIT_CREATE_WINDOW_FAILED, g_create_failed, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_UNKNOWN_EXCEPTION, g_unknown_exception, MAX_LOADSTRING);

	// 检验唯一程序实例。
	if (!AppWin32::uniqueInstance()) {
		MessageBoxW(NULL, g_another_instance, g_information, MB_ICONINFORMATION);
		return 0;
	}
	// 注册窗口类。
	if (!SystemThings::MyRegisterClass(hInstance)) {
		MessageBoxW(NULL, g_register_failed, g_fatal_error, MB_ICONERROR);
		return 1;
	}
	// 创建窗口。
	HWND hWnd(NULL);
	if (!SystemThings::MyCreateWindow(hInstance, nCmdShow, hWnd)) {
		MessageBoxW(NULL, g_create_failed, g_fatal_error, MB_ICONERROR);
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
		MessageBoxW(hWnd, g_unknown_exception, g_fatal_error, MB_ICONERROR);
	}
	// 清理。
	DestroyWindow(hWnd);
	SystemThings::MyUnregisterClass(hInstance);
	AppWin32::instanceExit();
	return 0;
}
