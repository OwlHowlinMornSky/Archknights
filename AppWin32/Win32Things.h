#pragma once

#include "framework.h"

#include <functional>

namespace SystemThings {

extern std::function<void()> fOnSizing;

void WinCheckError(LPCWSTR lpszFunction);

bool MyRegisterClass(HINSTANCE hInstance);

bool MyCreateWindow(HINSTANCE hInstance, int nCmdShow, HWND& hWnd);

void MyUnregisterClass(HINSTANCE hInstance);

} // namespace SystemThings
