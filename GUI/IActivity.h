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

#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>
#include "ICarnival.h"

namespace GUI {

/**
 * @brief �ӿ� Activity: �� Carnival ��ִ�еĵ������档
*/
class IActivity {
public:
	IActivity() noexcept = default;
	virtual ~IActivity() noexcept = default;

public:
	/**
	 * @brief Activity ������������ǰ�Ĵ���
	 * @param carnival: ������ Carnival �����á�
	*/
	virtual void start(GUI::ICarnival& carnival) = 0;
	/**
	 * @brief Activity �������Ƴ�ǰ�Ĵ���
	*/
	virtual void stop() noexcept = 0;
	/**
	 * @brief Activity ��ͣʱ�Ĵ���
	*/
	virtual void pause() noexcept = 0;
	/**
	 * @brief Activity ȡ����ͣʱ�Ĵ���
	*/
	virtual void resume() noexcept = 0;
	/**
	 * @brief ��ȡ�� Activity ��Ψһ��ʶ����
	 * @return Ψһ��ʶ����
	*/
	virtual uint32_t getID() noexcept = 0;

public:
	/**
	 * @brief �� Acitivity �Ƿ�Ҫ�������С�
	 * @return True �����, ����Ƕ�����
	*/
	virtual bool isIndependent() const noexcept = 0;
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
	 * @param deltaTime ������ʱ�䡣
	*/
	virtual void update(sf::Time deltaTime) = 0;

	virtual void onEnterSysloop() noexcept {}
	virtual void onExitSysloop() noexcept {}
}; // class IActivity

} // namespace GUI
