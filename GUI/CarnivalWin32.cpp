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
#include "CarnivalWin32.h"

#include "WindowWin32.h"
#include <assert.h>
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace GUI {

CarnivalWin32::CarnivalWin32(bool mutipleWindows) :
	Carnival(mutipleWindows) {}

bool CarnivalWin32::emplaceWindow(std::unique_ptr<Activity>&& activity) {
	assert(activity != nullptr);
	if (activity == nullptr)
		return false;
	if (m_mutipleWindows) {
		std::unique_ptr<WindowWin32> wnd = std::make_unique<WindowWin32>();
		if (!wnd->Create(SW_SHOWNORMAL))
			return false;
		wnd->changeActivity(std::move(activity));
		m_wnds.push_front(std::move(wnd));
	}
	else {
		assert(m_singleWnd == nullptr);
		if (m_singleWnd != nullptr)
			return false;
		std::unique_ptr<WindowWin32> wnd = std::make_unique<WindowWin32>();
		if (!wnd->Create(SW_SHOWNORMAL))
			return false;
		wnd->changeActivity(std::move(activity));
		m_singleWnd = std::move(wnd);
	}
	return true;
}

void CarnivalWin32::showErrorMessageBox(std::string_view title, std::string_view text) const noexcept {
	MessageBoxA(NULL, text.data(), title.data(), MB_ICONERROR);
	return;
}

void GUI::CarnivalWin32::resetSleepCounter() noexcept {
	SetThreadExecutionState(ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
	return;
}

void CarnivalWin32::setSleepEnabled(bool allowSleep) noexcept {
	if (allowSleep)
		SetThreadExecutionState(ES_CONTINUOUS);
	else
		SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
	return;
}

void CarnivalWin32::systemMessagePump() const noexcept {
	MSG msg{ 0 };
	while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return;
}

} // namespace GUI
