#pragma once 

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {

private:
	std::string directory;
	std::vector<Mesh> meshes;
	std::vector<vec3> instances;
	void ProcessNode(aiNode * node, const aiScene * scene);
	Mesh ProcessMesh(aiMesh * mesh, const aiScene * scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName);

public:
	Model(const char*);

	//Used for instance drawing
	Model(const char*, std::vector<vec3> instanceData);

	void LoadModel(const char* path);
	Model();
	~Model();
	void Draw(Shader& shader);
	void DrawInstanced(Shader& shader);
	void SetInstances(std::vector<vec3> instance);
};
