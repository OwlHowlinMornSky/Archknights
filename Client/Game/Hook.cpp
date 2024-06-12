#include "Hook.h"

Game::Hook::Hook() :
	m_running(true) {}

int Game::Hook::CallHook(Entity& entity, EntityMsg& msg) {
	if (!m_running)
		return 1;
	return HookProc(entity, msg);
}
