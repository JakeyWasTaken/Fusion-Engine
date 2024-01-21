#pragma once
#include "FusionCore.h"

namespace Fusion
{
	struct MeshVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec2 TexCoords;
	};

	struct MeshTexture {
		unsigned int id;
		std::string type;
		std::string path;
	};
}

#include "FusionMaterial.h"

namespace Fusion
{

	class Mesh
	{
	public:
		// Mesh Data
		std::vector<MeshVertex> vertices;
		std::vector<unsigned int> indices;
		Material material;

		Mesh(std::vector<MeshVertex> vertices, std::vector<unsigned int> indices, Material material)
		{
			this->vertices = vertices;
			this->indices = indices;
			this->material = material;

			setupMesh();
		}

		void Draw(bool drawWireframe, Camera camera)
		{
			material.PreDraw(camera);

			// draw mesh
			glPolygonMode(GL_FRONT_AND_BACK, drawWireframe ? GL_LINE : GL_FILL);

			glBindVertexArray(m_VAO);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	private:
		//  render data
		unsigned int m_VAO, m_VBO, m_EBO;

		void setupMesh()
		{
			glGenVertexArrays(1, &m_VAO);
			glGenBuffers(1, &m_VBO);
			glGenBuffers(1, &m_EBO);

			glBindVertexArray(m_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshVertex), &vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
				&indices[0], GL_STATIC_DRAW);

			// vertex positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)0);
			// vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, Normal));
			// vertex texture coords
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, TexCoords));

			glBindVertexArray(0);
		}
	};
}