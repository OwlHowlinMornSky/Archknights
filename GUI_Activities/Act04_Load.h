﻿/*
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

#include "../GUI/Activity.h"
#include "../GUI/IBgm.h"

#include <SFML/Graphics.hpp>

namespace Activity {

class Act04_Load final :
	public GUI::Activity {
public:
	Act04_Load();
	virtual ~Act04_Load() noexcept;

protected:
	virtual bool start(GUI::Window& wnd) noexcept override;
	virtual void stop() noexcept override;

public:
	virtual bool handleEvent(const sf::Event& evt) override;
	virtual void update(sf::Time dtime) override;

	virtual void OnEnterSysloop() noexcept override;
	virtual void OnExitSysloop() noexcept override;

protected:
	void updateSize(sf::Vector2u newWindowSize);

protected:
	int m_status;
	GUI::Window* r_wnd;
	std::unique_ptr<GUI::IBgm> m_bgm;
	sf::Texture m_tex;
	sf::Sprite m_sp;
	sf::RectangleShape m_blackBar[2];
	//sf::Text m_text;
};

}
