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

#include "ITransformT.h"
#include "ITransformR.h"

namespace g3d {

/**
 * @brief 接口 Camera。
*/
class ICamera :
	public ITransformT,
	public ITransformR {
public:
	ICamera() = default;
	virtual ~ICamera() = default;

	/**
	 * @brief 获取近场临界距离。
	 * @return 近场临界距离。
	*/
	float getFF() {
		return m_ff;
	}

	/**
	 * @brief 获取远场临界距离。
	 * @return 远场临界距离。
	*/
	float getFN() {
		return m_fn;
	}

protected:
	float m_fn; // 近场临界距离。
	float m_ff; // 远场临界距离。
};

} // namespace g3d
