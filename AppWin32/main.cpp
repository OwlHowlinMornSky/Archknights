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
#include <locale>
#include <MysteryEngine/Client/Carnival.h>
#include <MysteryEngine/Client/Window.h>

#include "Activities/Act01_DefaultEntrance.h"

#include "framework.h"
#include "UniqueInstance.h"
#include <MysteryEngine/G3D/G3dGlobal.h>

#include <SFML/GpuPreference.hpp>
SFML_DEFINE_DISCRETE_GPU_PREFERENCE

namespace {

const char g_str_information[] = "Archknights: Information";
const char g_str_unique_inst[] = "Another instance exists.";
const char g_str_fatal_error[] = "Archknights: Fatal Error";
const char g_str_unknown_exp[] = "Unknown exception.";

} // namespace

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow
) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	std::locale::global(std::locale("en-us.utf8"));

	if (!AppWin32::uniqueInstance()) {
		MessageBoxA(NULL, g_str_unique_inst, g_str_information, MB_ICONINFORMATION);
		return 0;
	}
	// Initialize.
	ME::Carnival::setup(false);
	ME::Carnival& carnival = ME::Carnival::instance();
	ME::G3dGlobal::setup();
	try {
		sf::Image icon;
		icon.loadFromFile("assets/icon.png");

		std::unique_ptr<ME::Window> window = ME::Window::Create1Window(nCmdShow);;
		// Create window and run.
		if (window) {
			window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
			window->setVerticalSyncEnabled(true);
			// Start with default activity.
			window->changeActivity(std::make_unique<Activity::Act01_DefaultEntrance>());
			carnival.pushWindow(std::move(window));
			carnival.run();
		}
	}
	catch (std::exception& exp) {
		MessageBoxA(NULL, exp.what(), g_str_fatal_error, MB_ICONERROR);
	}
	catch (...) {
		MessageBoxA(NULL, g_str_unknown_exp, g_str_fatal_error, MB_ICONERROR);
	}
	// Clear.
	ME::G3dGlobal::drop();
	ME::Carnival::drop();
	AppWin32::instanceExit();
	return 0;
}
