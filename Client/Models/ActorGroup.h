#pragma once

#include <MysteryEngine/G3D/IModel.h>
#include <memory>
#include <list>
#include "../ModelFactory.h"

namespace Game {

namespace ActorVertexAttribute {
enum ME_API : unsigned int {
	Position = 0, // 顶点坐标。
	TexCoord,     // 纹理坐标。
	Color,        // 颜色。
	COUNT         // [计数]
};
} // namespace ActorVertexAttribute


class ActorShader final :
	public ME::Shader {
public:
	int m_matPVM;
	int m_matM;
	int m_campos;
	int m_offset;
	int m_cover;
	int m_coverClr;

public:
	virtual void setup() override;
};

struct ActorVertex {
	ActorVertex() :
		position({ 0 }),
		color({ 0 }),
		texCoord({ 0 }) {}

	ActorVertex(const glm::vec2& pos) :
		position(pos),
		color(1, 1, 1, 1),
		texCoord({ 0 }) {}

	ActorVertex(const glm::vec2& pos, const glm::vec2& tex) :
		position(pos),
		color(1, 1, 1, 1),
		texCoord(tex) {}

	ActorVertex(const glm::vec2& pos, const glm::vec4& tint) :
		position(pos),
		color(tint),
		texCoord({ 0 }) {}

	ActorVertex(const glm::vec2& pos, const glm::vec2& tex, const glm::vec4& tint) :
		position(pos),
		color(tint),
		texCoord(tex) {}

	glm::vec2 position; // 顶点坐标。
	glm::vec2 texCoord; // 纹理坐标。
	glm::vec4 color;    // 颜色。
};

class ActorGroup final:
	public IActorGroup {
public:
	ActorGroup();

public:
	void AddActor(std::shared_ptr<IModel> actor);

public:
	virtual void Update(float dt) override;
	virtual void Draw(ME::Camera& camera, ME::Shader& shader) override;

protected:
	std::list<std::shared_ptr<IModel>> m_actors;
	std::unique_ptr<ME::Shader> m_shader;
};

}
