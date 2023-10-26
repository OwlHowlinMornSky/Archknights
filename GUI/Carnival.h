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

#include <list>
#include <functional>
#include "Window.h"

namespace GUI {

/**
 * @brief ����״̬�Ļص���
 * @brief ��������ڵ�Ŀ����Ҫ���Ƿ�ֹ �ƶ����� �� �ı䴰�ڴ�С ʱ �����߼���ס��
 * @brief ������ ���ö�ʱ������ʱ���Ļص� MyTimerProc �����������⡣
 * @brief ���������߼���Ǩ��ʱ��Ҫ���������������һ���ǵøĻ�������Ϊ�г�ʼ�պ�������
*/
extern std::function<void()> OnIdle;
/**
 * @brief ������˳�ϵͳѭ��ʱ�Ļص�������Ϊ true ��Ϊ ���룬false Ϊ�˳���
 * @brief �������������� ��ʼ�ƶ����� ���� ��ʼ�ı䴰�ڴ�С ʱ���õġ�
 * @brief ����֪ͨ�����һЩ����������ͣ��Ϸʲô�ģ���
 * @brief ���� �˳��ƶ���ı��С ��ʱ�� ͻȻ����һ�� �������� ������ʮ�� �� ֡��
*/
extern std::function<void(bool)> OnSystemLoop;

/**
 * @brief Carnival: �� RenderWindow ������ Activity �Ĺ����ࡣ
*/
class Carnival {
public:
	Carnival(bool mutipleWindows);
	virtual ~Carnival() noexcept;

public:
	static void setup(bool mutipleWindows = true) noexcept;
	static Carnival& instance() noexcept;
	static void drop() noexcept;

public:
	/**
	 * @brief ��ʼִ�С�
	*/
	void run() noexcept;

	bool pushWindow(std::unique_ptr<Window>&& wnd);
	virtual bool emplaceWindow(std::unique_ptr<Activity>&& activity) = 0;

public:
	/**
	 * @brief ��ʾһ����Ϣ��
	 * @param title: ��Ϣ��ı��⡣
	 * @param text: ��Ϣ������ݡ�
	*/
	virtual void showErrorMessageBox(std::string_view title, std::string_view text) const noexcept = 0;

	/**
	 * @brief ����˯�߼��������˴���˵˯�߰����������ر���Ļ������
	*/
	virtual void resetSleepCounter() noexcept = 0;
	/**
	 * @brief ������ֹϵͳ˯�ߣ��˴���˵˯�߰����������ر���Ļ������
	 * @param allowSleep: �Ƿ�����˯�ߣ���Ϊ false �� ��ֹ˯�� �� ������Ļ������
	*/
	virtual void setSleepEnabled(bool allowSleep) noexcept = 0;

protected:
	void removeStoppedWindows() noexcept;
	void onIdle();
	void onIdleSingle();
	void onSystemLoop(bool enter);
	void onSystemLoopSingle(bool enter);

	virtual void systemMessagePump() const noexcept = 0;

protected:
	bool m_mutipleWindows;
	sf::Clock m_clk;
	std::unique_ptr<Window> m_singleWnd;
	std::list<std::unique_ptr<Window>> m_wnds;

	static std::unique_ptr<Carnival> s_instance;
}; // class ICarnival

} // namespace GUI
