/*
*    Archknights
*
*    Copyright (C) 2023-2024  Tyler Parret True
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Affero General Public License as published
*    by the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Affero General Public License for more details.
*
*    You should have received a copy of the GNU Affero General Public License
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
* @Authors
*    Tyler Parret True <mysteryworldgod@outlook.com><https://github.com/OwlHowlinMornSky>
*/
#pragma once

#include <MysteryEngine/G3D/IModel.h>
#include "tiny_obj_loader.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>
#include <map>
#include <memory>

#include <glm/matrix.hpp>

#include <MysteryEngine/G3D/Shader.h>

#include "IGround.h"
//#include "V3DLight.h"

namespace Model {

///3D Model Class for Loading and Rendering models in the .OBJ file format using TinyOBJLoader.
class ObjModel :
	public IObjModel {
protected:
	///The structure of each individual shape.
	struct ModelData {
		///Vertex Buffer ID.
		unsigned int vb;
		///Vertex Array Object ID.
		unsigned int vao;
		///Number of Triangles in this model.
		int triangleCount;
		///Material ID of this mode.
		size_t materialID;
		sf::Texture* tex;
	};

	///Get the directory of the file.
	static std::string GetBaseDir(const std::string& filepath);
	///Calculate the normal from three vertices.
	static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]);

	///List of models to shape.
	std::vector<ModelData> modelData;
	///List of materials 
	std::vector<tinyobj::material_t> materials;
	///List of textures.
	std::map<sf::String, sf::Texture> textures;
	///Model Matrix.
	//glm::mat4 transform;
	///Material Data for rendering.
	//V3DMaterial* material;

public:
	ObjModel();

	~ObjModel();

	virtual bool setup();
	virtual void clear();
	/**
	Loads Object Model from filepath.
	Any material and texture files should be placed relative to the .obj file.
	*/
	virtual bool loadModelData(const char* filename) override;
	virtual void setGroundSize(float x, float y) override;

	virtual void update(float dt) override;
	virtual void draw(ME::Camera* camera, ME::Shader* shader) override;

protected:
	void UpdateShader(ME::Camera* camera, ME::Shader* shader);

	std::unique_ptr<ME::Shader> m_shader;
	float m_groundSz[2];
};

}
