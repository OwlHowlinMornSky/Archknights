/*
*    Mystery Engine
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
#include <MysteryEngine/Game/IGameBoard.h>

namespace ME {

bool IGameBoard::IsEmpty() {
	// 空闲位置数量等于实体位置数量说明没有实体。
	return m_entities.size() == m_emptyLocations.size();
}

void IGameBoard::SetPaused(bool pause) {

}

void IGameBoard::JoinEntity(std::shared_ptr<Entity> entity) {
	assert(m_entityIdCnt < UINT32_MAX); // 拒绝id溢出。（虽然正常情况下不会产生这么多实体）
	if (m_emptyLocations.empty()) { // 没有空闲位置
		m_entities.push_back(entity); // 直接放在末尾。
		entity->BasicOnJoined(++m_entityIdCnt, m_entities.size() - 1); // 通知相应标识。
	}
	else { // 有空闲位置。
		size_t location = m_emptyLocations.top(); // 优先取用空闲位置。
		m_emptyLocations.pop();
		m_entities[location] = entity; // 放入空闲位置。
		entity->BasicOnJoined(++m_entityIdCnt, location); // 通知相应标识。
	}
}

void IGameBoard::KickEntity(size_t location) {
	assert(location < m_entities.size());
	assert(m_entities[location]); // 在场。
	m_entities[location]->BasicOnKicking(); // 通知。
	m_entities[location].reset(); // 置空！这是保证“在场”“离场”概念的基础。
	m_emptyLocations.push(location); // 记录空位。
}

std::shared_ptr<Entity> IGameBoard::EntityAt(size_t location) {
	assert(location < m_entities.size());
	return m_entities[location];
}

void IGameBoard::Update(float dt) {
	// 目前注释掉的是自动注销逻辑，算是一种保险。
	// 只要注册过的实体保证退场时注销，就不会需要这个。

	//std::set<EntityLocationType> failedLoc;
	for (EntityLocationType loc : m_trigger_update) {
		if (m_entities[loc] == nullptr) {
			//failedLoc.insert(loc);
			continue;
		}
		m_entities[loc]->OnUpdate(dt);
	}
	//if (failedLoc.size()) {
	//	for (auto loc : failedLoc) {
	//		m_trigger_update.erase(loc);
	//	}
	//}

}

void IGameBoard::Register_Update(EntityLocationType location) {
	m_trigger_update.insert(location);
}

void IGameBoard::Unregister_Update(EntityLocationType location) {
	m_trigger_update.erase(location);
}

MsgResultType IGameBoard::SendMsg(EntityLocationType location, MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam) {
	assert(location < m_entities.size());
	assert(m_entities[location]); // 在场。
	return m_entities[location]->ReceiveMessage(msg, wparam, lparam);
}

void IGameBoard::DistributeMsg(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam) {
	// 目前注释掉的是自动退订逻辑，算是一种保险。
	// 只要注册过的实体保证退场时注销，就不会需要这个。

	auto mapIt = m_msgMap.find(msg);
	if (mapIt == m_msgMap.end())
		return;
	auto& reg = mapIt->second;
	//std::set<EntityLocationType> failedLoc;
	for (EntityLocationType loc : reg) {
		if (m_entities[loc] == nullptr) { // 跳过离场位置。
		//	failedLoc.insert(loc);
			continue;
		}
		m_entities[loc]->ReceiveMessage(msg, wparam, lparam);
	}
	//if (failedLoc.size()) {
	//	for (auto loc : failedLoc) {
	//		reg.erase(loc);
	//	}
	//	if (reg.size() == 0) {
	//		m_msgMap.erase(mapIt);
	//	}
	//}
}

void IGameBoard::BroadcastMsg(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam) {
	for (auto& entity : m_entities) {
		if (entity == nullptr) // 跳过离场位置。
			continue;
		entity->ReceiveMessage(msg, wparam, lparam);
	}
}

void IGameBoard::SubscribeMsg(MsgIdType msg, EntityLocationType location) {
	m_msgMap[msg].insert(location);
}

void IGameBoard::UnsubscribeMsg(MsgIdType msg, EntityLocationType location) {
	auto mapIt = m_msgMap.find(msg);
	if (mapIt == m_msgMap.end())
		return;
	auto& reg = mapIt->second;
	reg.erase(location);
	if (reg.size() == 0) { // 在没有订阅者时清除该消息的set。也许意义不大，还徒增内存碎片。
		m_msgMap.erase(mapIt);
	}
}

} // namespace game
