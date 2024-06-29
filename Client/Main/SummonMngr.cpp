/*
*    Archknights
*
*    Copyright (C) 2023-2024  Tyler Parret True
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Affero General Public License as published
*    by the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Affero General Public License for more details.
*
*    You should have received a copy of the GNU Affero General Public License
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
* @Authors
*    Tyler Parret True <mysteryworldgod@outlook.com><https://github.com/OwlHowlinMornSky>
*/
#include "SummonMngr.h"

#include "../Game/MsgResult.h"
#include "../Game/Global.h"
#include "../Game/Board.h"

#include "../Models/IAnimation.h"

#include "MsgId.h"

namespace Game {

SummonMngr::SummonMngr() {}

SummonMngr::~SummonMngr() {}

void SummonMngr::AddBegin() {
	Model::IAnimationFactory::Instance();
	return;
}

bool SummonMngr::AddEntity(size_t id, bool isEnemy) {
	SummonData data;
	data.id = id;

	if (isEnemy) {
		if (!EntityFactory::CreateEnemy(data.factory, id)) {
			return false;
		}
	}
	else {
		if (!EntityFactory::Create(data.factory, id)) {
			return false;
		}
	}
	if (!data.factory->Load()) {
		return false;
	}

	m_data.push_back(std::move(data));
	return true;
}

void SummonMngr::AddEnd() {
	Model::IAnimationFactory::Drop();
	return;
}

void SummonMngr::OnJoined() {
	GameGlobal::board->SubscribeMsg(MsgId::Summon, m_location);
}

void SummonMngr::OnKicking() {
	GameGlobal::board->UnsubscribeMsg(MsgId::Summon, m_location);
}

void SummonMngr::FixedUpdate() {}

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
