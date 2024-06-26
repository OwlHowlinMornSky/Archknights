﻿/*
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
#include <MysteryEngine/Core/GLM.h>

namespace ME {

/**
 * @brief 接口 Transform 原点：用于可修改原点的可变换物体。
*/
class ME_API ITransformO {
protected:
	ITransformO() :
		m_origin(0.0f, 0.0f, 0.0f),
		m_originChanged(true) {}
	ITransformO(const ITransformO&) = default;
	ITransformO(ITransformO&&) = default;
	~ITransformO() = default;

public:
	/**
	 * @brief 设置原点位置
	*/
	void setOrigin(const glm::vec3& o) {
		m_origin = o;
		m_originChanged = true;
	}
	void setOrigin(glm::vec3&& o) {
		m_origin = o;
		m_originChanged = true;
	}
	void setOrigin(float ox, float oy, float oz) {
		m_origin = { ox, oy, oz };
		m_originChanged = true;
	}

protected:
	bool m_originChanged; // 标记原点是否修改过（使用后请置0）
	glm::vec3 m_origin; // 原点
}; // class ITransformO

} // namespace ME
