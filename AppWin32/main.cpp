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
#include "..\Global\GlobalAttribute.h"
#include <SFML/Graphics.hpp>
#include "Win32Things.h"
#include "Callbacks.h"
#include "resource.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					  _In_opt_ HINSTANCE hPrevInstance,
					  _In_ LPWSTR    lpCmdLine,
					  _In_ int       nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

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

	sf::Context* context = new sf::Context;
	context->setActive(true);
	sf::RenderWindow* window = new sf::RenderWindow;
	window->create(hWnd);
	if (!window->isOpen()) {
		MessageBoxW(NULL,
					L"Initialization failed:\ncannot initialize the window.",
					L"Archknights: Fatal Error",
					MB_ICONERROR);
		context->setActive(false);
		delete context;
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
		sf::RectangleShape shape;
		shape.setFillColor(sf::Color::Red);
		shape.setSize({ 100.0f, 100.0f });
		shape.setPosition({ 400.0f, 300.0f });

		window->setFramerateLimit(60);
		bool run = true;
		MSG msg{ 0 };
		sf::Event evt;

		sf::Clock clk;
		float dt;

		Callbacks::OnIdle = [&window, &dt, &clk, &shape]() -> void {
			dt = clk.restart().asSeconds();
			shape.rotate(dt * 90.0f);

			window->clear();
			window->draw(shape);
			window->display();
		};

		while (run) {
			while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
			while (window->pollEvent(evt)) {
				switch (evt.type) {
				case sf::Event::Closed:
					run = false;
					break;
				default:
					break;
				}
			}

			dt = clk.restart().asSeconds();
			shape.rotate(dt * 90.0f);

			window->clear();
			window->draw(shape);
			window->display();
		}

		//ohms::ui::Desktop::init();
		//ohms::ui::Desktop::instance().browse(window, std::move(ohms::appWin32::GetInitUI()));
		//ohms::ui::Desktop::drop();
	}
	catch (std::exception& exp) {
		MessageBoxA(hWnd, exp.what(), "Archknights: Fatal Error", MB_ICONERROR);
	}
	catch (...) {
		MessageBoxW(hWnd, L"Unknown error.", L"Archknights: Fatal Error", MB_ICONERROR);
	}

	window->close();
	delete window;
	context->setActive(false);
	delete context;
	DestroyWindow(hWnd);
	SystemThings::MyUnregisterClass(hInstance);
	return 0;
}
