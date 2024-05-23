#include "char_128_plosis_factory.h"

#include "char_128_plosis.h"
#include "char_128_plosis_actor.h"

bool Game::Char_128_Plosis_Factory::Load() {
	auto fac = ohms::ISpineFactory::Instance();

	test = false;

	if (test) {
		char res = fac->CreatePose2(m_pose[0], m_pose[1], "char_128_plosis_epoque#3");
		if (res != 3)
			return false;
	}
	else {
		char res = fac->CreatePose2(m_pose[0], m_pose[1], "char_128_plosis");
		if (res != 3)
			return false;
	}

	return true;
}

bool Game::Char_128_Plosis_Factory::CreateEntity(std::shared_ptr<Entity>& ptr) {
	auto unit = std::make_shared<Units::Char_128_Plosis>();


	auto anim0 = m_pose[0]->CreateAnimation();
	auto anim1 = m_pose[1]->CreateAnimation();

	if (test) {
		auto actor = std::make_shared<Char_128_Plosis_Actor_Epoque>(anim0, anim1);
		Game::IActorGroup::Instance()->AddActor(actor);
		unit->m_actor = actor;
	}
	else {
		auto actor = std::make_shared<Char_128_Plosis_Actor_Vanilla>(anim0, anim1);
		Game::IActorGroup::Instance()->AddActor(actor);
		unit->m_actor = actor;
	}

	ptr = unit;

	return true;
}

namespace EntityFactoryLink {

std::unique_ptr<Game::EntityFactory> Create128() {
	std::unique_ptr<Game::EntityFactory> res;
	res = std::make_unique<Game::Char_128_Plosis_Factory>();
	return std::move(res);
}

}
