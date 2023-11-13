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
#pragma once

#include "IFontMgr.h"
#include <memory>

namespace GUI {

class FontMgr final :
	public IFontMgr {
public:
	FontMgr() noexcept = default;
	virtual ~FontMgr() override = default;

public:
	virtual void load() noexcept override;
	virtual const sf::Font& getFont(GUI::FontType type) const noexcept override;

protected:
	using up_font = std::unique_ptr<sf::Font>;
	sf::Font m_commonContext;
	up_font m_monoNumber;
	up_font m_levelTitle;
};

} // namespace GUI
