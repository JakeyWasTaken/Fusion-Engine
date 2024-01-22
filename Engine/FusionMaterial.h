#pragma once
#include "FusionCore.h"

/*
* Materials takes in Diffuse, Specular, Normal, and a shader
* Materials can then be assigned to Models which then call Draw()
*/

namespace Fusion
{
	class Material
	{
	public:
		Material() {}

		Material(std::vector<MeshTexture> textures, Shader* p_shader)
		{
			m_textures.insert(m_textures.end(), textures.begin(), textures.end());
			m_shader = p_shader;
		}

		void PreDraw(Camera camera)
		{
			unsigned int diffuseIndex = 1;
			unsigned int specularIndex = 1;
			unsigned int normalIndex = 1;

			for (unsigned int i = 0; i < m_textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
				// retrieve texture number (the N in diffuse_textureN)
				std::string number;
				std::string name = m_textures[i].type;

				if (name == "texture_diffuse")
					number = std::to_string(diffuseIndex++);
				else if (name == "texture_specular")
					number = std::to_string(specularIndex++);
				else if (name == "texture_normal")
					number = std::to_string(normalIndex++);

				m_shader->setInt("material." + name + number, i);
				glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
			}
			glActiveTexture(GL_TEXTURE0);

			m_shader->setVec3("lightPosition", LightPosition);
			m_shader->setVec3("cameraPosition", camera.GetPosition());
			m_shader->setVec3("lightColor", LightColor);

			m_shader->setMat4("projection", Fusion::CameraProj);
			m_shader->setMat4("view", Fusion::CameraView);

			m_shader->use();
		}

	private:
		std::vector<MeshTexture> m_textures;
		Shader* m_shader;
	};
}