#pragma once

#include "../GUI/ICarnival.h"
#include "framework.h"

namespace AppWin32 {

std::unique_ptr<GUI::ICarnival> crateCarnival(HWND hwnd, sf::RenderWindow* r_window);

}
