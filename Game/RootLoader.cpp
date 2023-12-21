#include "RootLoader.h"

namespace game {

void RootLoader::OnJoined(size_t id, size_t location) {
	Parent::OnJoined(id, location);

}

void RootLoader::OnUpdate(float dt) {}

void RootLoader::OnKicking() {
	return Parent::OnKicking();
}

}
