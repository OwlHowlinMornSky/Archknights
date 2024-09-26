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
	for (auto i = m_entities.rbegin(), n = m_entities.rend(); i != n; ++i)
		(**i).basicOnKicking();
	for (auto i = m_entities.rbegin(), n = m_entities.rend(); i != n; ++i)
		(*i).reset();
	m_entities.clear();
	m_entityIdCnt = 0;

	m_hosts.clear();

	m_world.reset();
	m_msgMap.clear();
}

bool Board::isEmpty() {
	return m_entities.empty();
}

void Board::joinEntity(std::shared_ptr<Entity> entity) {
	assert(m_entityIdCnt < UINT32_MAX); // 拒绝id溢出。（虽然正常情况下不会产生这么多实体）
	m_entities.push_back(entity); // 直接放在末尾。
	entity->basicOnJoined(++m_entityIdCnt, (std::weak_ptr<Entity>)entity); // 通知相应标识。
}

void Board::ExitGame(int code) {
	m_exitCallback(code);
}

void Board::update(long long dt) {
	if (dt < 33333) {
		m_time += dt;
		if (m_time < 33333)
			return;
		m_time -= 33333;
	}
	m_world->update();

	std::vector<std::shared_ptr<Entity>>::iterator i = m_entities.begin();
	std::vector<std::shared_ptr<Entity>>::iterator j = i;
	std::vector<std::shared_ptr<Entity>>::iterator n = m_entities.end();
	while (i < n) {
		bool isNormal = (**i).fixedUpdate();
		[[likely]] if (isNormal) {
			(*j).swap(*i);
			++j;
			++i;
		}
		else {
			(**i).basicOnKicking();
			++i;
		}
	}
	m_entities.erase(j, n);

	return;
}

void Board::postMsg(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam) {
	auto mapIt = m_msgMap.find(msg);
	if (mapIt == m_msgMap.end())
		return;
	auto& reg = mapIt->second;

	std::vector<std::weak_ptr<Entity>>::iterator i = reg.begin();
	std::vector<std::weak_ptr<Entity>>::iterator j = i;
	std::vector<std::weak_ptr<Entity>>::iterator n = reg.end();
	while (i < n) {
		std::shared_ptr<Entity> entity = (*i).lock();
		bool ok = true;
		if (entity == nullptr) { // 跳过离场单位。
			ok = false;
		}
		else {
			MsgResultType res = entity->EntityProc(msg, wparam, lparam);
			if (res == MsgResult::Unsubscribe) { // 相当于消息失败。
				ok = false;
			}
		}
		[[likely]] if (ok) {
			(*j).swap(*i);
			++j;
			++i;
		}
		else {
			++i;
		}
	}
	reg.erase(j, n);
}

void Board::broadcast(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam) {
	for (std::shared_ptr<Entity>& entity : m_entities) {
		entity->receiveMessage(msg, wparam, lparam);
	}
}

void Board::subscribeMsg(MsgIdType msg, std::weak_ptr<Entity> ref) {
	m_msgMap[msg].push_back(ref);
}

void Board::unsubscribeMsg(MsgIdType msg, std::weak_ptr<Entity> ref) {
	auto mapIt = m_msgMap.find(msg);
	if (mapIt == m_msgMap.end())
		return;
	auto& reg = mapIt->second;
	auto t = ref.lock().get();

	for (auto it = reg.begin(), n = reg.end(); it != n; ++it) {
		if (it->lock().get() == t) {
			reg.erase(it);
			break;
		}
	}

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
