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
	virtual void cancelKeepRunning() override;

	virtual void setTransition(int t, size_t a0 = 0, size_t a1 = 0) override;

	virtual sf::RenderWindow& getRenderWindow() override;

protected:
	bool handleTransition();
	void pauseRunningActivity();
	void stopRunningActicity();
	void stopPausedActivity(size_t id);
	bool stackContains(size_t id);

	std::unique_ptr<IActivity> getActivity(size_t id);
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
