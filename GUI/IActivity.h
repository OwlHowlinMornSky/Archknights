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

#include <SFML/Window/Event.hpp>
#include "ICarnival.h"

namespace GUI {

/**
 * @brief �ӿ� Activity: �� Carnival ��ִ�еĵ������档
*/
class IActivity {
public:
	IActivity() = default;
	virtual ~IActivity() = default;

public:
	/**
	 * @brief Activity ������������ǰ�Ĵ���
	 * @param carnival: ������ Carnival �����á�
	*/
	virtual void start(ICarnival& carnival) = 0;
	/**
	 * @brief Activity �������Ƴ�ǰ�Ĵ���
	*/
	virtual void stop() = 0;
	/**
	 * @brief Activity ��ͣʱ�Ĵ���
	*/
	virtual void pause() = 0;
	/**
	 * @brief Activity ȡ����ͣʱ�Ĵ���
	*/
	virtual void resume() = 0;
	/**
	 * @brief ��ȡ�� Activity ��Ψһ��ʶ����
	 * @return Ψһ��ʶ����
	*/
	virtual size_t getID() = 0;

public:
	/**
	 * @brief �� Acitivity �Ƿ�Ҫ�������С�
	 * @return True �����, ����Ƕ�����
	*/
	virtual bool isIndependent() const = 0;
	/**
	 * @brief ���������С�
	*/
	virtual void runIndependently() = 0;

	/**
	 * @brief �����¼���
	 * @param evt: SFML ���¼���
	*/
	virtual void handleEvent(const sf::Event& evt) = 0;
	/**
	 * @brief ���¡�
	 * @param dt ������ʱ�䡣
	*/
	virtual void update(float dt) = 0;
}; // class IActivity

} // namespace GUI
