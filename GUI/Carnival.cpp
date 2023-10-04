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
#include "Carnival.h"

#include "ToDefaultEntry.h"
#include "Activities/ToActivities.h"

namespace GUI {

Carnival::Carnival(sf::RenderWindow* r_window) :
	ref_window(r_window),
	m_keepRunning(false),
	m_transition(0),
	m_transitionTarget() {}

Carnival::~Carnival() {
	ref_window = nullptr;
}

void Carnival::cancelKeepRunning() {
	m_keepRunning = false;
	return;
}

void Carnival::setTransition(int t, size_t a0, size_t a1) {
	m_transition = t;
	m_transitionTarget[0] = a0;
	m_transitionTarget[1] = a1;
	return;
}

sf::RenderWindow& Carnival::getRenderWindow() {
	return *ref_window;
}

bool Carnival::handleTransition() {
	if (m_transition == 0)
		return true;
	bool isStop = true;
	int t = m_transition;
	if (t < 0) {
		isStop = false;
		t = -t;
	}
	size_t oldID = m_runningActivity->getID(); // 目前运行的Activity的ID。
	size_t newID = 0; // 将要运行的Activity的ID。
	size_t lca = 0; // 最近公共祖先（PopTo和PopPush用的）。
	// 计算将要运行的ID。
	switch (t) {
	case Transition::Switch:
		newID = m_transitionTarget[0];
		break;
	case Transition::Push:
		newID = m_transitionTarget[0];
		break;
	case Transition::Pop:
		if (m_activityStack.empty())
			lca = 1; // 栈空等同于 Exit。详见下面。
		else
			newID = m_activityStack.top();
		break;
	case Transition::PopTo:
		// 弹栈目标和新ID相同。
		newID = m_transitionTarget[0];
		lca = m_transitionTarget[0];
		break;
	case Transition::PopPush:
		newID = m_transitionTarget[1];
		if (newID != 0)
			lca = m_transitionTarget[0];
		break;
	case Transition::Exit:
		lca = 1; // 规定的 Exit。详见下面。
		break;
	default:
		break;
	}
	// 重置迁移量。
	m_transition = 0;
	m_transitionTarget[0] = m_transitionTarget[1] = 0;
	// 初步判断。
	if (newID == 0) {
		if (lca == 0)
			return true; // 规定的 都为0 就算 无动作。
		else {
			// 规定的 newID 为0 且 lca 不为0 就算 Exit。
			stopRunningActicity();
			return false;
		}
	}
	if (oldID == newID)
		return true; // 新旧相同 也算 无动作。

	// 下面的代码一定有以下条件:
	// newID 不为 0。

	std::unique_ptr<IActivity> newActivity = getActivity(newID);
	if (newActivity == nullptr)
		return true; // 创建失败。
	if (newID != newActivity->getID())
		return true; // 也是新旧相同。

	// 栈变迁。
	switch (t) {
	case Transition::Switch:
		if (isStop)
			stopRunningActicity();
		else
			pauseRunningActivity();
		break;
	case Transition::Push:
		if (isStop)
			stopRunningActicity();
		else
			pauseRunningActivity();
		m_activityStack.push(oldID);
		break;
	case Transition::Pop:
		if (isStop)
			stopRunningActicity();
		else
			pauseRunningActivity();
		m_activityStack.pop();
		break;
	case Transition::PopTo:
		if (lca == oldID || !stackContains(lca))
			break;
		if (isStop)
			stopRunningActicity();
		else
			pauseRunningActivity();
		while (m_activityStack.top() != lca) {
			if (isStop)
				stopPausedActivity(m_activityStack.top());
			m_activityStack.pop();
		}
		m_activityStack.pop();
		break;
	case Transition::PopPush:
		if (isStop)
			stopRunningActicity();
		else
			pauseRunningActivity();
		if (lca != oldID && stackContains(lca)) {
			while (m_activityStack.top() != lca) {
				if (isStop)
					stopPausedActivity(m_activityStack.top());
				m_activityStack.pop();
			}
		}
		else {
			m_activityStack.push(oldID);
		}
		break;
	}
	// 应用新Activity。
	m_runningActivity = std::move(newActivity);
#ifdef _DEBUG
	showStack();
#endif
	return true;
}

void Carnival::pauseRunningActivity() {
	m_runningActivity->pause();
	m_pausedActivities.emplace(m_runningActivity->getID(), std::move(m_runningActivity));
	return;
}

void Carnival::stopRunningActicity() {
	m_runningActivity->stop();
	m_runningActivity.reset();
	return;
}

void Carnival::stopPausedActivity(size_t id) {
	auto i = m_pausedActivities.find(id);
	if (i != m_pausedActivities.end()) {
		i->second->stop();
		m_pausedActivities.erase(i);
	}
	return;
}

bool Carnival::stackContains(size_t id) {
	const std::deque<size_t>& l = m_activityStack._Get_container();
	for (const size_t& i : l) {
		if (i == id) {
			return true;
		}
	}
	return false;
}

void Carnival::showStack() {
	const std::deque<size_t>& l = m_activityStack._Get_container();
	printf_s("Stack:");
	for (const size_t& i : l) {
		printf_s(" %zu", i);
	}
	printf_s(". Running: %zu.\n", m_runningActivity->getID());
	return;
}

std::unique_ptr<IActivity> Carnival::getActivity(size_t id) {
	std::unique_ptr<IActivity> res;
	auto i = m_pausedActivities.find(id);
	if (i != m_pausedActivities.end()) {
		res = std::move(i->second);
		m_pausedActivities.erase(i);
		res->resume();
	}
	else {
		res = createActivity(id);
		if (res != nullptr)
			res->start(*this);
	}
	return res;
}

std::unique_ptr<IActivity> Carnival::createActivity(size_t id) const {
	return GUI::createActivity(id);
}

} // namespace GUI
