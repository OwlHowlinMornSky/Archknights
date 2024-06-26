/*
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

#include "IWall.h"
#include <box2d/box2d.h>

#include <vector>

namespace Physics {

class Wall :
	public IWall {
public:
	Wall();
	virtual ~Wall();

	static b2Body* GetWallInstance();

	virtual void SetSize(int m, int n) override;
	virtual void AddWallBlock(int posx, int posy) override;

	void Create(b2World* world);

protected:
	b2Body* m_body;
	b2Fixture* m_bound;
	std::vector<b2Fixture*> m_fixtures;
};

}
