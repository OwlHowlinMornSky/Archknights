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

#include <MysteryEngine/G3D/ITransformT.h>
#include <MysteryEngine/G3D/ITransformR.h>

namespace ME {

/**
 * @brief Camera，具体实现有 透视相机、正交相机 和 斜轴相机。
*/
class ME_API Camera final :
	public ITransformR,
	public ITransformT {
public:
	enum class Type : unsigned char {
		Orthographic = 0,
		Perspective,
		Oblique,

		COUNT
	};
	struct CameraData {
		union {
			struct PerspectiveData {
				float fov; // 纵向视场角
				float aspectRatio; // 宽高比
			} persp;
			struct OrthographicData {
				float dimX; // 画面宽度（不是半宽）
				float dimY; // 画面高度（不是半高）
			} ortho;
			struct ObliqueData {
				float dimX; // 画面宽度（不是半宽）
				float dimY; // 画面高度（不是半高）
				float sheerX;
				float sheerY;
			} obliq;
		};
		float zNear; // z of near plane
		float zFar;  // z of far plane
	};

public:
	Camera();
	~Camera() = default;

public:
	void setZFar(float z);
	float getZFar() const;
	void setZNear(float z);
	float getZNear() const;

	// 不使用 const & 的原因：update shader时传参为 非const 指针。
	// 不使用 const 函数的原因：内部调用更新不为 const。

	glm::mat4& getMatP();
	glm::mat4& getMatV();
	glm::mat4& getMatPV();

	void setType(Camera::Type type);
	Camera::Type getType() const;

	void setFOV(float degree);
	float getFOV() const;
	void setAspectRatio(float ratio);
	float getAspectRatio() const;

	void setDim(float x, float y);
	float getDimX() const;
	float getDimY() const;

	void setSheer(float a, float b);
	float getSheerX() const;
	float getSheerY() const;

	glm::vec3 testPointFromNdcToWorld(glm::vec4 ndc);
	bool testDirectionFromNdcToWorld(glm::vec4 ndc, glm::vec3& d, glm::vec3& p);
	bool testDirectionFromNdcToCamera(glm::vec4 ndc, glm::vec3& d, glm::vec3& p);

protected:
	void ensureMatPVUpdated();
	void updateMatV();
	void updateMatP();

protected:
	Type m_type;
	bool m_matPVChanged;
	bool m_matP_needUpdate;
	CameraData m_data;
	glm::mat4 m_matP;
	glm::mat4 m_matV;
	glm::mat4 m_matPV;
}; // class Camera

} // namespace ME
