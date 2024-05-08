/*
*    Mystery Engine
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

#include <MysteryEngine/Client/Carnival.h>

#ifdef SFML_SYSTEM_WINDOWS

namespace ME {

/**
 * @brief Carnival 在 Win32 下的实现。
*/
class CarnivalWin32 final :
    public Carnival {
	friend class Carnival; // 这是为了唯一实例的 setup 能够构造这个类。
protected:
	CarnivalWin32(bool mutipleWindows);
public:
	virtual ~CarnivalWin32() override = default;

public:
	virtual void Run() noexcept override;

	virtual bool emplaceWindow(std::unique_ptr<Activity>&& activity, bool foreground = false) override;

	virtual void showErrorMessageBox(std::string_view title, std::string_view text) const noexcept override;
	virtual void resetSleepCounter() noexcept override;
	virtual void setSleepEnabled(bool allowSleep) noexcept override;

protected:
	virtual void systemMessagePump() const noexcept override;
}; // class CarnivalWin32

} // namespace ME

#endif
