#include "Global.h"

namespace {

gamegui::Global* g_instance = nullptr;

}

namespace gamegui {

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
