#pragma once

#include "..\GUI\ActivityIndependent.h"

namespace Activity {

class Act02_Load final :
	public GUI::ActivityIndependent {
public:
	Act02_Load() noexcept = default;
	virtual ~Act02_Load() noexcept = default;

public:
	virtual void start(GUI::ICarnival& carnival) override;
	virtual void stop() noexcept override;
	virtual void pause() noexcept override;
	virtual void resume() noexcept override;
	virtual uint32_t getID() noexcept override;

public:
	virtual void runIndependently() override;

protected:
	GUI::ICarnival* ref_carnival;
};

}
