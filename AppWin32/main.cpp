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
*     main.cpp : 定义应用程序的入口点。
*/
#include "../Global/GlobalAttribute.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include "Win32Things.h"
#include "../GUI/Callbacks.h"
#include "CarnivalWin32/ToCarnival.h"

#include <memory>

#include "UniqueInstance.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					  _In_opt_ HINSTANCE hPrevInstance,
					  _In_ LPWSTR    lpCmdLine,
					  _In_ int       nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (!AppWin32::uniqueInstance()) {
		MessageBoxW(NULL,
					L"Initialization failed:\nanother instance exists.",
					L"Archknights: Fatal Error",
					MB_ICONERROR);
		return 1;
	}

	if (!SystemThings::MyRegisterClass(hInstance)) {
		MessageBoxW(NULL,
					L"Initialization failed:\ncannot register class.",
					L"Archknights: Fatal Error",
					MB_ICONERROR);
		return 1;
	}

	HWND hWnd(NULL);
	if (!SystemThings::MyCreateWindow(hInstance, nCmdShow, hWnd)) {
		MessageBoxW(NULL,
					L"Initialization failed:\ncannot create window.",
					L"Archknights: Fatal Error",
					MB_ICONERROR);
		SystemThings::MyUnregisterClass(hInstance);
		return 1;
	}

	std::unique_ptr<sf::RenderWindow> window = std::make_unique<sf::RenderWindow>();
	window->create(hWnd);
	if (!window->isOpen()) {
		MessageBoxW(NULL,
					L"Initialization failed:\ncannot initialize the window.",
					L"Archknights: Fatal Error",
					MB_ICONERROR);
		DestroyWindow(hWnd);
		SystemThings::MyUnregisterClass(hInstance);
		return 1;
	}
	window->clear();
	window->display();

	RECT clientrect{ 0 };
	POINT oldsize;
	GetClientRect(hWnd, &clientrect);
	oldsize = { clientrect.right, clientrect.bottom };

	Callbacks::OnSizing = [&clientrect, &oldsize, &hWnd, &window]() -> void {
		GetClientRect(hWnd, &clientrect);
		if (oldsize.x != clientrect.right || oldsize.y != clientrect.bottom) {
			oldsize = { clientrect.right, clientrect.bottom };
			window->setSize({ (unsigned int)clientrect.right, (unsigned int)clientrect.bottom });
		}
	};

	try {
		std::unique_ptr<GUI::ICarnival> carnival = AppWin32::crateCarnival(hWnd, window.get());
		carnival->run();
	}
	catch (std::exception& exp) {
		MessageBoxA(hWnd, exp.what(), "Archknights: Fatal Error", MB_ICONERROR);
	}
	catch (...) {
		MessageBoxW(hWnd, L"Unknown error.", L"Archknights: Fatal Error", MB_ICONERROR);
	}

	window->close();
	DestroyWindow(hWnd);
	SystemThings::MyUnregisterClass(hInstance);

	AppWin32::instanceExit();
	return 0;
}
