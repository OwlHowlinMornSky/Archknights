#pragma once

#include <spine/spine.h>
//#include <spine/spine-sfml.h>
#include <SFML/Graphics.hpp>

#include <list>
#include <stack>

#include <MysteryEngine/G3D/IModel.h>
#include <MysteryEngine/G3D/Vertex.h>

#include "ActorGroup.h"
#include "ISpine.h"

namespace ohms {

/**
 * @brief 完备的造型数据, 由所有骨架共享
 * @brief Setup pose data, shared by all skeletons
 */
struct SpinePoseData {
	spine::Atlas* atlas;
	spine::SkeletonData* skeletonData;
	spine::AnimationStateData* animationStateData;
};

/**
 * @brief 骨架实体 及其 动画状态
 * @brief skeleton entity and its animation states
 */
class SpineAnimation final :
	public ISpineAnimation {
	typedef ME::IModel Parent;
public:
	SpineAnimation(const ohms::SpinePoseData _pose);
	~SpineAnimation();

public:
	virtual void Update(float dt) override;
	virtual void Draw(ME::Camera& camera, ME::Shader& shader) override;
	virtual void SetOutline(bool enabled) override;

	/// <summary>
	/// 设置动画。
	/// </summary>
	/// <param name="trackIndex">轨道编号</param>
	/// <param name="animationName">动画名称</param>
	/// <param name="loop">是否循环播放</param>
	/// <returns>State for playback of an animation</returns>
	spine::TrackEntry* setAnimation(size_t trackIndex, const std::string& animationName, bool loop);
	/// <summary>
	/// 设置动画。
	/// </summary>
	/// <param name="trackIndex">轨道编号</param>
	/// <param name="animation">动画</param>
	/// <param name="loop">是否循环播放</param>
	/// <returns>State for playback of an animation</returns>
	spine::TrackEntry* setAnimation(size_t trackIndex, spine::Animation* animation, bool loop);
	/// <summary>
	/// 设置空动画（用于结束轨道）。
	/// </summary>
	/// <param name="trackIndex">轨道编号</param>
	/// <param name="mixDuration">混合时间</param>
	/// <returns>State for playback of an animation</returns>
	spine::TrackEntry* setEmptyAnimation(size_t trackIndex, float mixDuration);
	/// <summary>
	/// 在所有轨道设置空动画（用于结束 SPINE 动画）。
	/// </summary>
	/// <param name="mixDuration">混合时间</param>
	void setEmptyAnimations(float mixDuration);
	/// <summary>
	/// 添加动画。
	/// </summary>
	/// <param name="trackIndex">轨道编号</param>
	/// <param name="animationName">动画名称</param>
	/// <param name="loop">是否循环</param>
	/// <param name="delay">延迟时间</param>
	/// <returns>State for playback of an animation</returns>
	spine::TrackEntry* addAnimation(size_t trackIndex, const std::string& animationName, bool loop, float delay);
	/// <summary>
	/// 添加动画。
	/// </summary>
	/// <param name="trackIndex">轨道编号</param>
	/// <param name="animation">动画</param>
	/// <param name="loop">是否循环</param>
	/// <param name="delay">延迟时间</param>
	/// <returns>State for playback of an animation</returns>
	spine::TrackEntry* addAnimation(size_t trackIndex, spine::Animation* animation, bool loop, float delay);
	/// <summary>
	/// 添加空动画。
	/// </summary>
	/// <param name="trackIndex">轨道编号</param>
	/// <param name="loop">是否循环</param>
	/// <param name="delay"></param>
	/// <returns>State for playback of an animation</returns>
	spine::TrackEntry* addEmptyAnimation(size_t trackIndex, bool loop, float delay);
	/// <summary>
	/// 查找动画。
	/// </summary>
	/// <param name="animationName">动画名称</param>
	/// <returns>动画</returns>
	spine::Animation* findAnimation(const std::string& animationName) const;
	/// <summary>
	/// 取骨骼位置。
	/// </summary>
	/// <param name="BoneName">骨骼名称</param>
	/// <returns>骨骼</returns>
	sf::Vector2f getBonePosition(const std::string& boneName) const;
	sf::Vector2f getBonePositionByIndex(int boneIndex) const;
	int getBoneIndex(const std::string& boneName) const;

protected:
	void UpdateShader(ME::Shader& shader, ME::Camera& camera);

	void DrawVertices(ME::Shader& shader, sf::Texture* texture);

protected:
	bool m_outline;
	/// <summary>
	/// Vertex Array Object ID.
	/// </summary>
	unsigned int m_vao;
	/// <summary>
	/// Vertex Buffer Object for Vertices.
	/// </summary>
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
	virtual bool CreatePose(std::unique_ptr<ISpinePose>& ptr, const std::string& name, unsigned char type) override;

private:
	ohms::SpinePose* createPoseBinary(const std::string& binaryPath, const std::string& atlasPath);
};

} // end namespace ohms
