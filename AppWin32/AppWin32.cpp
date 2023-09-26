/* AppWin32.cpp : 定义应用程序的入口点。
*
*/
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

	MSG msg;

	// 主消息循环:
	while (GetMessageW(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	DestroyWindow(hWnd);
	SystemThings::MyUnregisterClass(hInstance);
	return (int)msg.wParam;
}
