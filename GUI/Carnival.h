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
#include "Window.h"

namespace GUI {

/**
 * @brief Carnival: �������д��ڵ��ࡣ
*/
class Carnival {
protected:
	/**
	 * @brief ���캯������Ϊ ������ģʽ �� �ര��ģʽ��
	 * @param mutipleWindows: �Ƿ����öര�ڡ�
	*/
	Carnival(bool mutipleWindows);
public:
	/**
	 * @brief ����������
	*/
	virtual ~Carnival() noexcept;

public:
	/**
	 * @brief ��ʼ����Carnival ֻ����һ��ʵ�����ڸ÷����й��졣
	 * @param mutipleWindows: �Ƿ����öര�ڡ�
	*/
	static void setup(bool mutipleWindows = true) noexcept;
	/**
	 * @brief ��ȡ Carnival ��Ψһʵ��������ǰ������ setup��
	 * @return Carnival ʵ�������á�
	*/
	static Carnival& instance() noexcept;
	/**
	 * @brief �������� Carnival ʵ����
	*/
	static void drop() noexcept;

public:
	/**
	 * @brief ��ʼִ�С�
	*/
	void run() noexcept;

	/**
	 * @brief �� Carnival ��Ӵ��ڡ��ڵ�����ģʽ����ӵڶ������Ժ�Ĵ��ڶ���ʧ�ܡ�
	 * @param wnd: ���ڣ������Ѿ� Create���Һ�����Ч Activity��
	 * @return ����Ƿ�ɹ���
	*/
	bool pushWindow(std::unique_ptr<Window>&& wnd);
	/**
	 * @brief �� Carnival ����������һ�� Window������ʼ���и����� Activity��
	 * @brief �ڵ�����ģʽ�´����ڶ������Ժ�Ĵ��ڶ���ʧ�ܡ�
	 * @param activity: ������ Activity��
	 * @return �����Ƿ�ɹ���
	*/
	virtual bool emplaceWindow(std::unique_ptr<Activity>&& activity) = 0;

public:
	/**
	 * @brief ��ʾһ����Ϣ�򡣸� ��Ϣ�� ���� Error ͼ�꣬�Ҳ������ڴ��ڡ�
	 * @param title: ��Ϣ��ı��⡣
	 * @param text: ��Ϣ������ݡ�
	*/
	virtual void showErrorMessageBox(std::string_view title, std::string_view text) const noexcept = 0;

	/**
	 * @brief ����ϵͳ˯�߼��������˴���˵˯�߰����������ر���Ļ������
	*/
	virtual void resetSleepCounter() noexcept = 0;
	/**
	 * @brief ������ֹϵͳ˯�ߣ��˴���˵˯�߰����������ر���Ļ������
	 * @param allowSleep: �Ƿ�����˯�ߣ���Ϊ false �� ��ֹ˯�� �� ������Ļ������
	*/
	virtual void setSleepEnabled(bool allowSleep) noexcept = 0;

protected:
	/**
	 * @brief �Ƴ� �ȴ��ر� �Ĵ��ڡ�
	*/
	void removeStoppedWindows() noexcept;
	/**
	 * @brief ϵͳѭ���е� Idle �ص���
	*/
	void onIdle();
	/**
	 * @brief ϵͳѭ���е� Idle �ص���������ģʽ����
	*/
	void onIdleSingle();
	/**
	 * @brief ϵͳѭ��֪ͨ�ص���
	 * @param enter: ture Ϊ ���룬false Ϊ�˳���
	*/
	void onSystemLoop(bool enter);
	/**
	 * @brief ϵͳѭ��֪ͨ�ص���������ģʽ����
	 * @param enter: ture Ϊ ���룬false Ϊ�˳���
	*/
	void onSystemLoopSingle(bool enter);

	/**
	 * @brief ��Ϣѭ����
	*/
	virtual void systemMessagePump() const noexcept = 0;

protected:
	bool m_mutipleWindows; // ���öര��ģʽ�ı�ǡ���ֻ�ڹ���ʱ�޸ģ���
	sf::Clock m_clk; // ʱ�ӡ�
	std::unique_ptr<Window> m_singleWnd; // ������ģʽʹ�õġ�
	std::list<std::unique_ptr<Window>> m_wnds; // �ര��ģʽʹ�õġ�

	static std::unique_ptr<Carnival> s_instance; // Ψһʵ����
}; // class ICarnival

} // namespace GUI
