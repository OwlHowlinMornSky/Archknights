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
#pragma once

#include <MysteryEngine/Core/MEAPI.h>

namespace ME::G3dGlobal {

/**
 * @brief 加载G3D环境。包括初始化GLEW和创建Context。
*/
void ME_API Setup();

/**
 * @brief 激活Context或取消。
*/
bool ME_API SetActive(bool active);

/**
 * @brief 卸载G3D环境。包括销毁Context。
*/
void ME_API Drop() noexcept;

} // namespace ME::G3dGlobal
