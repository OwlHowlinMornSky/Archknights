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
	Carnival() noexcept;
	virtual ~Carnival() noexcept override;

public:
	virtual void run() noexcept override final;

	virtual sf::RenderWindow& getRenderWindow() noexcept override final;

	virtual void meDependentActivityStopRunning() noexcept override final;
	virtual void meActivitySetTransition(int t, uint32_t a0 = 0, uint32_t a1 = 0) noexcept override final;

	virtual void setSizingAsResized(bool enabled) noexcept override final;
	virtual bool isSizingAsResized() const noexcept final;

	virtual void windowSetClientSize(uint32_t w, uint32_t h) noexcept override final;
	virtual bool windowIsCloseEnabled() const noexcept override final;
	virtual bool windowIsResizeEnabled() const noexcept override final;
	virtual bool windowIsMinimizeEnabled() const noexcept override final;

	virtual WindowType windowGetWindowType() const noexcept override final;

	virtual const IFontMgr& getFontMgr() const noexcept final;

protected:
	/**
	 * @brief 
	 * @return True if any change happened.
	*/
	bool handleTransition() noexcept;
	void pauseRunningActivity() noexcept;
	void stopRunningActicity() noexcept;
	void stopPausedActivity(uint32_t id) noexcept;
	bool stackContains(uint32_t id) noexcept;
	void showStack() noexcept;

	std::unique_ptr<IActivity> getActivity(uint32_t id) noexcept;

protected:
	virtual void runTheActivity() = 0;
	virtual std::unique_ptr<IActivity> createActivity(uint32_t id) const noexcept = 0;

protected:
	bool m_enabledResize;
	bool m_enabledMinimize;
	bool m_enableFullResizeMessage;
	bool m_keepRunning;
	bool m_mainLoopKeep;
	int m_transition;
	WindowType m_windowType;
	uint32_t m_transitionTarget[2];
	std::unique_ptr<sf::RenderWindow> m_renderWindow;
	std::unique_ptr<IActivity> m_runningActivity;
	std::unique_ptr<IFontMgr> m_fontMgr;
	std::stack<uint32_t> m_activityStack;
	std::map<uint32_t, std::unique_ptr<IActivity>> m_pausedActivities;
};

} // namespace GUI
