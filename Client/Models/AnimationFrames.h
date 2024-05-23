/*
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
#pragma once

#ifndef ARCHKNIGHTS_LIMITED

#include <MysteryEngine/G3D/Vertex.h>
#include <string_view>
#include <array>

#include "IAnimation.h"
#include "ActorGroup.h"

namespace Game {

class AnimationFrames final :
	public ME::IModel {
public:
	int Load(std::string_view file);

public:
	virtual void Update(float dt) override;
	virtual void Draw(ME::Camera& camera, ME::Shader& shader) override;

protected:
	float m_test;
	std::array<ME::Vertex, 4> vertexArray;
	unsigned int vao;
	unsigned int vertexVBO;
	unsigned int drawCount;
};

class FramesPose final :
	public IAnimationPose {
public:
	virtual std::shared_ptr<ME::IModel> CreateAnimation() override;
};

class FramesFactory final :
	public IAnimationFactory {
public:
	virtual bool CreatePose(
		std::unique_ptr<IAnimationPose>& ptr,
		std::string_view name,
		unsigned char type
	) override;

	virtual char CreatePose2(
		std::unique_ptr<IAnimationPose>& ptr0,
		std::unique_ptr<IAnimationPose>& ptr1,
		std::string_view name
	) override;
};

}

#endif // !ARCHKNIGHTS_LIMITED
