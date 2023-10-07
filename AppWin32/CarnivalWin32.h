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
	CarnivalWin32(sf::RenderWindow* r_window) noexcept :
		Carnival(r_window),
		m_hwnd(r_window->getSystemHandle()) {}
	virtual ~CarnivalWin32() noexcept override {}

public:
	virtual void run() noexcept override;

	virtual void showMessageBox(std::string_view title,
								std::string_view text,
								MBInfo info = MBInfo::None) const noexcept override;

	virtual bool isEnabledClose() const noexcept override;
	virtual bool isEnabledResize() const noexcept override;
	virtual bool isEnabledMinimize() const noexcept override;

	virtual void enableClose(bool enabled) const noexcept override;
	virtual void enableResize(bool enabled) const noexcept override;
	virtual void enableMinimize(bool enabled) const noexcept override;

	virtual void setFullwindow(bool full) noexcept override;
	virtual void setFullscreen(bool full) noexcept override;

	virtual void systemMessagePump() const noexcept override;

protected:
	/**
	 * @brief ���е�ǰ�� Activity��
	*/
	void runTheActivity();

	/**
	 * @brief [ע��] ����������� "ToActivities.cpp" ��ʵ�ֵġ�
	*/
	virtual std::unique_ptr<IActivity> createActivity(size_t id) const noexcept override;

protected:
	HWND m_hwnd; // Ҫ����Ĵ��� �ľ����
};

}

