#pragma once

#include "framework.h"

namespace SystemThings {

void WinCheckError(LPCWSTR lpszFunction);

bool MyRegisterClass(HINSTANCE hInstance);

bool MyCreateWindow(HINSTANCE hInstance, int nCmdShow, HWND& hWnd);

void MyUnregisterClass(HINSTANCE hInstance);

} // namespace SystemThings
