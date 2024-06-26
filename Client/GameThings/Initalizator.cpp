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
#include "Initalizator.h"

#include "../Game/GameGlobal.h"
#include "../Scenes/Scene_GameCommon.h"
#include "../Game/GameBoard.h"
#include "../Game/GameHost.h"
#include "../Game/MsgResult.h"

//#include <thread>
//#include <functional>

#include <MysteryEngine/G3D/Camera.Perspective.h>
#include <MysteryEngine/G3D/Camera.Oblique.h>
#include <MysteryEngine/G3D/G3dGlobal.h>

#include "../Models/IGround.h"
#include "../Models/IActorGroup.h"

#include "ISummonMngr.h"
#include <SFML/Window/Event.hpp>
#include "MsgId.h"

#include "MapHost.h"
#include "HostMsgId.h"

namespace Game {

void Initalizator::OnJoined() {
	GameGlobal::board->SubscribeMsg(5678, m_location);

	auto camera = std::make_shared<ME::PerspectiveCamera>();
	camera->setAspectRatio(16.0f / 9.0f);
	camera->setFOV(40.0f);
	camera->setPosition(0.0f + 5.5f, -5.5f + 3.5f, 8.66025f);
	camera->setRotation(30.0f, 0.0f, 0.0f);
	/*auto camera = std::make_shared<ME::ObliqueCamera>();
	camera->setZNear(-15.0f);
	camera->setZFar(15.0f);
	camera->setDim(16.0f, 9.0f);
	camera->setSheer(0.5f, 0.5f);
	camera->setPosition(0.0f, -4.0f, 0.1f);
	camera->setRotation(90.0f, 0.0f, 0.0f);*/
	Game::GameGlobal::show->SetCamera(camera);

	////////////////////
	ME::G3dGlobal::setActive(true);

	auto ground = IObjModel::Create();
	ground->LoadModelData("res/main_7-3/main.obj");
	//ground->setRotation(0.0f, 180.0f, 0.0f);
	ground->setScale(-1.0f, 1.0f, -1.0f);
	ground->setPosition(5.5f, 3.5f, 0.0f);

	auto actorGroup = Game::IActorGroup::Instance();

	ME::G3dGlobal::setActive(false);

	Game::GameGlobal::show->AddModel(ground);
	Game::GameGlobal::show->AddModel(actorGroup);
	////////////////////

	auto summonmngr = ISummonMngr::Create();
	GameGlobal::board->JoinEntity(summonmngr);

	summonmngr->AddBegin();
	summonmngr->AddEntity(151);
	summonmngr->AddEntity(101);
	summonmngr->AddEntity(128);
	summonmngr->AddEntity(1002, true);
	summonmngr->AddEnd();

	///////////////////

	pos[0] = 5.5f;
	pos[1] = 3.5f;
	Game::GameGlobal::board->SubscribeMsg(MsgId::GuiEvent, m_location);

	auto maphost = std::make_shared<MapHost>();
	std::ifstream ifs;
	ifs.open("res/main_7-3/map.txt");
	maphost->Load(ifs);

	maphost->ReceiveMessage(HostMsgId::MapInitCheckpointCnt, 1, 0);
	int testp[2] = { 0, 4 };
	maphost->ReceiveMessage(HostMsgId::MapInitCheckpointSet, 0, (intptr_t)&(testp[0]));

	maphost->ReceiveMessage(HostMsgId::MapInitOk, 0, 0);

	Game::GameGlobal::board->SetHost(
		HostJob::MapPathManager,
		maphost
	);
}

void Initalizator::OnKicking() {
	Game::GameGlobal::board->UnsubscribeMsg(MsgId::GuiEvent, m_location);

	GameGlobal::board->Broadcast(1234, 0, 0);
}

void Initalizator::FixedUpdate() {}

MsgResultType Initalizator::ReceiveMessage(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam) {
	switch (msg) {
	case MsgId::GuiEvent:
	{
		auto e = (sf::Event*)lparam;
		switch (e->type) {
		case sf::Event::KeyPressed:
			switch (e->key.code) {
			case sf::Keyboard::Num1:
				GameGlobal::board->PostMsg(2, 0, (intptr_t)this->pos);
				break;
			case sf::Keyboard::Num2:
				GameGlobal::board->PostMsg(2, 1, (intptr_t)this->pos);
				break;
			case sf::Keyboard::Num3:
				GameGlobal::board->PostMsg(2, 2, (intptr_t)this->pos);
				break;
			case sf::Keyboard::Num4:
				GameGlobal::board->PostMsg(2, 3, (intptr_t)this->pos);
				break;
			case sf::Keyboard::Left:
				pos[0] -= 1.0f;
				break;
			case sf::Keyboard::Right:
				pos[0] += 1.0f;
				break;
			case sf::Keyboard::Down:
				pos[1] -= 1.0f;
				break;
			case sf::Keyboard::Up:
				pos[1] += 1.0f;
				break;
			}
			break;
		}
		break;
	}
	case 9876:
		GameGlobal::board->UnsubscribeMsg(5678, m_location);
		KickSelf();
		break;
	case 1234:
		GameGlobal::board->ExitGame(4321);
		break;
	}
	return MsgResult::OK;
}

void Initalizator::LoadThread() {}

}
