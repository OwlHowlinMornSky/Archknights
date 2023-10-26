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

#include "../GUI/Activity.h"

#include <SFML/Graphics.hpp>

namespace Activity {

/**
 * @brief Ĭ�ϳ�ʼActivity��
*/
class Act01_DefaultEntry final :
	public GUI::Activity {
public:
	Act01_DefaultEntry() noexcept;
	virtual ~Act01_DefaultEntry() noexcept;

protected:
	virtual bool start(GUI::Window& wnd) noexcept override;
	virtual void stop() noexcept override;

public:
	virtual void handleEvent(const sf::Event& evt) override;
	virtual void update(sf::Time dtime) override;

	virtual void OnEnterSysloop() noexcept override;
	virtual void OnExitSysloop() noexcept override;

protected:
	GUI::Window* r_wnd;
	bool m_haveRunned;
	std::unique_ptr<sf::Texture> g_tex;
	std::unique_ptr<sf::Sprite> g_sp;
};

} // namespace Activity
