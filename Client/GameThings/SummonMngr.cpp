#include "SummonMngr.h"

#include "../Game/MsgResult.h"
#include "../Game/GameGlobal.h"
#include "../Game/GameBoard.h"
#include "../Models/IAnimationSpine.h"
#include "MsgId.h"

namespace Game {

SummonMngr::SummonMngr() {}

SummonMngr::~SummonMngr() {}

void SummonMngr::AddBegin() {
	ohms::ISpineFactory::Instance();
	return;
}

bool SummonMngr::AddEntity(size_t id) {
	SummonData data;
	data.id = id;

	data.factory = EntityFactory::Create(id);
	if (data.factory == nullptr) {
		return false;
	}
	if (!data.factory->Load()) {
		return false;
	}

	m_data.push_back(std::move(data));
	return true;
}

void SummonMngr::AddEnd() {
	ohms::ISpineFactory::Drop();
	return;
}

void SummonMngr::OnJoined() {
	GameGlobal::board->SubscribeMsg(MsgId::Summon, m_location);
}

void SummonMngr::OnKicking() {
	GameGlobal::board->UnsubscribeMsg(MsgId::Summon, m_location);
}

void SummonMngr::FixedUpdate(float dt) {}

MsgResultType SummonMngr::ReceiveMessage(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam) {
	switch (msg) {
	case MsgId::Summon:
		if (wparam < m_data.size()) {
			SummonData& data = m_data[wparam];

			std::shared_ptr<Entity> entity;
			data.factory->CreateEntity(entity);

			float* pos = (float*)lparam;

			entity->setPosition(pos[0], pos[1]); // test

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
