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
}

void GameInitalizator::OnJoined() {
	LoadStart();
}

void GameInitalizator::OnKicking() {
	LoadOver();
}

void GameInitalizator::FixedUpdate(float dt) {
	LoadUpdate(dt);
}

MsgResultType GameInitalizator::ReceiveMessage(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam) {
	switch (msg) {
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

void GameInitalizator::LoadStart() {
	//cc = 120;

	cc = 0;

	GameGlobal::board->SubscribeMsg(5678, m_location);


	auto actorGroup = Game::IActorGroup::Create();

	auto camera = std::make_shared<ME::PerspectiveCamera>();
	Game::GameGlobal::show->SetCamera(camera);
	camera->setAspectRatio(16.0f / 9.0f);
	camera->setFOV(40.0f);
	camera->setPosition(0.0f, -5.5f, 8.66025f);
	//cam->setPosition(0.0f, 0.0f, 5.0f);
	camera->setRotation(30.0f, 0.0f, 0.0f);
	//cam->setRotation(0.0f, 0.0f, 0.0f);

	//cam->setZNear(0.5f);
	//cam->setZFar(50.0f);

	auto spineFactory = ohms::ISpineFactory::Create();

	spineFactory->CreatePose(m_testpose, "char_151_myrtle", 0);

	ME::G3dGlobal::setActive(true);
	auto animation = m_testpose->CreateAnimation();
	ME::G3dGlobal::setActive(false);

	animation->setPosition(0.0f, 0.0f, 0.0f);
	animation->setRotation(0.0f, 0.0f, 0.0f);
	animation->setOrigin(0.0f, 0.0f, 0.0f);
	animation->setScale(1.0f, 1.0f, 1.0f);

	//anim->setAnimation(0, "Default", false);
	//anim->setAnimation(1, "FlagFlutter", true)->setDelay(anim->findAnimation("Start")->getDuration());
	//anim->setAnimation(2, "Blink", true);
	//anim->setAnimation(3, "Start", false);
	//anim->addAnimation(3, "Idle", true, 0.0f);

	animation->SetOutline(true);

	actorGroup->AddActor(animation);

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

}

void GameInitalizator::LoadThread() {}

void GameInitalizator::LoadUpdate(float dt) {
	//printf_s("%zd ", cc);

	GameGlobal::board->DistributeMsg(5678, cc, 0);

	//if (cc >= 430) {
	//	GameGlobal::board->BroadcastMsg(9876, 0, 0);
	//}

	//cc--;
	//if (cc == 0)
	//	KickSelf();

	//gd->rotate(0.0f, 0.5f, 0.0f);
}

void GameInitalizator::LoadOver() {
	GameGlobal::board->BroadcastMsg(1234, 0, 0);
}

}
