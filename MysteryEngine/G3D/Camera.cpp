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
#include <MysteryEngine/G3D/Camera.h>

#include <glm/gtx/transform.hpp>

namespace ME {

Camera::Camera() :
	m_type(Type::COUNT),
	m_matPVChanged(false),
	m_matP_needUpdate(false),
	m_data{ 0 },
	m_matP(),
	m_matV(),
	m_matPV() {}

void Camera::setZFar(float z) {
	m_data.zFar = z;
	m_matP_needUpdate = true;
	return;
}

float Camera::getZFar() const {
	return m_data.zFar;
}

void Camera::setZNear(float z) {
	m_data.zNear = z;
	m_matP_needUpdate = true;
	return;
}

float Camera::getZNear() const {
	return m_data.zNear;
}

glm::mat4& Camera::getMatP() {
	if (m_matP_needUpdate)
		updateMatP();
	return m_matP;
}

glm::mat4& Camera::getMatV() {
	if (m_positionChanged || m_rotationChanged)
		updateMatV();
	return m_matV;
}

glm::mat4& Camera::getMatPV() {
	ensureMatPVUpdated();
	return m_matPV;
}

void Camera::setType(Camera::Type type) {
	switch (type) {
	case Type::Orthographic:
		m_data.ortho.dimX = 8.0f;
		m_data.ortho.dimY = 3.0f;
		break;
	case Type::Perspective:
		m_data.persp.fov = 45.0f;
		m_data.persp.aspectRatio = 1.0f;
		break;
	case Type::Oblique:
		m_data.obliq.dimX = 8.0f;
		m_data.obliq.dimY = 6.0f;
		m_data.obliq.sheerX = 0.70710678118654752440f;
		m_data.obliq.sheerY = 0.70710678118654752440f;
		break;
	default:
		return;
	}
	m_data.zFar = 128.0f;
	m_data.zNear = 0.25f;
	m_type = type;
	m_matP_needUpdate = true;
	return;
}

Camera::Type Camera::getType() const {
	return m_type;
}

void Camera::setFOV(float degree) {
	if (degree < 1.0f) degree = 1.0f;
	else if (degree > 179.0f) degree = 179.0f;
	m_data.persp.fov = degree;
	m_matP_needUpdate = true;
	return;
}

float Camera::getFOV() const {
	return m_data.persp.fov;
}

void Camera::setAspectRatio(float ratio) {
	m_data.persp.aspectRatio = ratio;
	m_matP_needUpdate = true;
}

float Camera::getAspectRatio() const {
	return m_data.persp.aspectRatio;
}

void Camera::setDim(float x, float y) {
	m_data.ortho.dimX = x;
	m_data.ortho.dimY = y;
	m_matP_needUpdate = true;
}

float Camera::getDimX() const {
	return m_data.ortho.dimX;
}

float Camera::getDimY() const {
	return m_data.ortho.dimY;
}

void Camera::setSheer(float a, float b) {
	m_data.obliq.sheerX = a;
	m_data.obliq.sheerY = b;
	m_matP_needUpdate = true;
}

float Camera::getSheerX() const {
	return m_data.obliq.sheerX;
}

float Camera::getSheerY() const {
	return m_data.obliq.sheerY;
}

glm::vec3 Camera::testPointFromNdcToWorld(glm::vec4 ndc) {
	glm::vec4 res = glm::inverse(getMatPV()) * ndc;
	return glm::vec3(res.x / res.w, res.y / res.w, res.z / res.w);
}

bool Camera::testDirectionFromNdcToWorld(glm::vec4 ndc, glm::vec3& d, glm::vec3& p) {
	switch (getType()) {
	case Camera::Type::Orthographic:
	{
		glm::mat4 iv = glm::inverse(getMatV());
		glm::vec4 F = iv * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
		glm::vec4 R = iv * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 U = iv * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		glm::vec3 forward = { F.x / F.w, F.y / F.w, F.z / F.w };
		glm::vec3 right = { R.x / R.w, R.y / R.w, R.z / R.w };
		glm::vec3 up = { U.x / U.w, U.y / U.w, U.z / U.w };
		d = forward;
		p = getPosition();
		p += right * ndc.x * m_data.ortho.dimX / 2.0f;
		p += up * ndc.y * m_data.ortho.dimX / 2.0f;
		break;
	}
	case Camera::Type::Perspective:
	{
		glm::vec4 res = glm::inverse(getMatPV()) * ndc;
		glm::vec3 ret{ res.x / res.w, res.y / res.w, res.z / res.w };
		ret -= getPosition();
		d = ret;
		p = getPosition();
		break;
	}
	case Camera::Type::Oblique:
	{
		glm::vec4 res = glm::inverse(getMatP()) * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		glm::vec3 ret{ res.x / res.w, res.y / res.w, res.z / res.w };
		ret -= getPosition();
		d = ret;

		glm::mat4 iv = glm::inverse(getMatV());
		glm::vec4 R = iv * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 U = iv * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		glm::vec3 right = { R.x / R.w, R.y / R.w, R.z / R.w };
		glm::vec3 up = { U.x / U.w, U.y / U.w, U.z / U.w };
		p = getPosition();
		p += right * ndc.x * m_data.ortho.dimX / 2.0f;
		p += up * ndc.y * m_data.ortho.dimX / 2.0f;
		break;
	}
	default:
		return false;
	}
	return true;
}

bool Camera::testDirectionFromNdcToCamera(glm::vec4 ndc, glm::vec3& d, glm::vec3& p) {
	switch (getType()) {
	case Camera::Type::Orthographic:
	{
		d = glm::vec3(0.0f, 1.0f, 0.0f);
		p = glm::vec3(ndc.x * m_data.ortho.dimX / 2.0f, ndc.y * m_data.ortho.dimX / 2.0f, 0.0f);
		break;
	}
	case Camera::Type::Perspective:
	{
		glm::vec4 res = glm::inverse(getMatP()) * ndc;
		glm::vec3 ret{ res.x / res.w, res.y / res.w, res.z / res.w };
		d = glm::vec3(ret.x, -ret.z, ret.y);
		p = glm::vec3(0.0f);
		break;
	}
	case Camera::Type::Oblique:
	{
		d = glm::vec3(-m_data.obliq.sheerX, 1.0f, -m_data.obliq.sheerY);
		p = glm::vec3(ndc.x * m_data.obliq.dimX / 2.0f, ndc.y * m_data.obliq.dimX / 2.0f, 0.0f);
		break;
	}
	default:
		return false;
	}
	return true;
}

void Camera::ensureMatPVUpdated() {
	if (m_matP_needUpdate)
		updateMatP();
	if (m_positionChanged || m_rotationChanged)
		updateMatV();
	if (m_matPVChanged) {
		m_matPV = m_matP * m_matV;
		m_matPVChanged = false;
	}
	return;
}

void Camera::updateMatV() {
	// 限制俯仰角
	if (m_rotation.x < 0.0f)
		m_rotation.x = 0.0f;
	else if (m_rotation.x > 180.0f)
		m_rotation.x = 180.0f;
	// 计算旋转
	m_matV = glm::rotate(glm::radians(-m_rotation.y), glm::vec3(0.0f, 0.0f, 1.0f));
	m_matV *= glm::rotate(glm::radians(-m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_matV *= glm::rotate(glm::radians(-m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	// 计算位移
	m_matV *= glm::translate(-m_position);
	// 标记
	m_positionChanged = false;
	m_rotationChanged = false;
	m_matPVChanged = true;
	return;
}

void Camera::updateMatP() {
	// 计算矩阵
	switch (m_type) {
	case Type::Orthographic:
	{
		// 计算半宽和半高
		float x = m_data.ortho.dimX * 0.5f;
		float y = m_data.ortho.dimY * 0.5f;
		// 计算矩阵
		m_matP = glm::ortho(-x, x, -y, y, m_data.zNear, m_data.zFar);
		break;
	}
	case Type::Perspective:
	{
		m_matP = glm::perspective(
			glm::radians(m_data.persp.fov),
			m_data.persp.aspectRatio,
			m_data.zNear,
			m_data.zFar
		);
		break;
	}
	case Type::Oblique:
	{
		// 计算半宽和半高
		float x = m_data.obliq.dimX * 0.5f;
		float y = m_data.obliq.dimY * 0.5f;
		// 计算矩阵
		m_matP = glm::identity<glm::mat4>();
		m_matP[2][0] = -m_data.obliq.sheerX;
		m_matP[2][1] = -m_data.obliq.sheerY;
		m_matP = glm::ortho(-x, x, -y, y, m_data.zNear, m_data.zFar) * m_matP;
		break;
	}
	}
	// 标记
	m_matP_needUpdate = false;
	m_matPVChanged = true;
	return;
}

} // namespace ME
