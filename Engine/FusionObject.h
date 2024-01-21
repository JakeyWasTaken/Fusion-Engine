#pragma once
#include "FusionCore.h"

namespace Fusion
{
	class Object
	{
	public:
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 rotation = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);
		bool drawWireframe = FS_DRAW_WIRE_FRAME;

		Object(Model model, const char* objectName)
		{
			m_model = model;
			m_name = objectName;
		}

		Object(Model model, const char* objectName, glm::vec3 modelPosition, glm::vec3 modelScale)
		{
			m_model = model;
			m_name = objectName;
			position = modelPosition;
			scale = modelScale;
		}

		Model GetModel() const { return m_model; }

		void SetModel(Model model)
		{
			m_model = model;
		}

		void Draw(Camera camera)
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, position);
			model *= glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
			model = glm::scale(model, scale);

			m_model.Draw(model, drawWireframe, camera);
		}

		void GetObjectTopologyStats(uint32_t& vertices, uint32_t& triangles, uint32_t& meshes)
		{
			std::vector<Mesh> modelMeshes = m_model.GetMeshes();

			meshes = (uint32_t)modelMeshes.size();

			for (uint32_t i = 0; i < modelMeshes.size(); i++)
			{
				Mesh mesh = modelMeshes[i];
				vertices += mesh.vertices.size();
				triangles += (uint32_t)glm::floor(mesh.indices.size() / 3);
			}
		}

		const char* GetName() const { return m_name; }
	private:
		Model m_model;
		const char* m_name;
	};
}