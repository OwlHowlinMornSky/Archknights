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
		Push      , // 1 ��Ŀ�ꡣ�佫��ѹ��ջ�в����С�
		Pop       , // 0 ��Ŀ�ꡣ��ǰ Activity �ᱻ�������µ�ջ�������У���ջ�����˳���
		PopTo     , // 1 ��Ŀ�ꡣ���Ŀ����ջ�У���ջ�ᱻ����Ŀ�꣬Ŀ����ջ���ҽ����У�������Ч����
		PopPush   , // 2 ��Ŀ�ꡣ���Ŀ��0��ջ�У���ջ�ᱻ����֮��Ŀ��1��ѹ���ҽ����У������� Push ��ͬ��
		Exit        // 0 ��Ŀ�ꡣǿ���˳���
	};

public:
	ICarnival();

	virtual ~ICarnival();

public:
	/**
	 * @brief ��ʼִ�С�
	*/
	virtual void run() = 0;

	/**
	 * @brief ���Ƕ��� Activity �õ��˳����еķ�����
	*/
	virtual void cancelKeepRunning() = 0;

	/**
	 * @brief �� Activity �õ����ñ�Ǩ�ķ�����
	 * @param t ��Ǩ���͡�
	*/
	virtual void setTransition(int t, size_t a0 = 0, size_t a1 = 0) = 0;

	/**
	 * @brief ȡ�ø� Carnival ������ RenderWindow��
	*/
	virtual sf::RenderWindow& getRenderWindow() = 0;

	/**
	 * @brief ��ʾһ����Ϣ��
	 * @param title ��Ϣ��ı��⡣
	 * @param text ��Ϣ������ݡ�
	 * @param info ��Ϣ��Ķ�����Ϣ��
	*/
	virtual void showMessageBox(std::string_view title, std::string_view text, MBInfo info = MBInfo::None) const;

	/**
	 * @brief �������������ڵĹرհ�ť�Ƿ����á�
	*/
	virtual void setCloseButton(bool enabled) const;
}; // class ICarnival

} // namespace GUI