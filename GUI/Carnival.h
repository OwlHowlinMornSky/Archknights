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

#include <map>
#include <stack>
#include "ICarnival.h"
#include "IActivity.h"

namespace GUI {

/**
 * @brief 对 Carnival 的基础实现。但仍有平台特异的方法未实现。
*/
class Carnival : public ICarnival {
public:
	Carnival(sf::RenderWindow* r_window) noexcept :
		ICarnival(r_window) {}
	virtual ~Carnival() noexcept override = default;

protected:
	bool handleTransition() noexcept;
	void pauseRunningActivity() noexcept;
	void stopRunningActicity() noexcept;
	void stopPausedActivity(size_t id) noexcept;
	bool stackContains(size_t id) noexcept;
	void showStack() noexcept;

	std::unique_ptr<IActivity> getActivity(size_t id) noexcept;

	virtual std::unique_ptr<IActivity> createActivity(size_t id) const noexcept = 0;

protected:
	std::unique_ptr<IActivity> m_runningActivity;
	std::stack<size_t> m_activityStack;
	std::map<size_t, std::unique_ptr<IActivity>> m_pausedActivities;
};

} // namespace GUI
