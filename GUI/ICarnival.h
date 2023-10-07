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

#include <string>
#include <SFML/Graphics/RenderWindow.hpp>

namespace GUI {

/**
 * @brief �ӿ�Carnival: �� RenderWindow ������ Activity �Ĺ����ࡣ
*/
class ICarnival {
public:
	/**
	 * @brief ��Ϣ��Ķ�����Ϣ��
	*/
	enum class MBInfo {
		None = 0,
		Info,
		Error
	};

	/**
	 * @brief ������ջ�����ͣ�����Ϊ Stop������Ϊ Pause��0 ��Ч��
	*/
	enum Transition : int {
		Switch = 1, // 1 ��Ŀ�ꡣ��ǰ Activity �ᱻ������Ŀ�꽫��ѹ��ջ�����С�
		Push, // 1 ��Ŀ�ꡣ�佫��ѹ��ջ�в����С�
		Pop, // 0 ��Ŀ�ꡣ��ǰ Activity �ᱻ�������µ�ջ�������У���ջ�����˳���
		PopTo, // 1 ��Ŀ�ꡣ���Ŀ����ջ�У���ջ�ᱻ����Ŀ�꣬Ŀ����ջ���ҽ����У�������Ч����
		PopPush, // 2 ��Ŀ�ꡣ���Ŀ��0��ջ�У���ջ�ᱻ����֮��Ŀ��1��ѹ���ҽ����У������� Push ��ͬ��
		Exit        // 0 ��Ŀ�ꡣǿ���˳���
	};

public:
	ICarnival() noexcept = default;
	virtual ~ICarnival() = default;

public:
	/**
	 * @brief ��ʼִ�С�
	*/
	virtual void run() noexcept = 0;

	/**
	 * @brief ȡ�ø� Carnival ����� RenderWindow��
	*/
	virtual sf::RenderWindow& getRenderWindow() noexcept = 0;

	/**
	 * @brief ���Ƕ��� Activity �õ��˳����еķ�����
	*/
	virtual void cancelKeepRunning() noexcept = 0;
	/**
	 * @brief �� Activity �õ����ñ�Ǩ�ķ�����
	 * @param t: ��Ǩ���͡�
	*/
	virtual void setTransition(int t, uint32_t a0 = 0, uint32_t a1 = 0) noexcept = 0;

	/**
	 * @brief �����Ƿ�� sizing ��Ϣ���� resize ��Ϣ�� Activity ����
	 * @param enabled: �Ƿ�����
	*/
	virtual void setFullResizeMessage(bool enabled) noexcept = 0;

	/**
	 * @brief ��ʾһ����Ϣ��
	 * @param title: ��Ϣ��ı��⡣
	 * @param text: ��Ϣ������ݡ�
	 * @param info: ��Ϣ��Ķ�����Ϣ��
	*/
	virtual void showMessageBox(std::string_view title,
								std::string_view text,
								MBInfo info = MBInfo::None) const noexcept = 0;

	/**
	 * @brief ��� �������ڵ� �رհ�ť �Ƿ����á�
	 * @return True �������ã������ѽ��á�
	*/
	virtual bool isEnabledClose() const noexcept = 0;
	/**
	 * @brief ��� �������ڵ� Resize �߿� �� ��󻯰�ť �Ƿ����á�
	 * @return True �������ã������ѽ��á�
	*/
	virtual bool isEnabledResize() const noexcept = 0;
	/**
	 * @brief ��� �������ڵ� ��С����ť �Ƿ����á�
	 * @return True �������ã������ѽ��á�
	*/
	virtual bool isEnabledMinimize() const noexcept = 0;

	/**
	 * @brief ���� �������ڵ� �رհ�ť �Ƿ����á�
	 * @param enabled: True �����ã�������á�
	*/
	virtual void enableClose(bool enabled) const noexcept = 0;
	/**
	 * @brief ���� �������ڵ� Resize �߿� �� ��󻯰�ť �Ƿ����á�
	 * @param enabled: True �����ã�������á�
	*/
	virtual void enableResize(bool enabled) const noexcept = 0;
	/**
	 * @brief ���� �������ڵ� ��С����ť �Ƿ����á�
	 * @param enabled: True �����ã�������á�
	*/
	virtual void enableMinimize(bool enabled) const noexcept = 0;

	/**
	 * @brief �����ޱ߿򴰿ڡ�
	 * @param full: �Ƿ������ޱ߿򴰿ڡ�
	*/
	virtual void setFullwindow(bool full) noexcept = 0;

	/**
	 * @brief ����ȫ����
	 * @param full: �Ƿ�����ȫ����
	*/
	virtual void setFullscreen(bool full) noexcept = 0;

	/**
	 * @brief һ��ϵͳ����Ϣѭ�������������ڼ���ʱ���ⴰ�ڱ��ж�Ϊδ��Ӧ��
	*/
	virtual void systemMessagePump() const noexcept = 0;
}; // class ICarnival

} // namespace GUI
