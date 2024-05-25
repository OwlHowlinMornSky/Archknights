#pragma once

#include "IBody.h"
#include "Detector.h"
#include <box2d/box2d.h>
#include <vector>
#include <memory>

namespace Physics {

class Body final :
	public IBody {
	friend class World;
public:
	Body();
	virtual ~Body();

public:
	virtual void SetPosition(float x, float y) override;
	virtual const float* GetPosition() const override;

	virtual size_t AddDetectorCircle(float x, float y, float radius) override;
	virtual size_t AddDetectorRows(float x, float y, Rows rows) override;

	virtual IDetector* GetDetector(size_t id) override;

	virtual void OnBeginContact(IFixture* another) {}
	virtual void OnEndContact(IFixture* another) {}

protected:
	void CreateCircle(b2World* world, b2Vec2 pos, float radius);

protected:
	b2Body* m_body;
	b2Fixture* m_fixture;
	std::vector<std::unique_ptr<Detector>> m_detectors;
};

}
