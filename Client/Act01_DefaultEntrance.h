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
#ifdef _DEBUG
#include <SFML/Graphics.hpp>
#endif // _DEBUG
#include "API.h"

namespace Activity {

/**
 * @brief 默认初始Activity。
*/
class CLIENT_API Act01_DefaultEntrance final :
	public ME::Activity {
public:
	Act01_DefaultEntrance();
	virtual ~Act01_DefaultEntrance() noexcept override;

protected:
	virtual bool start(ME::Window& wnd) noexcept override;
	virtual void stop() noexcept override;

public:
	virtual bool handleEvent(const sf::Event& evt) override;
	virtual void update(sf::Time dtime) override;

	virtual void OnEnterSysloop() noexcept override;
	virtual void OnExitSysloop() noexcept override;

protected:
	ME::WndRef r_wnd;
#ifdef _DEBUG
	sf::Sprite m_sp;
	sf::Texture m_tex; // 调试显示图片的纹理
#endif // _DEBUG
};

} // namespace Activity
