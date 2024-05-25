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

	virtual size_t AddDetectorCircle(float x, float y, float radius) = 0;
	virtual size_t AddDetectorRows(float x, float y, Rows rows) = 0;

	virtual IDetector* GetDetector(size_t id) = 0;
};

}
