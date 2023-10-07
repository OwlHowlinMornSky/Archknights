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
#include "Act01_DefaultEntry.h"

#include "../G3D/base.h"
#include "ActivityIDs.h"

namespace Activity {

void DefaultEntry::start(GUI::ICarnival& carnival) {
	ref_carnival = &carnival;
	ref_carnival->enableClose(false);

	g3d::base::setup();
	g3d::base::setActive(false);
	return;
}

void DefaultEntry::stop() noexcept {
	g3d::base::drop();
	ref_carnival = nullptr;
	return;
}

void DefaultEntry::pause() noexcept {
	ref_carnival->enableClose(true);
}

void DefaultEntry::resume() noexcept {
	ref_carnival->enableClose(false);
}

uint32_t DefaultEntry::getID() noexcept {
	return ID_DefaultEntry;
}

void DefaultEntry::runIndependently() {
	if (m_haveRunned) {
		ref_carnival->setTransition(GUI::Transition::Exit);
		return;
	}
	m_haveRunned = true;
	ref_carnival->setTransition(-GUI::Transition::Push, IDs::ID_Opening);
	return;
}

} // namespace Activity
