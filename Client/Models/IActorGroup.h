#pragma once

#include <MysteryEngine/G3D/IModel.h>
#include <memory>

namespace Game {

namespace ActorShaderUniformId {
enum : int {
	Mat4_PVM = 0,
	Mat4_M,
	Vec3_CamPos,
	Vec2_Offset,
	Int1_CvrClr,
	Vec4_CvrClr,

	COUNT
};
}

class IActorGroup :
	public ME::IModel {
public:
	virtual void AddActor(std::shared_ptr<IModel> actor) = 0;

public:
	static std::shared_ptr<Game::IActorGroup> Create();
};

}
