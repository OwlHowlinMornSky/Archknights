/*
*                    GNU AFFERO GENERAL PUBLIC LICENSE
*                       Version 3, 19 November 2007
*
*    Copyright (c) 2023  Tyler Parret True
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
*     Tyler Parret True (OwlHowlinMornSky) <mysteryworldgod@outlook.com>
*
* @Description
*     ������ ICarnival ʵ��֮һ CarnivalWin32��
*/
#pragma once

#include "../../GUI/Carnival.h"

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
	CarnivalWin32(HWND hwnd, sf::RenderWindow* r_window);

	virtual ~CarnivalWin32() override;

public:
	virtual void run() override;

	virtual void showMessageBox(std::string_view title, std::string_view text, MBInfo info = MBInfo::None) const override;

	virtual bool isEnabledClose() const override;
	virtual bool isEnabledResize() const override;
	virtual bool isEnabledMinimize() const override;

	virtual void enableClose(bool enabled) const override;
	virtual void enableResize(bool enabled) const override;
	virtual void enableMinimize(bool enabled) const override;

protected:
	/**
	 * @brief ���е�ǰ�� Activity��
	*/
	void runTheActivity();

protected:
	virtual std::unique_ptr<IActivity> createActivity(size_t id) const override;

protected:
	HWND m_hwnd; // Ҫ����Ĵ��� �ľ����
};

}

