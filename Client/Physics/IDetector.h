﻿/*
*    Archknights
*
*    Copyright (C) 2023-2024  Tyler Parret True
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

#include <map>
#include "IFixture.h"

namespace Physics {

struct Rows {
	int32_t offset;
	uint32_t length;
	uint32_t* widths;
};

struct MapValue {
	std::weak_ptr<Game::Entity> entity;
	size_t count;

	MapValue() :
		count(0) {}
};


class IDetector :
	public IFixture {
protected:
	IDetector() = default;
public:
	virtual ~IDetector() = default;

public:
	virtual void setPosition(float x, float y) = 0;
	virtual void setSendSensorMsg(bool enabled) = 0;

	virtual std::map<Game::EntityIdType, MapValue>::iterator listBegin() = 0;
	virtual std::map<Game::EntityIdType, MapValue>::iterator listEnd() = 0;
};

}
