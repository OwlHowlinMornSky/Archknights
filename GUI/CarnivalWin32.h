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
#include "Carnival.h"

namespace GUI {

/**
 * @brief Carnival �� Win32 �µ�ʵ�֡�
*/
class CarnivalWin32 final :
    public Carnival {
	friend class Carnival; // ����Ϊ��Ψһʵ���� setup �ܹ���������ࡣ
protected:
	CarnivalWin32(bool mutipleWindows);
public:
	virtual ~CarnivalWin32() = default;

public:
	virtual bool emplaceWindow(std::unique_ptr<Activity>&& activity, bool foreground = false) override;

	virtual void showErrorMessageBox(std::string_view title, std::string_view text) const noexcept override;
	virtual void resetSleepCounter() noexcept override;
	virtual void setSleepEnabled(bool allowSleep) noexcept override;

protected:
	virtual void systemMessagePump() const noexcept override;
};

} // namespace GUI
