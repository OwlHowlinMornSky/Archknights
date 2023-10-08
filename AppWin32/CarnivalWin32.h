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
*    ������ ICarnival ʵ��֮һ CarnivalWin32��
*/
#pragma once

#include "../GUI/Carnival.h"

namespace GUI {

/**
 * @brief Carnival �� Win32 ��ʵ�֡�
*/
class CarnivalWin32 final :
	public Carnival {
public:
	/**
	 * @brief ���캯���������� Ҫ����Ĵ��� ��ʼ����
	 * @param hwnd: Ҫ����Ĵ��� �ľ����
	 * @param r_window: Ҫ����Ĵ��� �� RenderWindow ָ�롣
	*/
	CarnivalWin32(HWND hWnd);
	virtual ~CarnivalWin32() override;

public:
	virtual void run() noexcept override;

public:
	virtual void systemShowMessageBox(std::string_view title,
								std::string_view text,
								MBInfo info = MBInfo::None) const noexcept override;

	virtual void windowSetClientSize(uint32_t w, uint32_t h) noexcept override;

	virtual bool windowIsCloseEnabled() const noexcept override;
	virtual bool windowIsResizeEnabled() const noexcept override;
	virtual bool windowIsMinimizeEnabled() const noexcept override;

	virtual void windowSetCloseEnabled(bool enabled) noexcept override;
	virtual void windowSetResizeEnabled(bool enabled) noexcept override;
	virtual void windowSetMinimizeEnabled(bool enabled) noexcept override;

	virtual bool windowSetBorderless() noexcept override;
	virtual bool windowSetFullscreen(sf::VideoMode mode) noexcept override;
	virtual void windowSetWindowed() noexcept override;
	virtual WindowType windowGetWindowType() const noexcept override;

	virtual void systemMessagePump(bool callerDoWantHandleThem) const noexcept override;

protected:
	/**
	 * @brief ���е�ǰ�� Activity��
	*/
	void runTheActivity();

	virtual std::unique_ptr<IActivity> createActivity(uint32_t id) const noexcept override;

protected:
	bool m_enabledResize;
	bool m_enabledMinimize;
	WindowType m_windowType;
	sf::VideoMode m_lastMode;
	HWND m_hwnd; // Ҫ����Ĵ��� �ľ����
	sf::Vector2u m_sizeBefore; // ȫ�����ޱ߿�֮ǰ�Ĵ��ڴ�С��
	sf::Vector2i m_positionBefore; // ȫ�����ޱ߿�֮ǰ�Ĵ���λ�á�
};

} // namespace GUI

