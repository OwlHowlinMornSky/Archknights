#pragma once

#include "ISummonMngr.h"

#include "../Models/IAnimationSpine.h"
#include "EntityFactory.h"
#include <vector>

namespace Game {

struct SummonData {
	bool animDouble;
	size_t id;
	std::unique_ptr<ohms::ISpinePose> animPose[2];
	std::unique_ptr<EntityFactory> factory;
};

class SummonMngr final :
	public ISummonMngr {
public:
	SummonMngr();
	virtual ~SummonMngr();

public:
	virtual void AddBegin() override;
	virtual void AddEntity(size_t id, std::string_view testname, bool test) override;
	virtual void AddEnd() override;

public:
	virtual void OnJoined();
	virtual void OnKicking();

	virtual void FixedUpdate(float dt);

	virtual MsgResultType ReceiveMessage(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam);

protected:
	std::vector<SummonData> m_data;
	std::unique_ptr<ohms::ISpineFactory> m_animFactory;
};

}
