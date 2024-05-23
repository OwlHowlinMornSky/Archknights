#include "EntityFactory.h"

#include "char_151_myrtle.h"

std::unique_ptr<Game::EntityFactory> Game::EntityFactory::Create(size_t entityId) {
	switch (entityId) {
	case 151:
		return std::make_unique<Char_151_Myrtle_Factory>();
	}
    return std::unique_ptr<EntityFactory>();
}
