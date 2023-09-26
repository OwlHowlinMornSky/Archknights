/* AppWin32.cpp : 定义应用程序的入口点。
*
*/
#include <SFML/Graphics.hpp>
#include "Win32Things.h"
#include "AppWin32.h"

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
		return 1;
	}

	sf::Context context;
	context.setActive(true);
	sf::RenderWindow window;
	window.create(hWnd);
	if (!window.isOpen()) {
		MessageBoxW(NULL,
					L"Initialization failed:\ncannot initialize the window.",
					L"Archknights: Fatal Error",
					MB_ICONERROR);
		return 1;
	}
	window.clear();
	window.display();

	RECT clientrect{ 0 };
	POINT oldsize;
	GetClientRect(hWnd, &clientrect);
	oldsize = { clientrect.right, clientrect.bottom };

	SystemThings::fOnSizing = [&clientrect, &oldsize, &hWnd, &window]() -> void {
		GetClientRect(hWnd, &clientrect);
		if (oldsize.x != clientrect.right || oldsize.y != clientrect.bottom) {
			oldsize = { clientrect.right, clientrect.bottom };
			window.setSize({ (unsigned int)clientrect.right, (unsigned int)clientrect.bottom });
		}
		return;
		};

	try {
		window.setFramerateLimit(60);
		bool run = true;
		MSG msg{ 0 };
		sf::Event evt;

		while (run) {
			while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
			while (window.pollEvent(evt)) {
				switch (evt.type) {
				case sf::Event::Closed:
					run = false;
					break;
				default:
					break;
				}
			}

			window.clear();
			window.display();
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

	window.close();
	context.setActive(false);
	DestroyWindow(hWnd);
	SystemThings::MyUnregisterClass(hInstance);
	return 0;
}
