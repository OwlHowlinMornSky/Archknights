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
* @Description
*     GlobalAttribute.h : Defines global attributes.
*/
#pragma once

#define SIMILARITY_LEVEL 2

#if SIMILARITY_LEVEL == 0
#define USE_CLEAR
#elif SIMILARITY_LEVEL == 1
#define USE_FRAMES
#elif SIMILARITY_LEVEL == 2
#define USE_SPINE
#else
#error "Invalid Risk Level".
#endif
