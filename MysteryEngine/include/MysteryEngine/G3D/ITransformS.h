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
#include <MysteryEngine/Core/GLM.h>

namespace ME {

/**
 * @brief 接口 Transform 缩放：用于可缩放的可变换物体。
*/
class ME_API ITransformS {
protected:
	ITransformS() :
		m_scale(1.0f, 1.0f, 1.0f),
		m_scaleChanged(true) {}
	ITransformS(const ITransformS&) = default;
	ITransformS(ITransformS&&) = default;
	~ITransformS() = default;

public:
	/**
	 * @brief 设置缩放
	*/
	void setScale(const glm::vec3& s) {
		m_scale = s;
		m_scaleChanged = true;
	}
	void setScale(glm::vec3&& s) {
		m_scale = s;
		m_scaleChanged = true;
	}
	void setScale(float sx, float sy, float sz) {
		m_scale = { sx, sy, sz };
		m_scaleChanged = true;
	}
	void setScale(float ss) {
		m_scale = { ss, ss, ss };
		m_scaleChanged = true;
	}

	/**
	 * @brief 缩放
	*/
	void scale(float dsx, float dsy, float dsz) {
		m_scale.x *= dsx;
		m_scale.y *= dsy;
		m_scale.z *= dsz;
		m_scaleChanged = true;
	}
	void scale(float ds) {
		m_scale *= ds;
		m_scaleChanged = true;
	}

protected:
	bool m_scaleChanged; // 标记缩放值是否修改过（使用后请置0）
	glm::vec3 m_scale; // 缩放值
}; // class ITransformS

} // namespace ME
