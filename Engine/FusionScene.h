// Holds a vector of Objects and can draw them all in one call

#pragma once
#include "FusionCore.h"

namespace Fusion
{
	class Scene
	{
	public:
		Scene() {}

		Scene(std::vector<Object*> objects)
		{
			m_Objects = objects;
		}

		void PushObject(Object* object)
		{
			m_Objects.push_back(object);
		}

		std::vector<Object*> GetObjects() const { return m_Objects; }

		void Draw(Camera camera)
		{
			for (unsigned int i = 0; i < m_Objects.size(); i++)
			{
				Object* object = m_Objects[i];

				object->Draw(camera);
			}
		}

		void GetSceneTopologyStats(uint32_t& vertices, uint32_t& triangles, uint32_t& objects, uint32_t& models, uint32_t& meshes)
		{
			std::map<float, bool> modelKeys;

			for (uint32_t i = 0; i < m_Objects.size(); i++)
			{
				Object* object = m_Objects[i];
				Model model = object->GetModel();

				uint32_t objectVert = 0;
				uint32_t objectTris = 0;
				uint32_t objectMeshes = 0;

				object->GetObjectTopologyStats(objectVert, objectTris, objectMeshes);
				vertices += objectVert;
				triangles += objectTris;
				meshes += objectMeshes;

				if (modelKeys[model.CreationTime])
					continue;

				models++;
				modelKeys[model.CreationTime] = true;
			}

			objects = (uint32_t)m_Objects.size();
		}

	private:
		std::vector<Object*> m_Objects;
	};
}