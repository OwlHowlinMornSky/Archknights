#pragma once

#include "Entity.h"
#include <deque>
#include "MessageListeners.h"

namespace game {

class MessageRepeater :
	public Entity {
public:
	void OnClosing();

public:
	size_t register_OnClosing(listener::OnClosing* c);
	void unregister_OnClosing(size_t id);

protected:
	std::deque<listener::OnClosing*> onClosing;

};

}
