#pragma once

#include <MysteryEngine/G3D/IModel.h>
#include <memory>

class IObjModel :
	public ME::IModel {
public:
	virtual bool LoadModelData(const char* filename) = 0;

public:
	static std::shared_ptr<IObjModel> Create();
};
