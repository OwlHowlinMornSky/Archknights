#pragma once

#include "ISummonMngr.h"

#include "EntityFactory.h"
#include <vector>

namespace Game {

struct SummonData {
	size_t id;
	std::unique_ptr<EntityFactory> factory;
};

class SummonMngr final :
	public ISummonMngr {
public:
	SummonMngr();
	virtual ~SummonMngr();

public:
	virtual void AddBegin() override;
	virtual bool AddEntity(size_t id) override;
	virtual void AddEnd() override;

public:
	virtual void OnJoined();
	virtual void OnKicking();

	virtual void FixedUpdate(float dt);

	virtual MsgResultType ReceiveMessage(MsgIdType msg, MsgWparamType wparam, MsgLparamType lparam);

protected:
	std::vector<SummonData> m_data;
};

}
