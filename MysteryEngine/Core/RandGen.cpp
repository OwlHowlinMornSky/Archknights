/*
*    Mystery Engine
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
#include <MysteryEngine/Core/RandGen.h>

namespace {

std::random_device rd;
std::uniform_real_distribution<float> u1(0.0f, 1.0f);

} // namespace

namespace ME::RandGen {

std::mt19937 gen(rd());

void reseed() {
	return gen.seed(rd());;
}

unsigned int get() {
	return gen();
}

float getUni01() {
	return u1(gen);
}

} // namespace ME::RandGen
