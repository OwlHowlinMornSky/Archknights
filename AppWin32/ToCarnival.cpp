
#include <memory>

#include "ToCarnival.h"

#include "CarnivalWin32.h"

namespace AppWin32 {

std::unique_ptr<GUI::ICarnival> crateCarnival(HWND hwnd, sf::RenderWindow* r_window) {
	return std::make_unique<GUI::CarnivalWin32>(hwnd, r_window);
}

}
