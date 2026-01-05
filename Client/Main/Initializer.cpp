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
#include "Initializer.h"

#include "../Game/Global.h"
#include "../Game/Board.h"

#include <MysteryEngine/G3D/GameStage.h>
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

#include "HitTestData.h"
#include "QueryDeployableData.h"

namespace Main {

void Initializer::onJoined() {
	Game::Global::board->subscribeMsg(5678, m_myself);

	auto& camera = ME::GameStage::instance().getCamera();
	/*camera.setType(ME::Camera::Type::Perspective);
	camera.setAspectRatio(16.0f / 9.0f);
	camera.setFOV(45.0f);
	camera.setPosition(0.0f + 5.5f, -5.5f * 0.8f + 3.5f, 8.66025f * 0.8f);
	camera.setRotation(30.0f, 0.0f, 0.0f);*/
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
	camera.setType(ME::Camera::Type::TEST);
	camera.setAspectRatio(16.0f / 9.0f);
	camera.setFOV(52.0f);
	camera.setPosition(0.0f + 5.5f, -5.5f * 0.55f + 3.5f, 8.66025f * 0.65f);
	camera.setRotation(30.0f, 0.0f, 0.0f);

	////////////////////
	ME::G3dGlobal::SetActive(true);

	auto ground = Model::IObjModel::Create();
	ground->setup();

	ground->loadModelData("res/main_7-3/main.obj");
	//ground->setRotation(0.0f, 180.0f, 0.0f);
	ground->setScale(-1.0f, 1.0f, -1.0f);
	ground->setPosition(5.5f, 3.5f, 0.0f);

	ME::G3dGlobal::SetActive(false);

	ME::GameStage::instance().addGround(ground);

	ground->setGroundSize(11.0f, 7.0f);
	ME::GameStage::instance().setGroundSize(11.0f, 7.0f);
	////////////////////

	auto summonmngr = ISummonMngr::Create();
	Game::Global::board->joinEntity(summonmngr);

	summonmngr->beginAdd();
	summonmngr->addEntity(151);
	summonmngr->addEntity(101);
	summonmngr->addEntity(128);
	summonmngr->addEntity(1002, true);
	summonmngr->endAdd();

	///////////////////

	pos[0] = 5.5f;
	pos[1] = 3.5f;
	Game::Global::board->subscribeMsg(Game::MsgId::GuiEvent, m_myself);

	auto maphost = std::make_shared<MapHost>();
	std::ifstream ifs;
	ifs.open("res/main_7-3/map.txt");
	maphost->load(ifs);

	maphost->receiveMessage(HostMsgId::MapInitCheckpointCnt, 6, 0);
	int testp[2];
	testp[0] = 10;
	testp[1] = 3;
	maphost->receiveMessage(HostMsgId::MapInitCheckpointSet, 0, (intptr_t)testp);
	testp[0] = 10;
	testp[1] = 1;
	maphost->receiveMessage(HostMsgId::MapInitCheckpointSet, 1, (intptr_t)testp);
	testp[0] = 1;
	testp[1] = 1;
	maphost->receiveMessage(HostMsgId::MapInitCheckpointSet, 2, (intptr_t)testp);
	testp[0] = 1;
	testp[1] = 5;
	maphost->receiveMessage(HostMsgId::MapInitCheckpointSet, 3, (intptr_t)testp);
	testp[0] = 10;
	testp[1] = 5;
	maphost->receiveMessage(HostMsgId::MapInitCheckpointSet, 4, (intptr_t)testp);
	testp[0] = 0;
	testp[1] = 3;
	maphost->receiveMessage(HostMsgId::MapInitCheckpointSet, 5, (intptr_t)testp);

	maphost->receiveMessage(HostMsgId::MapInitOk, 0, 0);

	Game::Global::board->setHost(
		Game::HostJob::MapManager,
		maphost
	);

	flag = 0;
}

void Initializer::onKicking() {
	Game::Global::board->unsubscribeMsg(Game::MsgId::GuiEvent, m_myself);

	Game::Global::board->broadcast(1234, 0, 0);
}

bool Initializer::fixedUpdate() {
	return true;
}

Game::MsgResultType Initializer::receiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	switch (msg) {
	case Game::MsgId::GuiEvent:
	{
		auto e = (sf::Event*)lparam;
		if (auto key = e->getIf<sf::Event::KeyPressed>()) {
			switch (key->code) {
			case sf::Keyboard::Key::Num1:
				flag = 0;
				//Game::Global::board->postMsg(Game::MsgId::Summon, 0, (intptr_t)this->pos);
				break;
			case sf::Keyboard::Key::Num2:
				flag = 1;
				//Game::Global::board->postMsg(Game::MsgId::Summon, 1, (intptr_t)this->pos);
				break;
			case sf::Keyboard::Key::Num3:
				flag = 2;
				//Game::Global::board->postMsg(Game::MsgId::Summon, 2, (intptr_t)this->pos);
				break;
			case sf::Keyboard::Key::Num4:
				flag = 3;
				//Game::Global::board->postMsg(Game::MsgId::Summon, 3, (intptr_t)this->pos);
				break;
			case sf::Keyboard::Key::Num0:
				flag = -1;
				//Game::Global::board->tellMsg(2, 3, Main::MsgId::UserRetreat, 0, 0);
				break;
			case sf::Keyboard::Key::Left:
				pos[0] -= 1.0f;
				break;
			case sf::Keyboard::Key::Right:
				pos[0] += 1.0f;
				break;
			case sf::Keyboard::Key::Down:
				pos[1] -= 1.0f;
				break;
			case sf::Keyboard::Key::Up:
				pos[1] += 1.0f;
				break;
			case sf::Keyboard::Key::W:
				ME::GameStage::instance().getCamera().translate(0.0f, 0.1f, 0.0f);
				break;
			case sf::Keyboard::Key::S:
				ME::GameStage::instance().getCamera().translate(0.0f, -0.1f, 0.0f);
				break;
			case sf::Keyboard::Key::D:
				ME::GameStage::instance().getCamera().translate(0.1f, 0.0f, 0.0f);
				break;
			case sf::Keyboard::Key::A:
				ME::GameStage::instance().getCamera().translate(-0.1f, 0.0f, 0.0f);
				break;
			}
		}
		else if (auto mouse = e->getIf<sf::Event::MouseButtonPressed>()) {
			if (mouse->button == sf::Mouse::Button::Left) {
				//glm::vec3 pos;
				//Game::Global::stage->testPoint({ e->mouseButton.x, e->mouseButton.y }, &pos);
				//Game::Global::board->postMsg(Game::MsgId::Summon, 3, (intptr_t) & (pos.x));
				//glm::vec3 d;
				//Game::Global::stage->testDirection({ e->mouseButton.x, e->mouseButton.y }, &d, &pos);

				auto map = Game::Global::board->getHost(Game::HostJob::MapManager);
				glm::vec3 pos;
				glm::vec3 d;
				Game::MsgResultType res;

				ME::GameStage::instance().testDirection(mouse->position, &d, &pos);

				if (flag == 3) {
					d *= pos.z / d.z;
					pos -= d;
					Game::Global::board->postMsg(Game::MsgId::Summon, 3, (intptr_t) & (pos.x));
					break;
				}

				HitTestData hitTestData;
				hitTestData.direction[0] = d[0];
				hitTestData.direction[1] = d[1];
				hitTestData.direction[2] = d[2];
				hitTestData.startPoint[0] = pos[0];
				hitTestData.startPoint[1] = pos[1];
				hitTestData.startPoint[2] = pos[2];
				map->receiveMessage(HostMsgId::HitTest, 0, (Game::MsgLparamType)&hitTestData);

				if (flag == -1) {
					Game::Global::board->
						getHost(Game::HostJob::MapManager)->
						receiveMessage(
							Main::HostMsgId::RetreatOccupation,
							hitTestData.place.subId,
							0
						);
					break;
				}

				if (hitTestData.place.status == OccupiedPlace::HitGround) {
					QueryDeployableData queryData;

					switch (flag) {
					case 0:
						queryData.type = QueryDeployableData::CommonTowerOnGround;
						break;
					case 1:
						queryData.type = QueryDeployableData::CommonTowerOnWall;
						break;
					case 2:
						queryData.type = QueryDeployableData::CommonTowerOnWall;
						break;
					}

					res = map->receiveMessage(
						HostMsgId::QueryDeployable,
						hitTestData.place.subId,
						(Game::MsgLparamType)&queryData
					);
					if (res == Game::MsgResult::OK) {
						Game::Global::board->postMsg(Game::MsgId::Deploy, flag, (Game::MsgLparamType)&hitTestData.place);
					}
				}
			}
		}
		break;
	}
	case 9876:
		Game::Global::board->unsubscribeMsg(5678, m_myself);
		kickSelf();
		break;
	case 1234:
		Game::Global::board->ExitGame(4321);
		break;
	}
	return Game::MsgResult::OK;
}

void Initializer::loadThread() {}

}
