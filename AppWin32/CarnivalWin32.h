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
*
* @Description
*    定义了 ICarnival 实例之一 CarnivalWin32。
*/
#pragma once

#include "../GUI/Carnival.h"

namespace GUI {

/**
 * @brief Carnival 在 Win32 的实现。
*/
class CarnivalWin32 final :
	public Carnival {
public:
	/**
	 * @brief 构造函数。必须以 要管理的窗口 初始化。
	 * @param hwnd: 要管理的窗口 的句柄。
	 * @param r_window: 要管理的窗口 的 RenderWindow 指针。
	*/
	CarnivalWin32(HWND hWnd);
	virtual ~CarnivalWin32() override;

public:
	virtual void systemMessagePump(bool callerDoWantHandleThem) const noexcept override;

	virtual void systemShowMessageBox(std::string_view title,
								std::string_view text,
								MBInfo info = MBInfo::None) const noexcept override;


	virtual void windowSetCloseEnabled(bool enabled) noexcept override;
	virtual void windowSetResizeEnabled(bool enabled) noexcept override;
	virtual void windowSetMinimizeEnabled(bool enabled) noexcept override;

	virtual bool windowSetBorderless() noexcept override;
	virtual bool windowSetFullscreen(sf::VideoMode mode) noexcept override;
	virtual void windowSetWindowed() noexcept override;

protected:
	virtual void runTheActivity() override;
	virtual std::unique_ptr<IActivity> createActivity(uint32_t id) const noexcept override;

protected:
	sf::VideoMode m_lastMode;
	HWND m_hwnd; // 要管理的窗口 的句柄。
	sf::Vector2u m_sizeBefore; // 全屏或无边框之前的窗口大小。
	sf::Vector2i m_positionBefore; // 全屏或无边框之前的窗口位置。
};

} // namespace GUI

