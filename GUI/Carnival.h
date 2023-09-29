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
*
*/
#pragma once

#include <map>
#include <stack>
#include "ICarnival.h"
#include "IActivity.h"

namespace GUI {

class Carnival : public ICarnival {
public:
	Carnival(sf::RenderWindow* r_window);
	virtual ~Carnival() override;

public:
	virtual void cancelKeepRunning() override final;

	virtual void setTransition(int t, size_t a0 = 0, size_t a1 = 0) override final;

	virtual sf::RenderWindow& getRenderWindow() override final;

protected:
	bool handleTransition();
	void pauseRunningActivity();
	void stopRunningActicity();
	void stopPausedActivity(size_t id);
	bool stackContains(size_t id);
	void showStack();

	std::unique_ptr<IActivity> getActivity(size_t id);

protected:
	virtual std::unique_ptr<IActivity> createActivity(size_t id) const = 0;

protected:
	bool m_keepRunning;
	int m_transition;
	size_t m_transitionTarget[2];
	sf::RenderWindow* ref_window;
	std::unique_ptr<IActivity> m_runningActivity;
	std::stack<size_t> m_activityStack;
	std::map<size_t, std::unique_ptr<IActivity>> m_pausedActivities;
};

} // namespace GUI
