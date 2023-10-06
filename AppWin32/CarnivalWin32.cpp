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

CarnivalWin32::CarnivalWin32(HWND hwnd, sf::RenderWindow* r_window) :
	Carnival(r_window),
	m_hwnd(hwnd) {}

CarnivalWin32::~CarnivalWin32() {}

void CarnivalWin32::run() {
	// ���������� Ĭ����� Activity��
	m_runningActivity = this->createActivity(Activity::ID_DefaultEntry);
	m_runningActivity->start(*this);

	// ����ѭ����
	while (handleTransition()) {
		if (m_runningActivity->isIndependent()) {
			m_runningActivity->runIndependently();
		}
		else {
			runTheActivity();
		}
	}

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

void CarnivalWin32::showMessageBox(std::string_view title, std::string_view text, MBInfo info) const {
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
	MessageBoxA(m_hwnd, text.data(), title.data(), type);
	return;
}

bool CarnivalWin32::isEnabledClose() const {
	return Callbacks::ButtonEnabled_Close;
}

bool CarnivalWin32::isEnabledResize() const {
	LONG_PTR style = GetWindowLongPtrW(m_hwnd, GWL_STYLE);
	return style & WS_SIZEBOX;
}

bool CarnivalWin32::isEnabledMinimize() const {
	LONG_PTR style = GetWindowLongPtrW(m_hwnd, GWL_STYLE);
	return style & WS_MINIMIZEBOX;
}

void CarnivalWin32::enableClose(bool enabled) const {
	HMENU hmenu = GetSystemMenu(m_hwnd, FALSE);
	// �޸ĳɹ������档
	if (EnableMenuItem(hmenu, SC_CLOSE, enabled ? MF_ENABLED : MF_GRAYED) != -1)
		Callbacks::ButtonEnabled_Close = enabled;
	return;
}

void CarnivalWin32::enableResize(bool enabled) const {
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

void CarnivalWin32::enableMinimize(bool enabled) const {
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

void CarnivalWin32::setFullwindow(bool full) {}

void CarnivalWin32::setFullscreen(bool full) {}

void CarnivalWin32::runTheActivity() {
	// ��ʼ����
	m_keepRunning = true;

	// ����ÿ֡�ĸ���ʱ�䡣
	sf::Clock clk;
	float dt = 0.0f;

	POINT oldsize{ 0 };
	{
		RECT clientrect{ 0 };
		GetClientRect(m_hwnd, &clientrect);
		oldsize = { clientrect.right, clientrect.bottom };
	}
	// �޸� Idle �ص���Ҫ�ѾɵĻص�����������
	std::function<void()> oldIdle = Callbacks::OnIdle;
	Callbacks::OnIdle = [this, &dt, &clk, &oldsize]() -> void {
		sf::Event evt;

		RECT clientrect{ 0 };
		GetClientRect(m_hwnd, &clientrect);
		if (oldsize.x != clientrect.right || oldsize.y != clientrect.bottom) {
			oldsize = { clientrect.right, clientrect.bottom };
			ref_window->setSize({ (unsigned int)clientrect.right, (unsigned int)clientrect.bottom });
			evt.type = sf::Event::Resized;
			evt.size.width = oldsize.x;
			evt.size.height = oldsize.y;
			m_runningActivity->handleEvent(evt);
		}

		while (ref_window->pollEvent(evt)) {
			m_runningActivity->handleEvent(evt);
		}
		dt = clk.restart().asSeconds();
		m_runningActivity->update(dt);
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

		dt = clk.restart().asSeconds();
		m_runningActivity->update(dt);
	}

	// �ָ��ɵ� Idle �ص���
	Callbacks::OnIdle = oldIdle;
	return;
}

} // namespace GUI
