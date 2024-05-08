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
 * @brief 接口 Transform 平移：用以可平移的可变换物体。
*/
class ME_API ITransformT {
protected:
	ITransformT() :
		m_position(0.0f, 0.0f, 0.0f),
		m_positionChanged(true) {}
	ITransformT(const ITransformT&) = default;
	ITransformT(ITransformT&&) = default;
	~ITransformT() = default;

public:
	/**
	 * @brief 设置位置
	*/
	void setPosition(const glm::vec3& p) {
		m_position = p;
		m_positionChanged = true;
	}
	void setPosition(glm::vec3&& p) {
		m_position = p;
		m_positionChanged = true;
	}
	void setPosition(float px, float py, float pz) {
		m_position = { px, py, pz };
		m_positionChanged = true;
	}

	/**
	 * @brief 设置x坐标
	*/
	void setPosX(float px) {
		m_position.x = px;
		m_positionChanged = true;
	}
	/**
	 * @brief 设置y坐标
	*/
	void setPosY(float py) {
		m_position.y = py;
		m_positionChanged = true;
	}
	/**
	 * @brief 设置z坐标
	*/
	void setPosZ(float pz) {
		m_position.z = pz;
		m_positionChanged = true;
	}

	/**
	 * @brief 设置xy坐标
	*/
	void setPosXY(float px, float py) {
		m_position.x = px;
		m_position.y = py;
		m_positionChanged = true;
	}
	/**
	 * @brief 设置xz坐标
	*/
	void setPosXZ(float px, float pz) {
		m_position.x = px;
		m_position.z = pz;
		m_positionChanged = true;
	}
	/**
	 * @brief 设置yz坐标
	*/
	void setPosYZ(float py, float pz) {
		m_position.y = py;
		m_position.z = pz;
		m_positionChanged = true;
	}

	/**
	 * @brief 移动
	*/
	void translate(float dpx, float dpy, float dpz) {
		m_position.x += dpx;
		m_position.y += dpy;
		m_position.z += dpz;
		m_positionChanged = true;
	}

protected:
	mutable bool m_positionChanged; // 标记位置是否修改过（使用后请置0）
	mutable glm::vec3 m_position; // 位置
}; // class ITransformT

} // namespace ME
