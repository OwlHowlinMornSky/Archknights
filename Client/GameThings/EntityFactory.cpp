#include "EntityFactory.h"

#include "char_101_sora.h"

namespace EntityFactoryLink {

//std::unique_ptr<Game::EntityFactory> Create101();
std::unique_ptr<Game::EntityFactory> Create151();

}

std::unique_ptr<Game::EntityFactory> Game::EntityFactory::Create(size_t entityId) {
	switch (entityId) {
	case 101:
		return std::make_unique<Char_101_Sora_Factory>();
	case 151:
		return EntityFactoryLink::Create151();
	}
    return std::unique_ptr<EntityFactory>();
}
