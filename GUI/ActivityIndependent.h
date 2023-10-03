/*
*                    GNU AFFERO GENERAL PUBLIC LICENSE
*                       Version 3, 19 November 2007
*
*    Copyright (c) 2023  Tyler Parret True
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
*     Tyler Parret True (OwlHowlinMornSky) <mysteryworldgod@outlook.com>
*/
#pragma once

#include "IActivity.h"

namespace GUI {

class ActivityIndependent : public IActivity {
public:
	ActivityIndependent() = default;
	virtual ~ActivityIndependent() override = default;

public:
	/**
	 * @brief 该 Acitivity 是否要独立运行。
	 * @return True 则独立, 否则非独立。
	*/
	virtual bool isIndependent() const override final;
	/**
	 * @brief 独立地运行。
	*/
	virtual void runIndependently() override = 0;

	/**
	 * @brief 处理事件。
	 * @param evt: SFML 的事件。
	*/
	virtual void handleEvent(const sf::Event& evt) override final;
	/**
	 * @brief 更新。
	 * @param dt 经过的时间。
	*/
	virtual void update(float dt) override final;
};

} // namespace GUI
