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

#include "Window.h"

namespace GUI {

/**
 * @brief Activity: �� Window ��ִ�еĵ������档
*/
class Activity {
	friend class Window;
public:
	/**
	 * @brief Ĭ�Ϲ��캯����
	*/
	Activity() = default;
	/**
	 * @brief Ĭ������������
	*/
	virtual ~Activity() = default;

protected:
	/**
	 * @brief Activity ������������ǰ�Ĵ���
	 * @param wnd: ������ Window �����á�
	*/
	virtual bool start(GUI::Window& wnd) noexcept = 0;
	/**
	 * @brief Activity �������Ƴ�ǰ�Ĵ���
	*/
	virtual void stop() noexcept = 0;

public:
	/**
	 * @brief �����¼���
	 * @param evt: SFML ���¼���
	 * @return ���� false ��ʾһ������������ true ��ʾ���������Ϣ���С�
	*/
	virtual bool handleEvent(const sf::Event& evt) = 0;
	/**
	 * @brief ���£��������ƣ���
	 * @param dtime: ������ʱ�䡣
	*/
	virtual void update(sf::Time dtime) = 0;

	/**
	 * @brief ����ϵͳѭ��ʱ��֪ͨ��
	*/
	virtual void OnEnterSysloop() noexcept;
	/**
	 * @brief �˳�ϵͳѭ��ʱ��֪ͨ��
	*/
	virtual void OnExitSysloop() noexcept;
}; // class IActivity

} // namespace GUI
