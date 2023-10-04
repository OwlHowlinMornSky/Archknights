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

#include "../ActivityDependent.h"

#include <SFML/Graphics.hpp>

class TestActivity final : public GUI::ActivityDependent {
public:
	TestActivity(size_t n);
	virtual ~TestActivity() override;

public:
	virtual void start(GUI::ICarnival& carnival) override;
	virtual void stop() override;
	virtual void pause() override;
	virtual void resume() override;
	virtual size_t getID() override;

public:
	virtual void handleEvent(const sf::Event& evt) override;
	virtual void update(float dt) override;

protected:
	GUI::ICarnival* ref_carnival;
	sf::RectangleShape m_shape;
	size_t m_id;
};