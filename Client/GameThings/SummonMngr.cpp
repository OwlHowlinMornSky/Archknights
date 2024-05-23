#include "SummonMngr.h"

#include "../Game/MsgResult.h"
#include "../Game/GameGlobal.h"
#include "../Game/GameBoard.h"
#include "CreateInfoForUnit.h"
#include "../Models/Actor.h"
#include <MysteryEngine/G3D/G3dGlobal.h>

namespace Game {

SummonMngr::SummonMngr() {}

SummonMngr::~SummonMngr() {}

void SummonMngr::AddBegin() {
	m_animFactory = ohms::ISpineFactory::Create();
	return;
}

void SummonMngr::AddEntity(size_t id, std::string_view testname, bool test) {
	SummonData data;
	bool res;

	data.animDouble = test;
	data.id = id;

	if (test) {
		res = m_animFactory->CreatePose2(data.animPose[0], data.animPose[1], testname);
	}
	else {
		res = m_animFactory->CreatePose(data.animPose[0], testname, 0);
	}

	data.factory = EntityFactory::Create(id);

	m_data.push_back(std::move(data));
	return;
}

void SummonMngr::AddEnd() {
	m_animFactory.reset();
	return;
}

void SummonMngr::OnJoined() {
	GameGlobal::board->SubscribeMsg(2, m_location);
}

void SummonMngr::OnKicking() {
	GameGlobal::board->UnsubscribeMsg(2, m_location);
}

void SummonMngr::FixedUpdate(float dt) {}

MsgResultType SummonMngr::ReceiveMessage(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam) {
	switch (msg) {
	case 2:
		if (wparam < m_data.size()) {
			SummonData& data = m_data[wparam];
			CreateInfoForUnit info;

			auto actorGroup = Game::IActorGroup::Instance();
			ME::G3dGlobal::setActive(true);
			if (data.animDouble) {
				auto actor = std::make_shared<ActorSpine2>(
					data.animPose[0]->CreateAnimation(),
					data.animPose[1]->CreateAnimation()
				);
				actorGroup->AddActor(actor);
				info.actor = actor;
			}
			else {
				auto actor = std::make_shared<ActorSpine>(
					data.animPose[0]->CreateAnimation()
				);
				actorGroup->AddActor(actor);
				info.actor = actor;
			}
			ME::G3dGlobal::setActive(false);

			std::shared_ptr<Entity> entity;
			data.factory->CreateEntity(entity, &info);

			entity->setPosition({ lparam, 0.0f });

			GameGlobal::board->JoinEntity(entity);
		}
		break;
	default:
		return MsgResult::Unsubscribe;
	}
	return MsgResult::OK;
}

}

std::shared_ptr<Game::ISummonMngr> Game::ISummonMngr::Create() {
	return std::make_shared<Game::SummonMngr>();
}
