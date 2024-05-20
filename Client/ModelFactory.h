#pragma once

#include <MysteryEngine/G3D/IModel.h>
#include <memory>

namespace ohms {

class ISpineEntity :
	public ME::IModel {
};

class ISpineEntitySet {
public:
	virtual ISpineEntity* runOneEntity() = 0;
};

class ISpineManager {
public:
	virtual ISpineEntitySet* addPose(const std::string& name, unsigned char type) = 0;
};

}

namespace Game {

class IActorGroup :
	public ME::IModel {
public:
	virtual void AddActor(std::shared_ptr<IModel> actor) = 0;
};

}

///3D Model Class for Loading and Rendering models in the .OBJ file format using TinyOBJLoader.
class IObjModel :
	public ME::IModel {
public:
	virtual bool LoadModelData(const char* filename) = 0;
};

namespace Factory {

std::unique_ptr<ohms::ISpineManager> Create_SpineManager();
std::shared_ptr<Game::IActorGroup> Create_ActorGroup();
std::shared_ptr<IObjModel> Create_ObjModel();

}
