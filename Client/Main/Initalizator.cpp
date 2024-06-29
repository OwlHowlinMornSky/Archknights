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

#include "../Game/Global.h"
#include "../Game/Board.h"

#include "../Game/Stage.h"
#include <MysteryEngine/G3D/Camera.h>
#include <MysteryEngine/G3D/G3dGlobal.h>
#include "../Models/IGround.h"

#include "../Game/Host.h"
#include "MapHost.h"
#include "HostMsgId.h"

#include "ISummonMngr.h"
#include "../Game/MsgResult.h"
#include <SFML/Window/Event.hpp>
#include "MsgId.h"

namespace Main {

void Initalizator::OnJoined() {
	Game::Global::board->SubscribeMsg(5678, m_location);

	auto& camera = Game::Global::show->getCamera();
	camera.setType(ME::Camera::Type::Perspective);
	camera.setAspectRatio(16.0f / 9.0f);
	camera.setFOV(40.0f);
	camera.setPosition(0.0f + 5.5f, -5.5f + 3.5f, 8.66025f);
	camera.setRotation(30.0f, 0.0f, 0.0f);
	/*camera.setType(ME::Camera::Type::Oblique);
	camera.setZNear(-15.0f);
	camera.setZFar(15.0f);
	camera.setDim(16.0f, 9.0f);
	camera.setSheer(0.5f, 0.5f);
	camera.setPosition(5.5f, -0.5f, 0.1f);
	camera.setRotation(90.0f, 0.0f, 0.0f);*/
	/*camera.setType(ME::Camera::Type::Orthographic);
	camera.setDim(16.0f, 9.0f);
	camera.setPosition(5.5f, -5.5f + 3.5f, 8.66025f);
	camera.setRotation(30.0f, 0.0f, 0.0f);*/

	////////////////////
	ME::G3dGlobal::setActive(true);

	auto ground = Model::IObjModel::Create();
	ground->Setup();

	ground->LoadModelData("res/main_7-3/main.obj");
	//ground->setRotation(0.0f, 180.0f, 0.0f);
	ground->setScale(-1.0f, 1.0f, -1.0f);
	ground->setPosition(5.5f, 3.5f, 0.0f);

	ME::G3dGlobal::setActive(false);

	Game::Global::show->AddGround(ground);

	ground->SetSize(11.0f, 7.0f);
	Game::Global::show->SetGroundSize(11.0f, 7.0f);
	////////////////////

	auto summonmngr = ISummonMngr::Create();
	Game::Global::board->JoinEntity(summonmngr);

	summonmngr->AddBegin();
	summonmngr->AddEntity(151);
	summonmngr->AddEntity(101);
	summonmngr->AddEntity(128);
	summonmngr->AddEntity(1002, true);
	summonmngr->AddEnd();

	///////////////////

	pos[0] = 5.5f;
	pos[1] = 3.5f;
	Game::Global::board->SubscribeMsg(Game::MsgId::GuiEvent, m_location);

	auto maphost = std::make_shared<MapHost>();
	std::ifstream ifs;
	ifs.open("res/main_7-3/map.txt");
	maphost->Load(ifs);

	maphost->ReceiveMessage(HostMsgId::MapInitCheckpointCnt, 2, 0);
	int testp[2] = { 9, 1 };
	maphost->ReceiveMessage(HostMsgId::MapInitCheckpointSet, 0, (intptr_t) & (testp[0]));
	int testp1[2] = { 0, 3 };
	maphost->ReceiveMessage(HostMsgId::MapInitCheckpointSet, 1, (intptr_t) & (testp1[0]));

	maphost->ReceiveMessage(HostMsgId::MapInitOk, 0, 0);

	Game::Global::board->SetHost(
		Game::HostJob::MapPathManager,
		maphost
	);
}

void Initalizator::OnKicking() {
	Game::Global::board->UnsubscribeMsg(Game::MsgId::GuiEvent, m_location);

	Game::Global::board->Broadcast(1234, 0, 0);
}

void Initalizator::FixedUpdate() {}

Game::MsgResultType Initalizator::ReceiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	switch (msg) {
	case Game::MsgId::GuiEvent:
	{
		auto e = (sf::Event*)lparam;
		switch (e->type) {
		case sf::Event::KeyPressed:
			switch (e->key.code) {
			case sf::Keyboard::Num1:
				Game::Global::board->PostMsg(2, 0, (intptr_t)this->pos);
				break;
			case sf::Keyboard::Num2:
				Game::Global::board->PostMsg(2, 1, (intptr_t)this->pos);
				break;
			case sf::Keyboard::Num3:
				Game::Global::board->PostMsg(2, 2, (intptr_t)this->pos);
				break;
			case sf::Keyboard::Num4:
				//GameGlobal::board->PostMsg(2, 3, (intptr_t)this->pos);
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
			case sf::Keyboard::S:
				Game::Global::show->getCamera().translate(0.0f, -0.1f, 0.0f);
				break;
			}
			break;
		case sf::Event::MouseMoved:
			break;
		case sf::Event::MouseButtonPressed:
			if (e->mouseButton.button == sf::Mouse::Left) {
				glm::vec3 pos;
				Game::Global::show->testPoint({ e->mouseButton.x, e->mouseButton.y }, &pos);
				Game::Global::board->PostMsg(2, 3, (intptr_t)&(pos.x));
			}
			break;
		}
		break;
	}
	case 9876:
		Game::Global::board->UnsubscribeMsg(5678, m_location);
		KickSelf();
		break;
	case 1234:
		Game::Global::board->ExitGame(4321);
		break;
	}
	return Game::MsgResult::OK;
}

void Initalizator::LoadThread() {}

}
