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
#include "Window.h"

namespace GUI {

class WindowWin32 final :
	public Window {
public:
	WindowWin32();
	virtual ~WindowWin32() noexcept;

public:
	/**
	 * @brief 给 main 函数用的 Create，因为要指定 nCmdShow。
	 * @param nCmdShow: WinMain 的形参。
	 * @return 创建是否成功。
	*/
	bool Create(int nCmdShow) noexcept;
	virtual bool Create() noexcept override;
	virtual void Close() noexcept override;

public:
	virtual void setCloseEnabled(bool enabled) noexcept override;
	virtual void setResizeEnabled(bool enabled) noexcept override;
	virtual void setMinimizeEnabled(bool enabled) noexcept override;
	virtual bool isCloseEnabled() const noexcept override;
	virtual bool isResizeEnabled() const noexcept override;
	virtual bool isMinimizeEnabled() const noexcept override;

	virtual void showMessageBox(std::string_view title, std::string_view text) const noexcept override;
	virtual void showMessageBox(std::wstring_view title, std::wstring_view text) const noexcept override;

	virtual void setWindowed() noexcept override;
	virtual bool setBorderless() noexcept override;
	virtual bool setFullscreen(sf::VideoMode mode) noexcept override;

protected:
	virtual void checkSizeInSystemLoop() noexcept override;

protected:
	HWND__* m_hwnd;
	sf::Vector2u m_oldSize;
	sf::Vector2u m_lastSizeWhenWindowed;
	sf::Vector2i m_lastPositionWhenWindowed;
};

} // namespace GUI
