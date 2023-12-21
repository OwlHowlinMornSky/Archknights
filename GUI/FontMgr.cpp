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
#include "FontMgr.h"

namespace GUI {

void FontMgr::load() noexcept {
	m_commonContext.loadFromFile("assets/fonts/SourceHanSansSC-Regular.otf");

	// TO-DO

	return;
}

const sf::Font& FontMgr::getFont(GUI::FontType type) const noexcept {
	switch (type) {
	case FontType::CommonContext:
		break;
	case FontType::MonoMunber:
		if (m_monoNumber != nullptr)
			return *m_monoNumber;
		break;
	case FontType::LevelTitle:
		if (m_levelTitle != nullptr)
			return *m_levelTitle;
		break;
	default:
		break;
	}
	return m_commonContext;
}

} // namespace GUI
