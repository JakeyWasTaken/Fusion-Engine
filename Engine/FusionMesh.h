#pragma once
#include "FusionCore.h"

namespace Fusion
{
	struct MeshVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct MeshTexture {
		unsigned int id;
		std::string type;
		std::string path;
	};

	class Mesh
	{
	public:
		// Mesh Data
		std::vector<MeshVertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<MeshTexture> textures;

		Mesh(std::vector<MeshVertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures)
		{
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;

			setupMesh();
		}
		void Draw(Shader& shader)
		{
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			for (unsigned int i = 0; i < textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
				// retrieve texture number (the N in diffuse_textureN)
				std::string number;
				std::string name = textures[i].type;
				if (name == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (name == "texture_specular")
					number = std::to_string(specularNr++);

				shader.setInt(("material." + name + number).c_str(), i);
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
			}
			glActiveTexture(GL_TEXTURE0);

			// draw mesh
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