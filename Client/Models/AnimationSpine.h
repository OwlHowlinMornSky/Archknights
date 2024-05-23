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

#include <spine/Atlas.h>
#include <spine/Skeleton.h>
#include <spine/AnimationState.h>
#include <spine/SkeletonClipping.h>

#include <SFML/Graphics.hpp>

#include <list>
#include <stack>

#include <MysteryEngine/G3D/IModel.h>
#include <MysteryEngine/G3D/Vertex.h>

#include "ActorGroup.h"
#include "IAnimationSpine.h"


namespace ohms {


struct SpinePoseData {
	spine::Atlas* atlas;
	spine::SkeletonData* skeletonData;
	spine::AnimationStateData* animationStateData;
};


class SpineAnimation :
	public ISpineAnimation {
	typedef ME::IModel Parent;
public:
	SpineAnimation(const ohms::SpinePoseData _pose);
	~SpineAnimation();

public:
	virtual void Update(float dt) override;
	virtual void Draw(ME::Camera& camera, ME::Shader& shader) override;
	virtual void SetOutline(bool enabled) override;

	spine::TrackEntry* setAnimation(size_t trackIndex, const std::string& animationName, bool loop);
	spine::TrackEntry* setAnimation(size_t trackIndex, spine::Animation* animation, bool loop);

	spine::TrackEntry* setEmptyAnimation(size_t trackIndex, float mixDuration);
	void setEmptyAnimations(float mixDuration);

	spine::TrackEntry* addAnimation(size_t trackIndex, const std::string& animationName, bool loop, float delay);
	spine::TrackEntry* addAnimation(size_t trackIndex, spine::Animation* animation, bool loop, float delay);
	spine::TrackEntry* addEmptyAnimation(size_t trackIndex, bool loop, float delay);

	spine::Animation* findAnimation(const std::string& animationName) const;

	glm::vec2 getBonePosition(const std::string& boneName) const;
	glm::vec2 getBonePositionByIndex(int boneIndex) const;
	int getBoneIndex(const std::string& boneName) const;

protected:
	void UpdateShader(ME::Shader& shader, ME::Camera& camera);

	void DrawVertices(ME::Shader& shader, sf::Texture* texture);

protected:
	bool m_outline;
	unsigned int m_vao;
	unsigned int m_vertexVBO;
	spine::Skeleton* m_skeleton;
	spine::AnimationState* m_animationState;
	spine::Vector<spine::Bone*>* m_bonesRef;
	const ohms::SpinePoseData m_pose;
	std::vector<Game::ActorVertex> vertexArray;
	mutable spine::Vector<float> worldVertices;
	mutable spine::SkeletonClipping m_clipper;
};


class SpinePose final :
	public ISpinePose {
public:
	SpinePose(ohms::SpinePoseData _pose);
	~SpinePose();

public:
	virtual std::shared_ptr<ISpineAnimation> CreateAnimation() override;

protected:
	ohms::SpinePoseData m_pose;
};


class SpineFactory final :
	public ISpineFactory {
public:
	SpineFactory();
	~SpineFactory();

public:
	virtual bool CreatePose(std::unique_ptr<ISpinePose>& ptr, std::string_view name, unsigned char type) override;
	virtual char CreatePose2(
		std::unique_ptr<ISpinePose>& ptr0,
		std::unique_ptr<ISpinePose>& ptr1,
		std::string_view name
	) override;

private:
	ohms::SpinePose* createPoseBinary(std::string_view binaryPath, std::string_view atlasPath);
};


}
