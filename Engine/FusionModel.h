#pragma once
#include "FusionCore.h"
#include "FusionMaterial.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

std::vector<Fusion::MeshTexture> textures_loaded;

namespace Fusion
{
	class Model
	{
	public:
		unsigned int Id = Fusion::GetModelIndex();

		Model() {}

		Model(const char* filePath, const char* modelName, Shader* p_shader)
		{
			m_shader = p_shader;
			m_name = modelName;
			m_path = filePath;
			loadModel(filePath);
		}

		~Model()
		{

		}

		void Draw(glm::mat4 modelMatrix, bool drawWireframe, Camera camera)
		{
			m_shader->setMat4("model", modelMatrix); // Set the matrix in vert shader to our model matrix

			for (unsigned int i = 0; i < meshes.size(); i++)
				meshes[i].Draw(drawWireframe, camera);
		}

		void ReloadShader() { m_shader->reloadShader(); }

		void GetObjectTopologyStats(uint32_t& vertices, uint32_t& triangles, uint32_t& meshes)
		{
			std::vector<Mesh> modelMeshes = GetMeshes();

			meshes = (uint32_t)modelMeshes.size();

			for (uint32_t i = 0; i < modelMeshes.size(); i++)
			{
				Mesh mesh = modelMeshes[i];
				vertices += mesh.vertices.size();
				triangles += (uint32_t)glm::floor(mesh.indices.size() / 3);
			}
		}

		std::vector<Mesh> GetMeshes() const { return meshes; }

		const char* GetPath() const { return m_path; }
		const char* GetName() const { return m_name; }
		Shader* GetShader() const { return m_shader; }
	private:
		std::vector<Mesh> meshes;
		std::string directory;
		const char* m_name;
		const char* m_path;
		Shader* m_shader;

		void loadModel(std::string filePath)
		{
			std::string FilePath = FileLoader::GetFullFilePath(filePath);

			Assimp::Importer import;
			const aiScene* scene = import.ReadFile(FilePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

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

				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.Tangent = vector;

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
				// diffuse
				std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(material,
					aiTextureType_DIFFUSE, "texture_diffuse");
				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
				// specular
				std::vector<MeshTexture> specularMaps = loadMaterialTextures(material,
					aiTextureType_SPECULAR, "texture_specular");
				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
				// normal
				std::vector<MeshTexture> normalMaps = loadMaterialTextures(material,
					aiTextureType_HEIGHT, "texture_normal");
				textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
			}

			Material meshMaterial(textures, m_shader);

			return Mesh(vertices, indices, meshMaterial);
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