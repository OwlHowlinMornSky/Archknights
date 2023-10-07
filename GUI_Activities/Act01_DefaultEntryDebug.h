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

#ifdef _DEBUG

#include "../GUI/ActivityDependent.h"

namespace Activity {

class DefaultEntryDebug final :
	public GUI::ActivityDependent {
public:
	DefaultEntryDebug() noexcept;
	virtual ~DefaultEntryDebug() noexcept override;

public:
	virtual void start(GUI::ICarnival& carnival) override;
	virtual void stop() noexcept override;
	virtual void pause() noexcept override;
	virtual void resume() noexcept override;
	virtual uint32_t getID() noexcept override;

public:
	virtual void handleEvent(const sf::Event& evt) override;
	virtual void update(sf::Time deltaTime) override;

protected:
	GUI::ICarnival* ref_carnival;
	bool m_disableClose;
	bool m_disableResize;
	bool m_disableMinimize;
};

} // namespace Activity

#endif
