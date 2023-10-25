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
#include "../G3D/base.h"
#include "framework.h"

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow
) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (!AppWin32::uniqueInstance()) {
		MessageBoxA(NULL, "Another instance exists.", "Archknights: Information", MB_ICONINFORMATION);
		return 0;
	}
	GUI::Carnival::initialize();
	GUI::Carnival& carnival = GUI::Carnival::instance();
	g3d::base::setup();
	g3d::base::setActive(false);
	try {
		std::unique_ptr<GUI::Window> window = std::make_unique<GUI::WindowWin32>(nCmdShow);
		if (window->Create()) {
			window->setVerticalSyncEnabled(true);
			window->setActivity(std::make_unique<Activity::Act01_DefaultEntry>());
			carnival.addWindow(std::move(window));
			carnival.run();
		}
	}
	catch (std::exception& exp) {
		MessageBoxA(NULL, exp.what(), "Archknights: Fatal Error", MB_ICONERROR);
	}
	catch (...) {
		MessageBoxA(NULL, "Unknown exception.", "Archknights: Fatal Error", MB_ICONERROR);
	}
	g3d::base::drop();
	GUI::Carnival::drop();
	AppWin32::instanceExit();
	return 0;
}
