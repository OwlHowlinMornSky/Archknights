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
#include <GL/glew.h>

#include "Ground.h"
//#include "V3DLight.h"
#include <MysteryEngine/G3D/Camera.h>
#include <MysteryEngine/G3D/GlCheck.h>
#include <MysteryEngine/G3D/Vertex.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace {

const std::string fragment_ground =
"#version 330\n"

"uniform sampler2D uShadowTex;"
"uniform sampler2D uTexture;"

"varying vec4 vTint;"
"varying vec2 vUv;"
"varying vec3 vShadowPos;"

"void main() {"
" vec4 shadowClr = texture2D(uShadowTex, vShadowPos.xy);"
" vec4 texColor = texture2D(uTexture, vUv);"
" gl_FragColor = mix(texColor * vTint, vec4(shadowClr.rgb, 1.0), (vShadowPos.z < 0.5 ? shadowClr.a : 0.0));"
"}";

const std::string vertex_ground =
"#version 330\n"

"attribute vec3 aVertexPos;"
"attribute vec2 aVertexTexCoord;"
"attribute vec4 aVertexColor;"

"uniform mat4 uMatPV;"
"uniform mat4 uMatM;"
"uniform vec2 uGroundSz;"

"varying vec4 vTint;"
"varying vec2 vUv;"
"varying vec3 vShadowPos;"

"void main() {"
" vTint = aVertexColor;"
" vUv = aVertexTexCoord;"

" vec4 vWorldPos = uMatM * vec4(aVertexPos, 1.0);"
" vShadowPos = vWorldPos.xyz / vec3(uGroundSz, 1.0);"
" gl_Position = uMatPV * vWorldPos;"

"}";

class GroundShader final :
	public ME::Shader {
protected:
	int m_uniforms[3];

public:
	GroundShader() :
		m_uniforms() {}
	virtual ~GroundShader() = default;

public:
	virtual void setup() override {
		clear();
		loadFromMemory(vertex_ground, ME::ShaderType::Vertex);
		loadFromMemory(fragment_ground, ME::ShaderType::Fragment);
		glCheck(glBindAttribLocation(m_program, static_cast<GLuint>(ME::VertexAttribute::Position), "aVertexPos"));
		glCheck(glBindAttribLocation(m_program, static_cast<GLuint>(ME::VertexAttribute::TexCoord), "aVertexTexCoord"));
		glCheck(glBindAttribLocation(m_program, static_cast<GLuint>(ME::VertexAttribute::Color), "aVertexColor"));
		linkShader();
		Bind(this);

		m_uniforms[0] = getUniformLocation("uMatPV");
		m_uniforms[1] = getUniformLocation("uMatM");
		m_uniforms[2] = getUniformLocation("uGroundSz");

		updateUniform1iName("uShadowTex", 0);
		updateUniform1iName("uTexture", 1);

		Bind(nullptr);
	}

	virtual void UpdateUniform(int id, GLfloat* data) const override {
		switch (id) {
		case 0:
			updateUniformMat4fv(m_uniforms[0], data);
			break;
		case 1:
			updateUniformMat4fv(m_uniforms[1], data);
			break;
		case 2:
			updateUniform2f(m_uniforms[2], data[0], data[1]);
			break;
		}
	}

	virtual void UpdateUniform1(int id, GLfloat val0) const override {}
	virtual void UpdateUniform2(int id, GLfloat val0, GLfloat val1) const override {
		updateUniform2f(m_uniforms[id], val0, val1);
	}
	virtual void UpdateUniform3(int id, GLfloat val0, GLfloat val1, GLfloat val2) const override {
		updateUniform3f(m_uniforms[id], val0, val1, val2);
	}
	virtual void UpdateUniform4(int id, GLfloat val0, GLfloat val1, GLfloat val2, GLfloat val3) const override {
		updateUniform4f(m_uniforms[id], val0, val1, val2, val3);
	}

	virtual void UpdateUniformI1(int id, GLint val) const override {
		updateUniform1i(m_uniforms[id], val);
	}
};

}

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace Model {

ObjModel::ObjModel() :
	m_groundSz{ 1.0f, 1.0f } {}

ObjModel::~ObjModel() {}

bool ObjModel::Setup() {
	m_shader = std::make_unique<::GroundShader>();
	m_shader->setup();
	//material = new V3DMaterial();
	return true;
}

void ObjModel::Clear() {
	m_shader->clear();

	for (unsigned int i = 0; i < modelData.size(); i++) {
		glCheck(glDeleteBuffers(1, &modelData[i].vb));
		glCheck(glDeleteVertexArrays(1, &modelData[i].vao));
	}

	modelData.clear();
	modelData.shrink_to_fit();
	materials.clear();
	materials.shrink_to_fit();

	//delete material;
}

bool ObjModel::LoadModelData(const char* filename) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::string err;

	modelData.clear();
	materials.clear();
	textures.clear();

	std::string path = GetBaseDir(filename);
	path += "/";

	bool r = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename, path.c_str());

	if (!r) {
		//VLog("%s failed to load", filename);
		return false;
	}

	materials.push_back(tinyobj::material_t());
	tinyobj::InitMaterial(&materials.back());

	for (const tinyobj::material_t& mat : materials) {
		//if (mat.diffuse_texname == "")
		if (mat.diffuse_texname.empty())
			continue;
		if (textures.find(mat.diffuse_texname) == textures.end()) {
			sf::Texture tex;
			if (!tex.loadFromFile(path + mat.diffuse_texname)) {
				//VLog("Unable to find texture %s", mat.diffuse_texname.c_str());
				return false;
			}
			tex.setSmooth(true);
			//tex.setSrgb(true);
			textures.insert(std::make_pair(mat.diffuse_texname, tex));
		}
	}

	for (const tinyobj::shape_t& shape : shapes) {
		ModelData md;
		std::vector<float> vb; // pos(3float), normal(3float), color(4float)

		for (unsigned int f = 0; f < shape.mesh.indices.size() / 3; ++f) {

			const tinyobj::index_t& idx0 = shape.mesh.indices[3 * f + 0];
			const tinyobj::index_t& idx1 = shape.mesh.indices[3 * f + 1];
			const tinyobj::index_t& idx2 = shape.mesh.indices[3 * f + 2];

			int currentMatID = shape.mesh.material_ids[f];

			if ((currentMatID < 0) || (currentMatID >= static_cast<int>(materials.size()))) {
				// Invaid material ID. Use default material.
				currentMatID = (int)materials.size() - 1; // Default material is added to the last item in `materials`.
			}

			float diffuse[3];
			for (unsigned int i = 0; i < 3; i++) {
				diffuse[i] = materials[currentMatID].diffuse[i];
			}

			float tc[3][2];
			if (attrib.texcoords.size() > 0) {
				tc[0][0] = attrib.texcoords[2 * idx0.texcoord_index];
				tc[0][1] = 1.0f - attrib.texcoords[2 * idx0.texcoord_index + 1];
				tc[1][0] = attrib.texcoords[2 * idx1.texcoord_index];
				tc[1][1] = 1.0f - attrib.texcoords[2 * idx1.texcoord_index + 1];
				tc[2][0] = attrib.texcoords[2 * idx2.texcoord_index];
				tc[2][1] = 1.0f - attrib.texcoords[2 * idx2.texcoord_index + 1];
			}
			else {
				tc[0][0] = 0.0f;
				tc[0][1] = 0.0f;
				tc[1][0] = 0.0f;
				tc[1][1] = 0.0f;
				tc[2][0] = 0.0f;
				tc[2][1] = 0.0f;
			}

			float v[3][3];
			for (int k = 0; k < 3; k++) {
				int f0 = idx0.vertex_index;
				int f1 = idx1.vertex_index;
				int f2 = idx2.vertex_index;

				v[0][k] = attrib.vertices[3 * f0 + k];
				v[1][k] = attrib.vertices[3 * f1 + k];
				v[2][k] = attrib.vertices[3 * f2 + k];
			}

			float n[3][3];
			if (attrib.normals.size() > 0) {
				int f0 = idx0.normal_index;
				int f1 = idx1.normal_index;
				int f2 = idx2.normal_index;
				for (int k = 0; k < 3; k++) {
					n[0][k] = attrib.normals[3 * f0 + k];
					n[1][k] = attrib.normals[3 * f1 + k];
					n[2][k] = attrib.normals[3 * f2 + k];
				}
			}
			else {
				// compute geometric normal
				CalcNormal(n[0], v[0], v[1], v[2]);
				n[1][0] = n[0][0];
				n[1][1] = n[0][1];
				n[1][2] = n[0][2];
				n[2][0] = n[0][0];
				n[2][1] = n[0][1];
				n[2][2] = n[0][2];
			}

			for (int k = 0; k < 3; k++) {
				vb.push_back(v[k][0]);
				vb.push_back(v[k][1]);
				vb.push_back(v[k][2]);
				vb.push_back(n[k][0]);
				vb.push_back(n[k][1]);
				vb.push_back(n[k][2]);
				// Combine normal and diffuse to get color.
				float c[3] = {
					diffuse[0],
					diffuse[1],
					diffuse[2]
				};
				/*float len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
				if (len2 > 0.0f) {
					float len = sqrtf(len2);

					c[0] /= len;
					c[1] /= len;
					c[2] /= len;
				}*/
				vb.push_back(c[0]);
				vb.push_back(c[1]);
				vb.push_back(c[2]);
				vb.push_back(1.0f);

				vb.push_back(tc[k][0]);
				vb.push_back(tc[k][1]);
			}
		}

		md.triangleCount = 0;

		if (vb.size() > 0) {
			md.triangleCount = (int)vb.size() / (3 + 3 + 4 + 2) / 3; // 3:vtx, 3:normal, 4:col, 2:texcoord
			glCheck(glGenVertexArrays(1, &md.vao));
			glCheck(glBindVertexArray(md.vao));
			glCheck(glGenBuffers(1, &md.vb));

			unsigned int stride = (3 + 3 + 4 + 2) * (unsigned int)sizeof(float);
			unsigned long long normalOffset = 3 * sizeof(float);
			unsigned long long colorOffset = normalOffset + (3 * sizeof(float));
			unsigned long long texCoordOffset = colorOffset + (4 * sizeof(float));

			glCheck(glBindBuffer(GL_ARRAY_BUFFER, md.vb));
			glCheck(glBufferData(GL_ARRAY_BUFFER, md.triangleCount * 3 * stride, vb.data(), GL_STATIC_DRAW));
			glCheck(glEnableVertexAttribArray(static_cast<GLuint>(ME::VertexAttribute::Position)));
			glCheck(glVertexAttribPointer(static_cast<GLuint>(ME::VertexAttribute::Position), 3, GL_FLOAT, GL_FALSE, stride, 0));
			glCheck(glEnableVertexAttribArray(static_cast<GLuint>(ME::VertexAttribute::Normal)));
			glCheck(glVertexAttribPointer(static_cast<GLuint>(ME::VertexAttribute::Normal), 3, GL_FLOAT, GL_TRUE, stride, (void*)normalOffset));
			glCheck(glEnableVertexAttribArray(static_cast<GLuint>(ME::VertexAttribute::Color)));
			glCheck(glVertexAttribPointer(static_cast<GLuint>(ME::VertexAttribute::Color), 4, GL_FLOAT, GL_FALSE, stride, (void*)colorOffset));
			glCheck(glEnableVertexAttribArray(static_cast<GLuint>(ME::VertexAttribute::TexCoord)));
			glCheck(glVertexAttribPointer(static_cast<GLuint>(ME::VertexAttribute::TexCoord), 2, GL_FLOAT, GL_FALSE, stride, (void*)texCoordOffset));
			glCheck(glBindVertexArray(0));
		}

		// OpenGL viewer does not support texturing with per-face material.
		if (shape.mesh.material_ids.size() > 0) {
			// Base case
			md.materialID = shape.mesh.material_ids[shape.mesh.material_ids.size() - 1];
		}
		else {
			md.materialID = materials.size() - 1; // = ID for default material.
		}

		if ((md.materialID < materials.size())) {
			auto it = textures.find(materials[md.materialID].diffuse_texname);
			if (it == textures.end())
				md.tex = nullptr;
			else
				md.tex = &(it->second);
		}

		modelData.push_back(md);
	}
	return r;
}

void ObjModel::SetSize(float x, float y) {
	m_groundSz[0] = x;
	m_groundSz[1] = y;
}

std::string ObjModel::GetBaseDir(const std::string& filepath) {
	if (filepath.find_last_of("/\\") != std::string::npos)
		return filepath.substr(0, filepath.find_last_of("/\\"));
	return "";
}

void ObjModel::CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]) {
	float v10[3];
	v10[0] = v1[0] - v0[0];
	v10[1] = v1[1] - v0[1];
	v10[2] = v1[2] - v0[2];

	float v20[3];
	v20[0] = v2[0] - v0[0];
	v20[1] = v2[1] - v0[1];
	v20[2] = v2[2] - v0[2];

	N[0] = v20[1] * v10[2] - v20[2] * v10[1];
	N[1] = v20[2] * v10[0] - v20[0] * v10[2];
	N[2] = v20[0] * v10[1] - v20[1] * v10[0];

	float len2 = (N[0] * N[0]) + (N[1] * N[1]) + (N[2] * N[2]);
	if (len2 > 0.0f) {
		float len = sqrtf(len2);

		N[0] /= len;
		N[1] /= len;
		N[2] /= len;
	}
}

void ObjModel::Update(float dt) {}

void ObjModel::Draw(ME::Camera* camera, ME::Shader* shader) {
	ME::Shader::Bind(m_shader.get());

	UpdateShader(camera, m_shader.get());

	for (unsigned int i = 0; i < modelData.size(); i++) {
		ModelData& md = modelData[i];

		if (md.tex) {
			sf::Texture::bind(md.tex);
		}
		else {
			//glCheck(glBindTexture(GL_TEXTURE_2D, Model::DefaultTexture));
		}

		/*if ((md.materialID < materials.size())) {
			tinyobj::material_t& mat = materials[md.materialID];

			material->Specular.x = mat.specular[0];
			material->Specular.y = mat.specular[1];
			material->Specular.z = mat.specular[2];
			material->Shininess = mat.shininess;

			shader->UpdateUniform(UniformType::Material, material);
		}*/

		/*
		if ((md.materialID < materials.size()))
		{
			tinyobj::material_t &mat = materials[md.materialID];

			//Set texture
			std::string diffuse_texname = mat.diffuse_texname;
			if (textures.find(diffuse_texname) == textures.end())
				glCheck(glBindTexture(GL_TEXTURE_2D, V3DModel::DefaultTexture));
			else
				sf::Texture::bind(&textures[diffuse_texname]);

			material->Specular.x = mat.specular[0];
			material->Specular.y = mat.specular[1];
			material->Specular.z = mat.specular[2];
			material->Shininess = mat.shininess;

			shader->UpdateUniform(UniformType::Material, material);
		}*/

		// Draw the model
		glCheck(glBindVertexArray(md.vao));
		glCheck(glDrawArrays(GL_TRIANGLES, 0, 3 * md.triangleCount));

		glCheck(glBindVertexArray(0));
		glCheck(glBindTexture(GL_TEXTURE_2D, 0));
	}
	ME::Shader::Bind(shader);
}

void ObjModel::UpdateShader(ME::Camera* camera, ME::Shader* shader) {
	//if (textures.size() == 0)
	//	V3DModel::GenerateDefaultTexture();

	ComputeMatrix();

	//glm::mat4 viewProj = camera.getMatPV() * m_matM;// transform;
	//shader.updateUniformMat4fvName("uMatPVM", &viewProj[0][0]);
	shader->UpdateUniform(0, &camera->getMatPV()[0][0]);
	shader->UpdateUniform(1, &m_matM[0][0]);
	shader->UpdateUniform(2, &m_groundSz[0]);

}

std::shared_ptr<IObjModel> IObjModel::Create() {
	return std::make_shared<ObjModel>();
}

}
