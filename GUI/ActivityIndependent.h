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

#include "IActivity.h"

namespace GUI {

/**
 * @brief 独立 Activity。
*/
class ActivityIndependent : public IActivity {
public:
	ActivityIndependent() = default;
	virtual ~ActivityIndependent() override = default;

public:
	// 禁止修改。
	virtual bool isIndependent() const noexcept override final;
	// 独立必须实现。
	virtual void runIndependently() override = 0;

	// 独立禁止使用。
	virtual void handleEvent(const sf::Event& evt) override final;
	// 独立禁止使用。
	virtual void update(sf::RenderWindow& window, sf::Time deltaTime) override final;
};

} // namespace GUI
