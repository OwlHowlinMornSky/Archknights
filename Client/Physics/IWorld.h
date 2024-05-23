#pragma once

#include "IDetector.h"
#include "IBody.h"
#include "IWall.h"

#include <memory>

namespace Physics {

class IWorld {

public:
	virtual std::unique_ptr<IBody> CreateBodyCircle() = 0;

	virtual std::unique_ptr<IDetector> CreateDetectorCircle() = 0;
	virtual std::unique_ptr<IDetector> CreateDetectorTiles() = 0;

	virtual std::unique_ptr<IWall> CreateWall() = 0;

};

}
