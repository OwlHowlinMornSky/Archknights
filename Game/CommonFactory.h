#pragma once

#include "UnitFactory.h"

namespace game {

template <class _T>
class CommonFactory :
	public UnitFactory {
protected:
	virtual std::shared_ptr<Unit> CreateUnit() const override {
		return std::make_shared<_T>();
	}
};

}

