/*
*    Archknights
*
*    Copyright (C) 2023-2024  Tyler Parret True
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

#include <MysteryEngine/Client/Window.h>

#include <SFML/Graphics.hpp>

namespace Activity {

class Act03_Opening final :
	public ME::Activity {
public:
	Act03_Opening();
	virtual ~Act03_Opening() noexcept override;

protected:
	virtual bool prepare(ME::Window& wnd) noexcept override;
	virtual void start() noexcept override;
	virtual void stop() noexcept override;

public:
	virtual bool handleEvent(const sf::Event& evt) override;
	virtual void update(sf::Time dtime) override;

	virtual void onEnterSysloop() noexcept override;
	virtual void onExitSysloop() noexcept override;

protected:
	void updateSize();

protected:
	int m_status; // 状态
	ME::WndRef r_wnd;
	sf::Time m_timer;
	sf::Texture m_tex[3];
	sf::Sprite circle[3];
};

} // namespace Activity
