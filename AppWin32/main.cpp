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
#include "UniqueInstance.h"
#include "../GUI/CarnivalWin32.h"
#include "../GUI/WindowWin32.h"
#include "../GUI_Activities/Act01_DefaultEntry.h"
#include "framework.h"
#include "resource.h"

#define MAX_LOADSTRING 64

namespace {

CHAR g_fatal_errorA[MAX_LOADSTRING];
WCHAR g_fatal_error[MAX_LOADSTRING];
WCHAR g_information[MAX_LOADSTRING];
WCHAR g_another_instance[MAX_LOADSTRING];
WCHAR g_register_failed[MAX_LOADSTRING];
WCHAR g_create_failed[MAX_LOADSTRING];
WCHAR g_unknown_exception[MAX_LOADSTRING];

} // namespace

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow
) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringA(hInstance, IDS_FATAL_ERROR, g_fatal_errorA, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_FATAL_ERROR, g_fatal_error, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_INFORMATION, g_information, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_INIT_ANOTHER_INSTANCE, g_another_instance, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_INIT_REGISTER_FAILED, g_register_failed, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_INIT_CREATE_WINDOW_FAILED, g_create_failed, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_UNKNOWN_EXCEPTION, g_unknown_exception, MAX_LOADSTRING);

	if (!AppWin32::uniqueInstance()) {
		MessageBoxW(NULL, g_another_instance, g_information, MB_ICONINFORMATION);
		return 0;
	}
	try {
		std::unique_ptr<GUI::Carnival> carnival =
			std::make_unique<GUI::CarnivalWin32>();
		std::unique_ptr<GUI::Window> window =
			std::make_unique<GUI::WindowWin32>();
		std::unique_ptr<GUI::Window> window2 =
			std::make_unique<GUI::WindowWin32>();
		if (window->Open() && window2->Open()) {

			window->setActivity(std::make_unique<Activity::Act01_DefaultEntry>());
			window2->setActivity(std::make_unique<Activity::Act01_DefaultEntry>());

			carnival->addWindow(std::move(window));
			carnival->addWindow(std::move(window2));

			carnival->run();
		}
		carnival.reset();
	}
	catch (std::exception& exp) {
		MessageBoxA(NULL, exp.what(), g_fatal_errorA, MB_ICONERROR);
	}
	catch (...) {
		MessageBoxW(NULL, g_unknown_exception, g_fatal_error, MB_ICONERROR);
	}
	AppWin32::instanceExit();
	return 0;
}
