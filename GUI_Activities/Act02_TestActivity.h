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

#ifdef _DEBUG
#include "../GUI/Activity.h"

#include <SFML/Graphics.hpp>

namespace Activity {

class Act02_TestActivity final :
	public GUI::Activity {
public:
	Act02_TestActivity();
	virtual ~Act02_TestActivity() noexcept override;

protected:
	virtual bool start(GUI::Window& wnd) noexcept override;
	virtual void stop() noexcept override;

public:
	virtual bool handleEvent(const sf::Event& evt) override;
	virtual void update(sf::Time dtime) override;

	virtual void OnEnterSysloop() noexcept override;
	virtual void OnExitSysloop() noexcept override;

protected:
	void updateSize() noexcept;
	void noticeSelectedMode() noexcept;

protected:
	GUI::Window* r_wnd;
	sf::RectangleShape m_shape;
	std::vector<sf::VideoMode> m_modes;
	size_t m_modeI;
	bool m_paused;
	bool m_disableClose;
	bool m_disableResize;
	bool m_disableMinimize;
	sf::Texture m_tex;
	sf::Sprite m_sp;
};

} // namespace Activity
#endif // _DEBUG
