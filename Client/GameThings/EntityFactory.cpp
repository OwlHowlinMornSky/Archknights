#include "EntityFactory.h"

namespace EntityFactoryLink {

std::unique_ptr<Game::EntityFactory> Create101();
std::unique_ptr<Game::EntityFactory> Create128();
std::unique_ptr<Game::EntityFactory> Create151();

}

bool Game::EntityFactory::Create(std::unique_ptr<Game::EntityFactory>& ptr, size_t entityId) {
	switch (entityId) {
	case 101:
		ptr = EntityFactoryLink::Create101();
		break;
	case 128:
		ptr = EntityFactoryLink::Create128();
		break;
	case 151:
		ptr = EntityFactoryLink::Create151();
		break;
	default:
		return false;
	}
    return true;
}
