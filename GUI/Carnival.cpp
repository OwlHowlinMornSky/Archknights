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
	if (m_transition == 0)
		return true;
	bool isStop = false;
	int t = m_transition;
	m_transition = 0;
	if (t < 0) {
		isStop = true;
		t = -t;
	}
	size_t oldID = m_runningActivity->getID();
	size_t newID = 0;
	size_t lca = 0;
	switch (t) {
	case Transition::Switch:
		newID = m_transitionTarget[0];
		break;
	case Transition::Push:
		newID = m_transitionTarget[0];
		break;
	case Transition::Pop:
		if (m_activityStack.empty())
			lca = 1;
		else
			newID = m_activityStack.top();
		break;
	case Transition::PopTo:
		newID = m_transitionTarget[0];
		lca = m_transitionTarget[0];
		break;
	case Transition::PopPush:
		newID = m_transitionTarget[1];
		if (newID != 0)
			lca = m_transitionTarget[0];
		break;
	case Transition::Exit:
		lca = 1;
		break;
	default:
		break;
	}
	m_transitionTarget[0] = m_transitionTarget[1] = 0;
	if (newID == 0) {
		if (lca == 0)
			return true;
		else {
			stopRunningActicity();
			return false;
		}
	}
	if (oldID == newID)
		return true;
	std::unique_ptr<IActivity> newActivity = getActivity(newID);
	if (newActivity == nullptr)
		return true;

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
	m_runningActivity = std::move(newActivity);
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

} // namespace GUI
