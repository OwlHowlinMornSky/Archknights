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

#ifdef _DEBUG
#include <MysteryEngine/Client/Window.h>

#include <SFML/Graphics.hpp>

namespace Activity {

class Act02_TestActivity final :
	public ME::Activity {
public:
	Act02_TestActivity();
	virtual ~Act02_TestActivity() noexcept override;

protected:
	virtual bool start(ME::Window& wnd) noexcept override;
	virtual void stop() noexcept override;

public:
	virtual bool handleEvent(const sf::Event& evt) override;
	virtual void update(sf::Time dtime) override;

	virtual void OnEnterSysloop() noexcept override;
	virtual void OnExitSysloop() noexcept override;

protected:
	/**
	 * @brief 以新的窗口大小更新布局
	*/
	void updateSize() noexcept;
	/**
	 * @brief 在控制台输出当前选中的屏幕模式
	*/
	void noticeSelectedMode() noexcept;

protected:
	ME::WndRef r_wnd;
	sf::RectangleShape m_shape; // 中心旋转的正方形
	std::vector<sf::VideoMode> m_modes; // 所有屏幕模式的副本
	size_t m_modeI; // 选择的屏幕模式标记
	bool m_paused; // 暂停标记
	sf::Texture m_tex; // 调试信息图片的纹理
	sf::Sprite m_sp;
};

} // namespace Activity
#endif // _DEBUG
