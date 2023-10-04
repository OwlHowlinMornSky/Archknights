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
#include "DefaultEntry.h"

#include "ActivityIDs.h"

#include "../../G3D/base.h"

namespace GUI {

DefaultEntry::DefaultEntry() :
	m_haveRunned(false),
	ref_carnival(nullptr) {}

DefaultEntry::~DefaultEntry() {}

void DefaultEntry::start(ICarnival& carnival) {
	ref_carnival = &carnival;
	g3d::base::setup();
	g3d::base::setActive(false);
	return;
}

void DefaultEntry::stop() {
	g3d::base::drop();
	ref_carnival = nullptr;
	return;
}

void DefaultEntry::pause() {}

void DefaultEntry::resume() {}

size_t DefaultEntry::getID() {
	return GUI::ID_DefaultEntry;
}

void DefaultEntry::runIndependently() {
	if (m_haveRunned) {
		ref_carnival->setTransition(ICarnival::Exit);
		return;
	}
	m_haveRunned = true;
	ref_carnival->setTransition(-ICarnival::Push, GUI::ID_Load);
	return;
}

} // namespace GUI
