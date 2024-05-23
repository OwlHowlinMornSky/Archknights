#include "char_101_sora_factory.h"

#include "char_101_sora.h"
#include "char_101_sora_actor.h"

bool Game::Char_101_Sora_Factory::Load() {
	auto fac = ohms::ISpineFactory::Instance();

	bool res = fac->CreatePose(m_pose[0], "char_101_sora", 0);
	if (!res)
		return false;

	return true;
}

bool Game::Char_101_Sora_Factory::CreateEntity(std::shared_ptr<Entity>& ptr) {
	auto unit = std::make_shared<Units::Char_101_Sora>();

	auto anim0 = m_pose[0]->CreateAnimation();

	auto actor = std::make_shared<Char_101_Sora_Actor_Vanilla>(anim0);

	Game::IActorGroup::Instance()->AddActor(actor);

	unit->m_actor = actor;

	ptr = unit;

	return true;
}


namespace EntityFactoryLink {

std::unique_ptr<Game::EntityFactory> Create101() {
	std::unique_ptr<Game::EntityFactory> res;
	res = std::make_unique<Game::Char_101_Sora_Factory>();
	return std::move(res);
}

}
