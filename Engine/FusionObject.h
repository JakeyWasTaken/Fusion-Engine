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

		Object(Model* p_model, const char* objectName)
		{
			m_model = p_model;
			m_name = objectName;
		}

		Model* GetModel() const { return m_model; }

		void SetModel(Model* p_model)
		{
			m_model = p_model;
		}

		void Draw(Camera camera)
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, position);
			model *= glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
			model = glm::scale(model, scale);

			m_model->Draw(model, drawWireframe, camera);
		}

		void GetObjectTopologyStats(uint32_t& vertices, uint32_t& triangles, uint32_t& meshes)
		{
			m_model->GetObjectTopologyStats(vertices, triangles, meshes);
		}

		const char* GetName() const { return m_name; }
	private:
		Model* m_model;
		const char* m_name;
	};
}