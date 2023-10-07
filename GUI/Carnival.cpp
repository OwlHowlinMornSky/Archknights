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
#include "Carnival.h"

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

namespace GUI {

Carnival::Carnival() noexcept :
	m_keepRunning(false),
	m_transition(0),
	m_transitionTarget(),
	m_enableFullResizeMessage(true) {}

sf::RenderWindow& Carnival::getRenderWindow() noexcept {
	return *m_renderWindow;
}

void Carnival::cancelKeepRunning() noexcept {
	m_keepRunning = false;
}

void Carnival::setTransition(int t, uint32_t a0, uint32_t a1) noexcept {
	m_transition = t;
	m_transitionTarget[0] = a0;
	m_transitionTarget[1] = a1;
}

void Carnival::setFullResizeMessage(bool enabled) noexcept {
	m_enableFullResizeMessage = enabled;
}

bool Carnival::handleTransition() noexcept {
	if (m_transition == 0)
		return true;
	bool isStop = true;
	int t = m_transition;
	if (t < 0) {
		isStop = false;
		t = -t;
	}
	uint32_t oldID = m_runningActivity->getID(); // Ŀǰ���е�Activity��ID��
	uint32_t newID = 0; // ��Ҫ���е�Activity��ID��
	uint32_t lca = 0; // ����������ȣ�PopTo��PopPush�õģ���
	// ���㽫Ҫ���е�ID��
	switch (t) {
	case Transition::Switch:
		newID = m_transitionTarget[0];
		break;
	case Transition::Push:
		newID = m_transitionTarget[0];
		break;
	case Transition::Pop:
		if (m_activityStack.empty())
			lca = 1; // ջ�յ�ͬ�� Exit��������档
		else
			newID = m_activityStack.top();
		break;
	case Transition::PopTo:
		// ��ջĿ�����ID��ͬ��
		newID = m_transitionTarget[0];
		lca = m_transitionTarget[0];
		break;
	case Transition::PopPush:
		newID = m_transitionTarget[1];
		if (newID != 0)
			lca = m_transitionTarget[0];
		break;
	case Transition::Exit:
		lca = 1; // �涨�� Exit��������档
		break;
	default:
		break;
	}
	// ����Ǩ������
	m_transition = 0;
	m_transitionTarget[0] = m_transitionTarget[1] = 0;
	// �����жϡ�
	if (newID == 0) {
		if (lca == 0)
			return true; // �涨�� ��Ϊ0 ���� �޶�����
		else {
			// �涨�� newID Ϊ0 �� lca ��Ϊ0 ���� Exit��
			stopRunningActicity();
			return false;
		}
	}
	if (oldID == newID)
		return true; // �¾���ͬ Ҳ�� �޶�����

	// ����Ĵ���һ������������:
	// newID ��Ϊ 0��

	std::unique_ptr<IActivity> newActivity = getActivity(newID);
	if (newActivity == nullptr)
		return true; // ����ʧ�ܡ�
	if (newID != newActivity->getID())
		return true; // Ҳ���¾���ͬ��

	// ջ��Ǩ��
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
		try {
			m_activityStack.push(oldID);
		}
		catch (...) {
			;
		}
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
			try {
				m_activityStack.push(oldID);
			}
			catch (...) {
				;
			}
		}
		break;
	}
	// Ӧ����Activity��
	m_runningActivity = std::move(newActivity);
#ifdef _DEBUG
	showStack();
#endif
	return true;
}

void Carnival::pauseRunningActivity() noexcept {
	m_runningActivity->pause();
	try {
		m_pausedActivities.emplace(m_runningActivity->getID(), std::move(m_runningActivity));
	}
	catch (...) {
		if (m_runningActivity)
			m_runningActivity->stop();
		m_runningActivity.reset();
	}
	return;
}

void Carnival::stopRunningActicity() noexcept {
	m_runningActivity->stop();
	m_runningActivity.reset();
	return;
}

void Carnival::stopPausedActivity(uint32_t id) noexcept {
	try {
		std::map<uint32_t, std::unique_ptr<GUI::IActivity>>::iterator i = m_pausedActivities.find(id);
		if (i != m_pausedActivities.end()) {
			i->second->stop();
			m_pausedActivities.erase(i);
		}
	}
	catch (...) {
		;
	}
	return;
}

bool Carnival::stackContains(uint32_t id) noexcept {
	const std::deque<uint32_t>& l = m_activityStack._Get_container();
	for (const uint32_t& i : l) {
		if (i == id) {
			return true;
		}
	}
	return false;
}

void Carnival::showStack() noexcept {
#ifdef _DEBUG
	const std::deque<uint32_t>& l = m_activityStack._Get_container();
	std::cerr << "Stack:";
	for (const uint32_t& i : l) {
		std::cerr << " " << i;
	}
	std::cerr << ". Running: " << m_runningActivity->getID() << "." << std::endl;
#endif // _DEBUG
	return;
}

std::unique_ptr<IActivity> Carnival::getActivity(uint32_t id) noexcept {
	try {
		std::unique_ptr<IActivity> res;
		std::map<uint32_t, std::unique_ptr<GUI::IActivity>>::iterator i = m_pausedActivities.find(id);
		if (i != m_pausedActivities.end()) {
			res = std::move(i->second);
			m_pausedActivities.erase(i);
			res->resume();
		}
		else {
			res = this->createActivity(id);
			if (res != nullptr)
				res->start(*this);
		}
		return res;
	}
#ifdef _DEBUG
	catch (std::exception& e) {
		std::cerr << "Exception: GUI::Carnival: getActivity failed!" << std::endl;
		std::cerr << "    " << e.what() << std::endl;
	}
#endif // _DEBUG
	catch (...) {
#ifdef _DEBUG
		std::cerr << "Exception: GUI::Carnival: getActivity failed!" << std::endl;
		std::cerr << "    Unknown Exception." << std::endl;
#endif // _DEBUG
	}
	return std::unique_ptr<IActivity>();
}

} // namespace GUI
