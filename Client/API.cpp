﻿/*
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
#include "API.h"
#include "Activities/Act01_DefaultEntrance.h"

std::unique_ptr<ME::Activity> GetClientEntrance() {
    return std::make_unique<Activity::Act01_DefaultEntrance>();
}

#include <gl/glew.h>
#include <MysteryEngine/G3D/G3dGlobal.h>

void ClientInitGlew() {
	ME::G3dGlobal::SetActive(true);
	glewExperimental = GL_TRUE;
	GLenum glew_err = glewInit(); // 初始化GLEW
	if (glew_err != GLEW_OK) {
		throw 1;
	}
	ME::G3dGlobal::SetActive(false);
}
