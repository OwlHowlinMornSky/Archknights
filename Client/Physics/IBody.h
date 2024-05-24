#pragma once

#include "IDetector.h"
#include "IFixture.h"

namespace Physics {

class IBody :
	public IFixture {
protected:
	IBody() = default;
public:
	virtual ~IBody() = default;

public:
	virtual void SetPosition(float x, float y) = 0;
	virtual const float* GetPosition() const = 0;

	virtual size_t AddDetectorCircle() = 0;
	virtual size_t AddDetectorRows() = 0;

	virtual IDetector* GetDetector(size_t id) = 0;
};

}
