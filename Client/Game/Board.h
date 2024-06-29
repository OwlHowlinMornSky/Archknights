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
#pragma once

#include <memory>
#include <map>
#include <set>
#include <vector>
#include <stack>
#include <functional>

#include "Entity.h"
#include "Host.h"
#include "../Physics/IWorld.h"

namespace Game {

/**
 * @brief 游戏板：定义了游戏场地基本功能。
*/
class Board {
public:
	Board();
	virtual ~Board();

	static int setup();
	static void drop();

	void SetExitCallback(std::function<void(int)> cb);
	void Clear();

// 基础功能
public:
	/**
	 * @brief 是否没有实体。
	 * @return true则没有实体。
	*/
	bool IsEmpty();

	/**
	 * @brief 将指定实体入场。这会让实体收到OnJoined。
	 * @param entity 指定实体。
	*/
	void JoinEntity(std::shared_ptr<Entity> entity);
	/**
	 * @brief 让指定实体退场。这会让实体收到OnKicking。
	 * @param location 指定实体的位置。
	*/
	void KickEntity(size_t location);

	void ExitGame(int code);

	/**
	 * @brief 获取指定实体。
	 * @param location 指定实体的位置。
	 * @return 相应位置的实体。（可能为nullptr）
	*/
	std::shared_ptr<Entity> EntityAt(size_t location);

protected:
	EntityIdType m_entityIdCnt; // 实体id计数，用于让每个实体都有独一无二的标识和判断方法。
	std::vector<std::shared_ptr<Entity>> m_entities; // 在场实体所处空间。在场即指针不为空，应当保证退场同时置为nullptr。
	std::stack<size_t> m_emptyLocations; // 空闲位置，是实体所在deque的偏移。由实体退场产生。
	std::function<void(int)> m_exitCallback;

public:
	std::unique_ptr<Physics::IWorld> m_world;

public:
	/**
	 * @brief 更新游戏板。
	 * @param dt 时间增量。
	*/
	void Update(long long dt);

	void RegistryForExit(EntityLocationType location);

protected:
	std::stack<EntityLocationType> m_readyForExit;

// 事件控制器
public:
	/**
	 * @brief 向指定位置发送消息。
	 * @param location 指定位置。
	 * @param msg 消息id。
	 * @param wparam 消息w参数。
	 * @param lparam 消息l参数。
	 * @return 消息回执。
	*/
	MsgResultType SendMsg(EntityLocationType location, MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam);
	/**
	 * @brief 向指定实体发送消息。
	 * @param location 指定的位置。
	 * @param id 指定的实体。
	 * @param msg 消息id。
	 * @param wparam 消息w参数。
	 * @param lparam 消息l参数。
	 * @return 消息回执。
	*/
	MsgResultType TellMsg(EntityLocationType location, EntityIdType id, MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam);
	/**
	 * @brief 分发消息。即 向所有订阅指定消息的实体发送消息。
	 * @param msg 消息id。即 指定消息。
	 * @param wparam 消息w参数。
	 * @param lparam 消息l参数。
	*/
	void PostMsg(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam);
	/**
	 * @brief 广播消息。即向在场所有实体发送消息。
	 * @param msg 消息id。
	 * @param wparam 消息w参数。
	 * @param lparam 消息l参数。
	*/
	void Broadcast(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam);
	/**
	 * @brief 订阅消息。
	 * @param msg 订阅的消息id。
	 * @param location 本实体的位置。
	*/
	void SubscribeMsg(MsgIdType msg, EntityLocationType location);
	/**
	 * @brief 退订消息。
	 * @param msg 退订的消息id。
	 * @param location 本实体的位置。
	*/
	void UnsubscribeMsg(MsgIdType msg, EntityLocationType location);

protected:
	std::map<MsgIdType, std::set<EntityLocationType>> m_msgMap; // 消息订阅表。
	unsigned long long m_time;

public:
	std::shared_ptr<Host> GetHost(int job);
	void SetHost(int job, std::shared_ptr<Host> host);

protected:
	std::vector<std::shared_ptr<Host>> m_hosts;
};

}
