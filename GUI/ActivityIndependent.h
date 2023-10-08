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

#include "IActivity.h"

namespace GUI {

/**
 * @brief ���� Activity��
*/
class ActivityIndependent : public IActivity {
public:
	ActivityIndependent() = default;
	virtual ~ActivityIndependent() override = default;

public:
	// ��ֹ�޸ġ�
	virtual bool isIndependent() const noexcept override final;
	// ��������ʵ�֡�
	virtual void runIndependently() override = 0;

	// ������ֹʹ�á�
	virtual void handleEvent(const sf::Event& evt) override final;
	// ������ֹʹ�á�
	virtual void update(sf::RenderWindow& window, sf::Time deltaTime) override final;
};

} // namespace GUI
