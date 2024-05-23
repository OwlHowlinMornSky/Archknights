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
#include "Creator.h"

#include "../Scenes/Scene_GameCommon.h"
#include "../Game/GameBoard.h"
#include "../Game/GameGlobal.h"
#include "../Game/GameHost.h"
#include "../Game/MsgResult.h"

#include <thread>
#include <functional>

#include <MysteryEngine/G3D/Camera.Perspective.h>
#include <MysteryEngine/G3D/G3dGlobal.h>

#include "../Models/IGround.h"
#include "../Models/IActorGroup.h"

//#include "../Models/Actor.h"
//#include "char_151_myrtle.h"

#include "ISummonMngr.h"
#include <SFML/Window/Event.hpp>

namespace Game::Creator {

int setup() {
	Game::GameHolder::setup();
	Scene::GameCommon::setup();
	Game::GameBoard::setup();
	Game::GameGlobal::board->JoinEntity(std::make_shared<GameInitalizator>());
	return 0;
}

void drop() {
	Game::GameBoard::drop();
	Scene::GameCommon::drop();
	Game::GameHolder::drop();

	Game::IActorGroup::Drop(); // 清除实例
}

void GameInitalizator::OnJoined() {
	cc = 0;

	GameGlobal::board->SubscribeMsg(5678, m_location);

	auto actorGroup = Game::IActorGroup::Instance();

	auto camera = std::make_shared<ME::PerspectiveCamera>();
	Game::GameGlobal::show->SetCamera(camera);
	camera->setAspectRatio(16.0f / 9.0f);
	camera->setFOV(40.0f);
	camera->setPosition(0.0f, -5.5f, 8.66025f);
	camera->setRotation(30.0f, 0.0f, 0.0f);


	auto summonmngr = ISummonMngr::Create();
	GameGlobal::board->JoinEntity(summonmngr);

	summonmngr->AddBegin();
	summonmngr->AddEntity(151, "char_151_myrtle", true);
	summonmngr->AddEnd();


	//auto spineFactory = ohms::ISpineFactory::Create();

	//spineFactory->CreatePose(m_testpose, "char_128_plosis_epoque#3", 0);
	//spineFactory->CreatePose(m_testpose1, "char_128_plosis_epoque#3", 1);
	//spineFactory->CreatePose(m_testpose, "char_151_myrtle", 0);
	//spineFactory->CreatePose(m_testpose1, "char_151_myrtle", 1);

	//ME::G3dGlobal::setActive(true);
	//auto animation = m_testpose->CreateAnimation();
	//auto animation1 = m_testpose1->CreateAnimation();
	//ME::G3dGlobal::setActive(false);


	//animation->SetOutline(true);
	//animation1->SetOutline(true);

	//actorGroup->AddActor(animation);

	//auto actor = std::make_shared<ActorSpine2>(animation, animation1);

	//actorGroup->AddActor(actor);

	//auto unit = std::make_shared<Units::Char_151_Myrtle>();
	//unit->m_actor = actor;

	//GameGlobal::board->JoinEntity(unit);

	////////////////////

	ME::G3dGlobal::setActive(true);
	auto ground = IObjModel::Create();
	ground->LoadModelData("res/main_7-3/main.obj");
	//gd->setRotation(0.0f, 0.0f, 180.0f);
	ground->setScale(-1.0f, 1.0f, -1.0f);
	ME::G3dGlobal::setActive(false);

	////////////////////

	Game::GameGlobal::show->AddModel(ground);
	Game::GameGlobal::show->AddModel(actorGroup);

	pos = 5;
	Game::GameGlobal::board->SubscribeMsg(1, m_location);
}

void GameInitalizator::OnKicking() {
	Game::GameGlobal::board->UnsubscribeMsg(1, m_location);
	GameGlobal::board->BroadcastMsg(1234, 0, 0);
}

void GameInitalizator::FixedUpdate(float dt) {

	//printf_s("%zd ", cc);

	//GameGlobal::board->DistributeMsg(5678, cc, 0);

	//if (cc >= 430) {
	//	GameGlobal::board->BroadcastMsg(9876, 0, 0);
	//}

	//cc--;
	//if (cc == 0)
	//	KickSelf();

	//gd->rotate(0.0f, 0.5f, 0.0f);
}

MsgResultType GameInitalizator::ReceiveMessage(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam) {
	switch (msg) {
	case 1:
	{
		auto e = (sf::Event*)lparam;
		switch (e->type) {
		case sf::Event::KeyPressed:
			switch (e->key.code) {
			case sf::Keyboard::Space:
				GameGlobal::board->DistributeMsg(2, 0, pos);
				pos--;
				break;
			}
			break;
		}
		break;
	}
	case 5678:
		cc = wparam + 1;
		break;
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

void GameInitalizator::LoadThread() {}

}
