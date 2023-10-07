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
*    ʵ���� ICarnival ʵ��֮һ CarnivalWin32��
*/
// SFML �Ķ������� �� Windows�� ֮ǰ��
#include <SFML/Graphics.hpp>
#include "framework.h"

#include "CarnivalWin32.h"

#include "../GUI/Callbacks.h"
#include "../GUI_Activities/ActivityIDs.h"

namespace GUI {

void CarnivalWin32::run() noexcept {
	// ���������� Ĭ����� Activity��
	m_runningActivity = this->createActivity(Activity::ID_DefaultEntry);
	if (m_runningActivity == nullptr) {
		return;
	}
	try {
		m_runningActivity->start(*this);
	}
	catch (...) {
		return;
	}

	std::function<void()> oldEnter = Callbacks::OnEnterSysloop;
	Callbacks::OnEnterSysloop = [this]()-> void {
		m_runningActivity->onEnterSysloop();
	};
	std::function<void()> oldExit = Callbacks::OnExitSysloop;
	Callbacks::OnExitSysloop = [this]()-> void {
		m_runningActivity->onExitSysloop();
	};

	// ����ѭ����
	while (handleTransition()) {
		try {
			if (m_runningActivity->isIndependent()) {
				m_runningActivity->runIndependently();
			}
			else {
				runTheActivity();
			}
		}
		catch (std::exception& e) {
			std::string err("Activity Exception:\n");
			err.append(e.what());
			this->showMessageBox("Archnights: Error", err, ICarnival::MBInfo::Error);
			this->setTransition(ICarnival::Transition::Pop);
		}
		catch (...) {
			std::string err("Activity Exception:\n");
			err.append("Unknown Exception.");
			this->showMessageBox("Archnights: Error", err, ICarnival::MBInfo::Error);
			this->setTransition(ICarnival::Transition::Pop);
		}
	}

	Callbacks::OnExitSysloop = oldExit;
	Callbacks::OnEnterSysloop = oldEnter;

	// �˳��� ��� Activity ջ��
	while (!m_activityStack.empty()) {
		m_activityStack.pop();
	}
	// ��ֹ���� Activity��
	for (const auto& i : m_pausedActivities) {
		i.second->stop();
	}
	// �ͷ����� Activity��
	m_pausedActivities.clear();
	return;
}

void CarnivalWin32::showMessageBox(std::string_view title, std::string_view text, MBInfo info) const noexcept {
	UINT type{ 0 };
	// �����Զ��� info ���� ����ϵͳ�����ֵ��
	switch (info) {
	case MBInfo::Info:
		type = MB_ICONINFORMATION;
		break;
	case MBInfo::Error:
		type = MB_ICONERROR;
		break;
	default:
		break;
	}
	try {
		MessageBoxA(m_hwnd, text.data(), title.data(), type);
	}
	catch (...) {
		;
	}
	return;
}

bool CarnivalWin32::isEnabledClose() const noexcept {
	return Callbacks::ButtonEnabled_Close;
}

bool CarnivalWin32::isEnabledResize() const noexcept {
	LONG_PTR style = GetWindowLongPtrW(m_hwnd, GWL_STYLE);
	return style & WS_SIZEBOX;
}

bool CarnivalWin32::isEnabledMinimize() const noexcept {
	LONG_PTR style = GetWindowLongPtrW(m_hwnd, GWL_STYLE);
	return style & WS_MINIMIZEBOX;
}

void CarnivalWin32::enableClose(bool enabled) const noexcept {
	HMENU hmenu = GetSystemMenu(m_hwnd, FALSE);
	// �޸ĳɹ������档
	if (EnableMenuItem(hmenu, SC_CLOSE, enabled ? MF_ENABLED : MF_GRAYED) != -1)
		Callbacks::ButtonEnabled_Close = enabled;
	return;
}

void CarnivalWin32::enableResize(bool enabled) const noexcept {
	LONG_PTR style = GetWindowLongPtrW(m_hwnd, GWL_STYLE);
	if (enabled) {
		SetWindowLongPtrW(m_hwnd, GWL_STYLE, style | (WS_SIZEBOX | WS_MAXIMIZEBOX));
	}
	else {
		SetWindowLongPtrW(m_hwnd, GWL_STYLE, style & (~(WS_SIZEBOX | WS_MAXIMIZEBOX)));
		ShowWindow(m_hwnd, SW_RESTORE); // ������ ������� ��ͬʱ ȡ���� ���е���󻯡�
	}
	return;
}

void CarnivalWin32::enableMinimize(bool enabled) const noexcept {
	LONG_PTR style = GetWindowLongPtrW(m_hwnd, GWL_STYLE);
	if (enabled) {
		SetWindowLongPtrW(m_hwnd, GWL_STYLE, style | WS_MINIMIZEBOX);
	}
	else {
		SetWindowLongPtrW(m_hwnd, GWL_STYLE, style & (~WS_MINIMIZEBOX));
		ShowWindow(m_hwnd, SW_RESTORE); // ������ ������С�� ��ͬʱ ȡ���� ���е���С����
	}
	return;
}

void CarnivalWin32::setFullwindow(bool full) noexcept {}

void CarnivalWin32::setFullscreen(bool full) noexcept {}

void CarnivalWin32::systemMessagePump() const noexcept {
	MSG msg{ 0 };
	while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	try {
		sf::Event evt;
		while (ref_window->pollEvent(evt)) {
			;
		}
	}
	catch (...) {
		;
	}
	return;
}

void CarnivalWin32::runTheActivity() {
	// ��ʼ����
	m_keepRunning = true;

	// ����ÿ֡�ĸ���ʱ�䡣
	sf::Clock clk;

	POINT oldsize{ 0 };
	{
		RECT clientrect{ 0 };
		GetClientRect(m_hwnd, &clientrect);
		oldsize = { clientrect.right, clientrect.bottom };
	}
	// �޸� Idle �ص���Ҫ�ѾɵĻص�����������
	std::function<void()> oldIdle = Callbacks::OnIdle;
	Callbacks::OnIdle = [this, &clk, &oldsize]() -> void {
		sf::Event evt;
		RECT clientrect{ 0 };
		GetClientRect(m_hwnd, &clientrect);
		if (oldsize.x != clientrect.right || oldsize.y != clientrect.bottom) {
			oldsize = { clientrect.right, clientrect.bottom };
			ref_window->setSize({ (unsigned int)oldsize.x, (unsigned int)oldsize.y });
			if (m_enableFullResizeMessage) {
				evt.type = sf::Event::Resized;
				evt.size.width = oldsize.x;
				evt.size.height = oldsize.y;
				m_runningActivity->handleEvent(evt);
			}
		}
		while (ref_window->pollEvent(evt)) {
			m_runningActivity->handleEvent(evt);
		}
		m_runningActivity->update(clk.restart());
	};

	// ��ѭ����
	MSG msg{ 0 };
	sf::Event evt;
	clk.restart();
	while (m_keepRunning) {
		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		while (ref_window->pollEvent(evt)) {
			m_runningActivity->handleEvent(evt);
		}

		m_runningActivity->update(clk.restart());
	}

	// �ָ��ɵ� Idle �ص���
	Callbacks::OnIdle = oldIdle;
	return;
}

} // namespace GUI
