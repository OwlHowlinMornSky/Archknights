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
 * @brief �Ƕ��� Activity��
*/
class ActivityDependent : public IActivity {
public:
	ActivityDependent() = default;
	virtual ~ActivityDependent() override = default;

public:
	// ��ֹ�޸ġ�
	virtual bool isIndependent() const noexcept override final;
	// �Ƕ�����ֹʹ�á�
	virtual void runIndependently() override final;

	// �Ƕ�������ʵ�֡�
	virtual void handleEvent(const sf::Event& evt) override = 0;
	virtual void update(sf::RenderWindow& window, sf::Time deltaTime) override = 0;
};

} // namespace GUI
