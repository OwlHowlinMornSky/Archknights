#include "MessageRepeater.h"

namespace game {

void MessageRepeater::OnClosing() {
	for (listener::OnClosing*& c : onClosing)
		c->trigger();
}

size_t MessageRepeater::register_OnClosing(listener::OnClosing* c) {
	onClosing.emplace_back(c);
	return onClosing.size() - 1;
}

void MessageRepeater::unregister_OnClosing(size_t id) {
	onClosing[id] = onClosing.back();
	onClosing.pop_back();
}

}
