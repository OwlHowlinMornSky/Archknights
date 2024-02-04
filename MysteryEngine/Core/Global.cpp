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
#include <MysteryEngine/Core/Global.h>

#include <MysteryEngine/Client/GlobalBGM.h>
#include <MysteryEngine/G3D/G3dGlobal.h>

namespace ME::EngineGlobal {

void ME_API setup(bool multiWnd) {
	ME::Carnival::setup(multiWnd);
	ME::GlobalBGM::setup();
	ME::G3dGlobal::setup();
}

void ME_API drop() {
	ME::G3dGlobal::drop();
	ME::GlobalBGM::drop();
	ME::Carnival::drop();
}

}
