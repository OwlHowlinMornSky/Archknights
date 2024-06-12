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

namespace Game {

struct Ability {
	using ValueType = int;

	ValueType original;
	ValueType effective;

	Ability() :
		original(0),
		effective(0) {}

	Ability(ValueType _org) :
		original(_org),
		effective(0) {}

	void SetOriginal(ValueType val) {
		effective += val - original;
		original = val;
	}

	bool IsAbled() const {
		return effective > 0;
	}
	
	ValueType GetValue() const {
		return effective;
	}
};

}
