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
#include "base.h"

#include <memory>
#include <string>

#include <GL/glew.h>
#include <SFML/Window/Context.hpp>

namespace {

std::unique_ptr<sf::Context> g_context;

} // namespace

namespace g3d::base {

void setup() {
	g_context = std::make_unique<sf::Context>();
	GLenum glew_err = glewInit();
	if (glew_err != GLEW_OK) {
		throw std::exception((std::string("Error initializing GLEW, error: ") + (const char*)glewGetErrorString(glew_err)).c_str());
	}
	return;
}

bool setActive(bool active) {
	return g_context->setActive(active);
}

void drop() noexcept {
	return g_context.reset();
}

} // namespace g3d::base
