#include "Model.h"
#include "TextureLoader.h"

Model::Model()
{

}

Model::Model(const char* path, std::vector<vec3> instance)
{
  this->instances = instance;
  LoadModel(path);
}
//call immediately after load model to take effect
void Model::SetInstances(std::vector<vec3> instance){
  this->instances = instance;
  std::cout << instance.size() << std::endl;
}

Model::~Model()
{
}

void Model::LoadModel(const char* path) {

  Assimp::Importer importer;

  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || !scene->mRootNode || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE) {
    std::cerr << "Error:: Assimp " << path << std::endl;
  }
  std::string dir = std::string(path);
  this->directory = dir.substr(0, dir.find_last_of('/'));
  ProcessNode(scene->mRootNode, scene);
}



void Model::ProcessNode(aiNode * node, const aiScene * scene)
{

  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    this->meshes.push_back(ProcessMesh(mesh, scene));
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    ProcessNode(node->mChildren[i], scene);
  }
}



Mesh Model::ProcessMesh(aiMesh * mesh, const aiScene * scene)
{
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    vec3 vector;
    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    vertex.position = vector;
	
    vector.x = mesh->mNormals[i].x;
    vector.y = mesh->mNormals[i].y;
    vector.z = mesh->mNormals[i].z;
    vertex.normal = vector;

    if (mesh->mTextureCoords[0]) {
      vec2 vec;
      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
      vertex.texCoord = vec;
    }
    else {
      vertex.texCoord = vec2(0.0f, 0.0f);
    }
    vertices.push_back(vertex);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  if (mesh->mMaterialIndex >= 0) {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	
    std::vector<Texture> diffuseMap = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMap.begin(), diffuseMap.end());

    std::vector<Texture> specularMap = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMap.begin(), specularMap.end());

		
    std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
  }

  return Mesh(vertices, indices, textures, instances);
}



std::vector<Texture> Model::LoadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName)
{
  std::vector<Texture> texture;
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    Texture t;
    aiString str;
    mat->GetTexture(type, i, &str);	
    std::string filePath = std::string(str.C_Str());
    filePath = directory + "/" + filePath;
    t.id = TextureLoader::LoadTexture(filePath.c_str());
    t.type = typeName;
    t.name = filePath;
    texture.push_back(t);
  }

  return texture;
}

void Model::Draw(Shader& shader) {
  for (unsigned int i = 0; i < meshes.size(); i++)
    meshes[i].Draw(shader);
}
