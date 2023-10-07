/*
*    Archknights
*
*    Copyright (C) 2023  Tyler Parret True
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

#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>
#include "ICarnival.h"

namespace GUI {

/**
 * @brief 接口 Activity: 于 Carnival 中执行的单个界面。
*/
class IActivity {
public:
	IActivity() noexcept = default;
	virtual ~IActivity() noexcept = default;

public:
	/**
	 * @brief Activity 被创建后、运行前的处理。
	 * @param carnival: 对所属 Carnival 的引用。
	*/
	virtual void start(GUI::ICarnival& carnival) = 0;
	/**
	 * @brief Activity 结束后、移除前的处理。
	*/
	virtual void stop() noexcept = 0;
	/**
	 * @brief Activity 暂停时的处理。
	*/
	virtual void pause() noexcept = 0;
	/**
	 * @brief Activity 取消暂停时的处理。
	*/
	virtual void resume() noexcept = 0;
	/**
	 * @brief 获取该 Activity 的唯一标识符。
	 * @return 唯一标识符。
	*/
	virtual uint32_t getID() noexcept = 0;

public:
	/**
	 * @brief 该 Acitivity 是否要独立运行。
	 * @return True 则独立, 否则非独立。
	*/
	virtual bool isIndependent() const noexcept = 0;
	/**
	 * @brief 独立地运行。
	*/
	virtual void runIndependently() = 0;

	/**
	 * @brief 处理事件。
	 * @param evt: SFML 的事件。
	*/
	virtual void handleEvent(const sf::Event& evt) = 0;
	/**
	 * @brief 更新。
	 * @param deltaTime 经过的时间。
	*/
	virtual void update(sf::Time deltaTime) = 0;

	virtual void onEnterSysloop() noexcept {}
	virtual void onExitSysloop() noexcept {}
}; // class IActivity

} // namespace GUI
