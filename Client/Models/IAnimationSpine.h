#pragma once

#include <MysteryEngine/G3D/IModel.h>
#include <memory>
//#include <spine/AnimationState.h>

namespace ohms {

class ISpineAnimation :
	public ME::IModel {
public:
	ISpineAnimation() = default;
	virtual ~ISpineAnimation() = default;

public:
	virtual void SetOutline(bool enabled) = 0;
};

class ISpinePose {
public:
	ISpinePose() = default;
	virtual ~ISpinePose() = default;

public:
	virtual std::shared_ptr<ISpineAnimation> CreateAnimation() = 0;
};

class ISpineFactory {
public:
	ISpineFactory() = default;
	virtual ~ISpineFactory() = default;

public:
	/**
	 * @brief 
	 * @param ptr 
	 * @param name 角色名称（非文件名称，例如基建动画文件名多一个‘build_’，但传参不应包含）。
	 * @param type 0: 战斗正面, 1: 战斗背面, 2: 基建.
	 * @return true 则加载成功。
	 */
	virtual bool CreatePose(
		std::unique_ptr<ISpinePose>& ptr,
		std::string_view name,
		unsigned char type
	) = 0;
	/**
	 * @brief 
	 * @param ptr0 
	 * @param ptr1 
	 * @param name 
	 * @return 0: 都加载失败, 1: ptr0加载成功, 2: ptr1加载成功, 3: 都加载成功.
	 */
	virtual char CreatePose2(
		std::unique_ptr<ISpinePose>& ptr0,
		std::unique_ptr<ISpinePose>& ptr1,
		std::string_view name
	) = 0;

public:
	static std::unique_ptr<ISpineFactory> Create();
};

}
