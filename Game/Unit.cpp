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
#include "Unit.h"

namespace game {

Unit::Unit() :
	m_productID(0) {
	m_updatable = true;
}

Unit::~Unit() {}

void Unit::UnitOnCreated(UnitFactory& factory, size_t productID) {
	m_factory(factory);
	m_productID = productID;
}

void Unit::OnKicking() {
	m_factory->ReturnUnit(this);
	return Parent::OnKicking();
}

} // namespace game
