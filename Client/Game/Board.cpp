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
#include "Board.h"

#include "MsgResult.h"
#include "Global.h"

#include <cassert>

namespace Game {

Board::Board() :
	m_time(0),
	m_entityIdCnt(0) {
	m_world = Physics::IWorld::CreateWorld();
	m_hosts.resize((size_t)HostJob::COUNT);
}

Board::~Board() {}

int Board::Setup() {
	if (Game::Global::board)
		return 1;
	Game::Global::board = std::make_unique<Board>();
	return 0;
}

void Board::Drop() {
	if (Game::Global::board != nullptr)
		Game::Global::board->clear();
	Game::Global::board.reset();
}

void Board::SetExitCallback(std::function<void(int)> cb) {
	m_exitCallback = cb;
}

void Board::clear() {
	for (size_t n = m_entities.size(), i = n - 1; i != 0; --i) {
		if (m_entities[i] == nullptr)
			continue;
		m_entities[i]->basicOnKicking();
		m_entities[i].reset();
	}
	if (m_entities[0] != nullptr) {
		m_entities[0]->basicOnKicking();
		m_entities[0].reset();
	}
	m_entities.clear();
	while (!m_readyForExit.empty())
		m_readyForExit.pop();
	while (!m_emptyLocations.empty())
		m_emptyLocations.pop();
	m_entityIdCnt = 0;

	m_hosts.clear();

	m_world.reset();
	m_msgMap.clear();
}

bool Board::isEmpty() {
	// 空闲位置数量等于实体位置数量说明没有实体。
	return m_entities.size() == m_emptyLocations.size();
}

void Board::joinEntity(std::shared_ptr<Entity> entity) {
	assert(m_entityIdCnt < UINT32_MAX); // 拒绝id溢出。（虽然正常情况下不会产生这么多实体）
	if (m_emptyLocations.empty()) { // 没有空闲位置
		m_entities.push_back(entity); // 直接放在末尾。
		entity->basicOnJoined(++m_entityIdCnt, m_entities.size() - 1); // 通知相应标识。
	}
	else { // 有空闲位置。
		size_t location = m_emptyLocations.top(); // 优先取用空闲位置。
		m_emptyLocations.pop();
		m_entities[location] = entity; // 放入空闲位置。
		entity->basicOnJoined(++m_entityIdCnt, location); // 通知相应标识。
	}
}

void Board::kickEntity(size_t location) {
	assert(location < m_entities.size());
	assert(m_entities[location]); // 在场。
	m_entities[location]->basicOnKicking(); // 通知。
	m_entities[location].reset(); // 置空！这是保证“在场”“离场”概念的基础。
	if (location)
		m_emptyLocations.push(location); // 记录空位。(0不复用，只作为最初initalizator占用位置，之后永远为空)。
}

void Board::ExitGame(int code) {
	m_exitCallback(code);
}

std::shared_ptr<Entity> Board::getEntityAt(size_t location) {
	assert(location < m_entities.size());
	return m_entities[location];
}

void Board::update(long long dt) {
	if (dt < 33333) {
		m_time += dt;
		if (m_time < 33333)
			return;
		m_time -= 33333;
	}
	m_world->update(1.0f / 30.0f);
	for (std::shared_ptr<Entity> entity : m_entities) {
		if (entity == nullptr)
			continue;
		entity->fixedUpdate();
	}
	while (!m_readyForExit.empty()) {
		kickEntity(m_readyForExit.top());
		m_readyForExit.pop();
	}
}

void Board::registryForExit(EntityLocationType location) {
	m_readyForExit.push(location);
}

MsgResultType Board::sendMsg(EntityLocationType location, MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam) {
	assert(location < m_entities.size());
	//assert(m_entities[location]); // 在场。
	if (m_entities[location] == nullptr) {
		return MsgResult::EmptyPlace;
	}
	return m_entities[location]->EntityProc(msg, wparam, lparam);
}

MsgResultType Board::tellMsg(EntityLocationType location, EntityIdType id, MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam) {
	assert(location < m_entities.size());
	//assert(m_entities[location]); // 在场。
	if (m_entities[location] == nullptr) {
		return MsgResult::EmptyPlace;
	}
	if (m_entities[location]->m_id != id) {
		return MsgResult::IncorrectId;
	}
	return m_entities[location]->EntityProc(msg, wparam, lparam);
}

void Board::postMsg(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam) {
	// failedLoc相关的是自动退订逻辑，算是一种保险。
	// 如果注册过的实体保证退场时注销，就不需要这个。
	auto mapIt = m_msgMap.find(msg);
	if (mapIt == m_msgMap.end())
		return;
	auto& reg = mapIt->second;
	std::set<EntityLocationType> failedLoc;
	for (EntityLocationType loc : reg) {
		if (m_entities[loc] == nullptr) { // 跳过离场位置。
			failedLoc.insert(loc);
			continue;
		}
		MsgResultType res = m_entities[loc]->EntityProc(msg, wparam, lparam);
		if (res == MsgResult::Unsubscribe) { // 相当于消息失败。
			failedLoc.insert(loc);
		}
	}
	if (failedLoc.size()) {
		for (auto loc : failedLoc) {
			reg.erase(loc);
		}
		if (reg.size() == 0) {
			m_msgMap.erase(mapIt);
		}
	}
}

void Board::broadcast(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam) {
	for (auto& entity : m_entities) {
		if (entity == nullptr) // 跳过离场位置。
			continue;
		entity->receiveMessage(msg, wparam, lparam);
	}
}

void Board::subscribeMsg(MsgIdType msg, EntityLocationType location) {
	m_msgMap[msg].insert(location);
}

void Board::unsubscribeMsg(MsgIdType msg, EntityLocationType location) {
	auto mapIt = m_msgMap.find(msg);
	if (mapIt == m_msgMap.end())
		return;
	auto& reg = mapIt->second;
	reg.erase(location);
	if (reg.size() == 0) { // 在没有订阅者时清除该消息的set。也许意义不大，还徒增内存碎片。
		m_msgMap.erase(mapIt);
	}
}

std::shared_ptr<Host> Board::getHost(int job) {
	if (job < 0 || job >= HostJob::COUNT)
		return std::shared_ptr<Host>();
	return m_hosts[job];
}

void Board::setHost(int job, std::shared_ptr<Host> host) {
	if (job < 0 || job >= HostJob::COUNT)
		return;
	m_hosts[job] = host;
	return;
}

} // namespace Game
