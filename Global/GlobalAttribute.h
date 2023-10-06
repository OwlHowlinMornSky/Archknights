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
*
* @Description
*    GlobalAttribute.h : Defines global attributes.
*/
#pragma once

#define SIMILARITY_LEVEL (2)

#if SIMILARITY_LEVEL == 0

#define USE_MINE
#define USE_ONE_PICTURE

#elif SIMILARITY_LEVEL == 1

#define USE_LIKE
#define USE_FRAME_ANIMATION

#elif SIMILARITY_LEVEL == 2

#define USE_SAME
#define USE_SPINE_RUNTIMES

#else

#error "Invalid Similarity Level".

#endif
