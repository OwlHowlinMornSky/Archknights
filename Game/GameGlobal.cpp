#include "GameGlobal.h"
#include "GameBoard.h"

namespace {

game::Global* g_instance = nullptr;

}

namespace game {

Global* Global::instance() {
	if (!::g_instance) {
		::g_instance = new Global;
	}
	return ::g_instance;
}

void Global::drop() {
	delete ::g_instance;
}

}
