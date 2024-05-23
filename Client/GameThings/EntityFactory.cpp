#include "EntityFactory.h"

namespace EntityFactoryLink {

std::unique_ptr<Game::EntityFactory> Create101();
std::unique_ptr<Game::EntityFactory> Create128();
std::unique_ptr<Game::EntityFactory> Create151();

}

std::unique_ptr<Game::EntityFactory> Game::EntityFactory::Create(size_t entityId) {
	switch (entityId) {
	case 101:
		return EntityFactoryLink::Create101();
	case 128:
		return EntityFactoryLink::Create128();
	case 151:
		return EntityFactoryLink::Create151();
	}
    return std::unique_ptr<EntityFactory>();
}
