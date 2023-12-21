#include "RootLoader.h"

namespace game {

void RootLoader::OnJoined(size_t id, size_t location) {
	Parent::OnJoined(id, location);

}

void RootLoader::OnUpdate(size_t dt) {}

void RootLoader::OnKicking() {
	return Parent::OnKicking();
}

}
