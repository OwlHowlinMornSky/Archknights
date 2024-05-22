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
	virtual bool CreatePose(std::unique_ptr<ISpinePose>& ptr, const std::string& name, unsigned char type) = 0;

public:
	static std::unique_ptr<ISpineFactory> Create();
};

}
