#include "Carnival.h"

#include "ToDefaultEntry.h"

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
	if (m_transition == 0) {
		return true;
	}
	bool isStop = false;
	int t = m_transition;
	size_t tt[2]{ m_transitionTarget[0], m_transitionTarget[1] };
	if (m_transition < 0) {
		isStop = true;
		t = -t;
	}
	m_transition = 0;
	m_transitionTarget[0] = m_transitionTarget[1] = 0;
	switch (t) {
	case Transition::Switch:
		if (tt[0] != 0 && m_runningActivity->getID() != tt[0]) {
			if (isStop) {
				m_runningActivity->stop();
			}
			else {
				m_runningActivity->pause();
				m_pausedActivities.emplace(m_runningActivity->getID(), std::move(m_runningActivity));
			}
			m_runningActivity = getActivity(tt[0]);
		}
		break;
	case Transition::Push:
		if (tt[0] != 0 && m_runningActivity->getID() != tt[0]) {
			size_t id = m_runningActivity->getID();
			if (isStop) {
				m_runningActivity->stop();
			}
			else {
				m_runningActivity->pause();
				m_pausedActivities.emplace(id, std::move(m_runningActivity));
			}
			m_activityStack.push(id);
			m_runningActivity = getActivity(tt[0]);
		}
		break;
	case Transition::Pop:
		if (isStop) {
			m_runningActivity->stop();
		}
		else {
			m_runningActivity->pause();
			m_pausedActivities.emplace(m_runningActivity->getID(), std::move(m_runningActivity));
		}
		if (m_activityStack.empty()) {
			return false;
		}
		m_runningActivity = getActivity(m_activityStack.top());
		m_activityStack.pop();
		break;
	case Transition::PopTo:
	{
		if (tt[0] == 0 || m_runningActivity->getID() == tt[0]) {
			break;
		}
		const std::deque<size_t>& l = m_activityStack._Get_container();
		bool found = false;
		for (const size_t& i : l) {
			if (i == tt[0]) {
				found = true;
				break;
			}
		}
		if (!found) {
			break;
		}
		if (isStop) {
			m_runningActivity->stop();
		}
		else {
			m_runningActivity->pause();
			m_pausedActivities.emplace(m_runningActivity->getID(), std::move(m_runningActivity));
		}
		while (m_activityStack.top() != tt[0]) {
			if (isStop) {
				stopPausedActivity(m_activityStack.top());
			}
			m_activityStack.pop();
		}
		m_runningActivity = getActivity(m_activityStack.top());
		m_activityStack.pop();
		break;
	}
	case Transition::PopPush:
	{
		if (tt[0] == 0 || tt[1] == 0 || m_runningActivity->getID() == tt[0] || m_runningActivity->getID() == tt[1]) {
			break;
		}
		const std::deque<size_t>& l = m_activityStack._Get_container();
		bool found = false;
		for (const size_t& i : l) {
			if (i == tt[0]) {
				found = true;
				break;
			}
		}
		if (!found) {
			size_t id = m_runningActivity->getID();
			if (isStop) {
				m_runningActivity->stop();
			}
			else {
				m_runningActivity->pause();
				m_pausedActivities.emplace(id, std::move(m_runningActivity));
			}
			m_activityStack.push(id);
			m_runningActivity = getActivity(tt[1]);
			break;
		}
		if (isStop) {
			m_runningActivity->stop();
		}
		else {
			m_runningActivity->pause();
			m_pausedActivities.emplace(m_runningActivity->getID(), std::move(m_runningActivity));
		}
		while (m_activityStack.top() != tt[0]) {
			if (isStop) {
				stopPausedActivity(m_activityStack.top());
			}
			m_activityStack.pop();
		}
		m_runningActivity = getActivity(tt[1]);
		break;
	}
	case Transition::Exit:
		m_runningActivity->stop();
		while (!m_activityStack.empty()) {
			stopPausedActivity(m_activityStack.top());
			m_activityStack.pop();
		}
		return false;
		break;
	default:
		break;
	}
	return true;
}

std::unique_ptr<IActivity> Carnival::getActivity(size_t id) {
	std::unique_ptr<IActivity> res;
	auto i = m_pausedActivities.find(id);
	if (i != m_pausedActivities.end()) {
		//("find %zu\n", id);
		res = std::move(i->second);
		m_pausedActivities.erase(i);
		res->resume();
	}
	else {
		//printf_s("create %zu\n", id);
		res = createActivity(id);
		res->start(*this);
	}
	return res;
}

void Carnival::stopPausedActivity(size_t id) {
	auto i = m_pausedActivities.find(id);
	if (i != m_pausedActivities.end()) {
		i->second->stop();
		m_pausedActivities.erase(i);
	}
	return;
}

} // namespace GUI
