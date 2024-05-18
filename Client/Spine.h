#pragma once

#include <spine/spine.h>
//#include <spine/spine-sfml.h>
#include <SFML/Graphics.hpp>

#include <list>
#include <stack>

#include <MysteryEngine/G3D/IModel.h>
#include <MysteryEngine/G3D/Vertex.h>

#define spine_to3d_scale_i (128.0f)
#define spine_global_scale (0.7125f)

#define outline_thickness (0.02f)

namespace ohms {

/**
 * @brief 完备的造型数据, 由所有骨架共享
 * @brief Setup pose data, shared by all skeletons
 */
struct SpinePose {
	spine::Atlas* atlas;
	spine::SkeletonData* skeletonData;
	spine::AnimationStateData* animationStateData;

	SpinePose();
	~SpinePose();
};

/**
 * @brief 骨架实体 及其 动画状态
 * @brief skeleton entity and its animation states
 */
class SpineEntity final :
	public sf::Drawable,
	public ME::IModel {
	friend class SpineEntitySet;
	friend class SpineManager;
	typedef ME::IModel Parent;
public:
	SpineEntity(const ohms::SpinePose* pose);
	~SpineEntity();

public:
	virtual void Update(float dt) override;
	virtual void Draw(ME::Camera& camera, ME::Shader& shader) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

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

public:
	bool outline;

protected:
	void UpdateShader(ME::Shader* shader, ME::Camera* camera);

protected:
	/// <summary>
	/// Vertex Array Object ID.
	/// </summary>
	unsigned int vao;
	/// <summary>
	/// Vertex Buffer Object for Vertices.
	/// </summary>
	unsigned int vertexVBO;
	/// <summary>
	/// Depending on input, the amount of vertices or indices that are needed to be drawn for this object.
	/// </summary>
	GLsizei drawCount;
	const ohms::SpinePose* poseRef;
	spine::Skeleton* skeleton;
	spine::AnimationState* animationState;
	spine::Vector<spine::Bone*>* bonesRef;
	mutable spine::Vector<float> worldVertices;
	mutable spine::SkeletonClipping clipper;
	std::vector<ME::Vertex> vertexArray;
}; // end class SpineEntity

/// <summary>
/// Spine 集合, 是一个 pose 对应的所有 entity 的集合
/// </summary>
class SpineEntitySet final {
public:
	SpineEntitySet(const ohms::SpinePose* pose);
	~SpineEntitySet();

public:
	SpineEntity* runOneEntity();

protected:
	const ohms::SpinePose* poseRef;
}; // end class SpineEntitySet


class SFMLTextureLoader :
	public spine::TextureLoader {
public:
	virtual void load(spine::AtlasPage& page, const spine::String& path);

	virtual void unload(void* texture);
};

/// <summary>
/// Spine 管理器
/// </summary>
class SpineManager final {
public:
	SpineManager();
	~SpineManager();

public:
	ohms::SpineEntitySet* addPose(const std::string& name, unsigned char type);
	//ohms::SpineEntitySet* getSetByName(const std::string& name);
	//const ohms::SpinePose* getPoseByName(const std::string& name);

private:
	bool addPoseBinary(const std::string& binaryPath, const std::string& atlasPath);
private:
	//ME::Shader* shaderRef;
	//ME::Camera* cameraRef;
	//float timeScale;
	std::vector<ME::Vertex> vertexArray;
	mutable spine::Vector<float> tempUvs;
	mutable spine::Vector<spine::Color> tempColors;
	mutable bool usePremultipliedAlpha;
	SFMLTextureLoader texLoader;
	std::list<ohms::SpinePose*> poses;
	std::list<ohms::SpineEntitySet*> sets;
}; // end class SpineManager
} // end namespace ohms
