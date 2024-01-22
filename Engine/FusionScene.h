// Holds a vector of Objects and can draw them all in one call

#pragma once
#include "FusionCore.h"

namespace Fusion
{
	class Scene
	{
	public:
		Scene() {}

		Scene(std::vector<Object*> p_objects)
		{
			m_Objects = p_objects;
		}

		void PushObject(Object* p_object)
		{
			m_Objects.push_back(p_object);
		}

		std::vector<Object*>* GetObjects() { return &m_Objects; }
		std::vector<Model*>* GetModels() { return &m_Models; }
		std::vector<Shader*>* GetShaders() { return &m_Shaders; }

		void GetUniqueDescendants(std::vector<Shader*>* p_shaders, std::vector<Model*>* p_models)
		{
			std::map<unsigned int, bool> ItemIds;

			auto objects = GetObjects();

			for (unsigned int i = 0; i < objects->size(); i++)
			{
				Object* object = (*objects)[i];
				Model* model = object->GetModel();

				if (ItemIds[model->Id]) // if we have already added this model to the vector
					continue;

				ItemIds[model->Id] = true;
				p_models->push_back(model);
			}

			ItemIds.clear(); // flush the map so we can do this again for shaders

			for (unsigned int i = 0; i < p_models->size(); i++)
			{
				Model* model = (*p_models)[i];
				Shader* shader = model->GetShader();

				if (ItemIds[shader->getProgId()]) // if we have already added this shader to the vector
					continue;

				ItemIds[shader->getProgId()] = true;
				p_shaders->push_back(shader);
			}
		}

		void ClearScene()
		{
			std::vector<Shader*> shaders;
			std::vector<Model*> models;

			GetUniqueDescendants(&shaders, &models);

			for (uint32_t i = 0; i < shaders.size(); i++)
			{
				delete shaders[i];
			}

			for (uint32_t i = 0; i < models.size(); i++)
			{
				delete models[i];
			}

			for (uint32_t i = 0; i < m_Objects.size(); i++)
			{
				delete m_Objects[i];
			}

			m_Objects.clear();
		}

		void Draw(Camera camera)
		{
			for (unsigned int i = 0; i < m_Objects.size(); i++)
			{
				Object* p_object = m_Objects[i];

				p_object->Draw(camera);
			}
		}
		
		// TODO: add a reload shader func here so we only reload indivdual shaders

		void ReloadShaders()
		{
			std::vector<Shader*> shaders;
			std::vector<Model*> _;

			GetUniqueDescendants(&shaders, &_);

			printf("[DEBUG] Reloading shaders\n");

			for (uint32_t i = 0; i < shaders.size(); i++)
			{
				Shader* shader = shaders[i];

				printf("[DEBUG] Reloading shader progId: %d\n", shader->getProgId());
				shader->reloadShader();
			}

		}

		void GetSceneTopologyStats(uint32_t& vertices, uint32_t& triangles, uint32_t& objects, uint32_t& models, uint32_t& meshes)
		{
			std::map<float, bool> modelKeys;

			for (uint32_t i = 0; i < m_Objects.size(); i++)
			{
				Object* object = m_Objects[i];
				Model* model = object->GetModel();

				uint32_t objectVert = 0;
				uint32_t objectTris = 0;
				uint32_t objectMeshes = 0;

				object->GetObjectTopologyStats(objectVert, objectTris, objectMeshes);
				vertices += objectVert;
				triangles += objectTris;
				meshes += objectMeshes;

				if (modelKeys[model->Id])
					continue;

				models++;
				modelKeys[model->Id] = true;
			}

			objects = (uint32_t)m_Objects.size();
		}

	private:
		std::vector<Object*> m_Objects;
		std::vector<Model*> m_Models; // for the sake of not dangling pointers
		std::vector<Shader*> m_Shaders; // for the sake of not dangling pointers
	};
}