#pragma once
#include "FusionCore.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

std::vector<Fusion::MeshTexture> textures_loaded;

namespace Fusion
{
	class Model
	{
	public:
		Model(const char* filePath)
		{
			loadModel(filePath);
		}

		void Draw(Shader& shader)
		{
			for (unsigned int i = 0; i < meshes.size(); i++)
				meshes[i].Draw(shader);
		}
	private:
		std::vector<Mesh> meshes;
		std::string directory;

		void loadModel(std::string filePath)
		{
			std::string FilePath = FileLoader::GetFullFilePath(filePath);

			Assimp::Importer import;
			const aiScene* scene = import.ReadFile(FilePath, aiProcess_Triangulate | aiProcess_FlipUVs);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				printf("[MODEL][ASSIMP][ERROR] %s", import.GetErrorString());
				return;
			}

			directory = FilePath.substr(0, FilePath.find_last_of("/"));

			processNode(scene->mRootNode, scene);
		}

		void processNode(aiNode* node, const aiScene* scene)
		{
			// process all the node's meshes (if any)
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				meshes.push_back(processMesh(mesh, scene));
			}
			// then do the same for each of its children
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				processNode(node->mChildren[i], scene);
			}
		}

		Mesh processMesh(aiMesh* mesh, const aiScene* scene)
		{
			std::vector<MeshVertex> vertices;
			std::vector<unsigned int> indices;
			std::vector<MeshTexture> textures;

			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				MeshVertex vertex;
				// process vertex positions, normals and texture coordinates
				glm::vec3 vector;
				vector.x = mesh->mVertices[i].x;
				vector.y = mesh->mVertices[i].y;
				vector.z = mesh->mVertices[i].z;
				vertex.Position = vector;

				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.Normal = vector;

				if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
				{
					glm::vec2 vec;
					vec.x = mesh->mTextureCoords[0][i].x;
					vec.y = mesh->mTextureCoords[0][i].y;
					vertex.TexCoords = vec;
				}
				else
					vertex.TexCoords = glm::vec2(0.0f, 0.0f);

				vertices.push_back(vertex);
			}
			// process indices
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++)
					indices.push_back(face.mIndices[j]);
			}
			// process material
			if (mesh->mMaterialIndex >= 0)
			{
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
				std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(material,
					aiTextureType_DIFFUSE, "texture_diffuse");
				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
				std::vector<MeshTexture> specularMaps = loadMaterialTextures(material,
					aiTextureType_SPECULAR, "texture_specular");
				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			}

			return Mesh(vertices, indices, textures);
		}

		std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
		{
			std::vector<MeshTexture> textures;
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);
				bool skip = false;
				for (unsigned int j = 0; j < textures_loaded.size(); j++)
				{
					if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
					{
						textures.push_back(textures_loaded[j]);
						skip = true;
						break;
					}
				}
				if (!skip)
				{   // if texture hasn't been loaded already, load it
					MeshTexture texture;
					texture.id = TextureFromFile(str.C_Str(), false);
					texture.type = typeName;
					texture.path = str.C_Str();
					textures.push_back(texture);
					textures_loaded.push_back(texture); // add to loaded textures
				}
			}
			return textures;
		}
	};
}