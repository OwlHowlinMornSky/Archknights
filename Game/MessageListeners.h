#pragma once

namespace game::listener {

class OnClosing {
public: virtual void trigger() = 0;
};

}
