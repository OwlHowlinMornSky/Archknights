#include "RootLoader.h"
#include "GameGlobal.h"
#include "GameBoard.h"
#include "Char001_Myrtle.h"

namespace game {

void RootLoader::OnJoined(size_t id, size_t location) {
	Parent::OnJoined(id, location);

	// for test
	Global::instance()->data.board->JoinEntity(std::make_shared<Char001_Myrtle_Factory>());

	return;
}

void RootLoader::OnUpdate(float dt) {}

void RootLoader::OnKicking() {
	return Parent::OnKicking();
}

}
