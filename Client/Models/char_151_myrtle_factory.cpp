#include "char_151_myrtle_factory.h"

#include "../GameThings/char_151_myrtle.h"
#include "char_151_myrtle_actor.h"

namespace Game {

bool Char_151_Mytle_Factory::Load() {
	auto fac = ohms::ISpineFactory::Instance();

	char res = fac->CreatePose2(m_pose[0], m_pose[1], "char_151_myrtle");
	if (res != 3)
		return false;

	return true;
}

bool Char_151_Mytle_Factory::CreateEntity(std::shared_ptr<Entity>& ptr, void* createInfo) {
	auto unit = std::make_shared<Units::Char_151_Myrtle>();


	auto anim0 = m_pose[0]->CreateAnimation();
	auto anim1 = m_pose[1]->CreateAnimation();

	auto actor = std::make_shared<Char_151_Mytle_Actor_Vanilla>(anim0, anim1);

	Game::IActorGroup::Instance()->AddActor(actor);

	unit->m_actor = actor;

	ptr = unit;

	return true;
}

}

namespace EntityFactoryLink {

std::unique_ptr<Game::EntityFactory> Create151() {
	std::unique_ptr<Game::EntityFactory> res;
	res = std::make_unique<Game::Char_151_Mytle_Factory>();
	return std::move(res);
}

}
