#pragma once
#include "FusionCore.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Fusion
{
	unsigned int TextureFromFile(const char* path, bool gamma)
	{
		std::string fp = FileLoader::GetFullFilePath(path);

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(fp.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			printf("[TEXTURE][ERROR] Texture failed to load at path: (%s)", path);
			stbi_image_free(data);
		}

		return textureID;
	}

	class Texture
	{
	public:
		int width, height, nrChannels;

		Texture(std::string filepath, bool flipTexture, unsigned int textureUnit)
		{
			m_textureUnit = textureUnit;

			glGenTextures(1, &m_texture);
			use();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			std::string fp = FileLoader::GetFullFilePath(filepath);
			stbi_set_flip_vertically_on_load(flipTexture);
			unsigned char* data = stbi_load(fp.c_str(), &width, &height, &nrChannels, 0);

			GLenum format;
			if (nrChannels == 1)
				format = GL_RED;
			else if (nrChannels == 3)
				format = GL_RGB;
			else if (nrChannels == 4)
				format = GL_RGBA;

			if (data)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				printf("[TEXTURE] Succesfully generated texture from filepath: (%s)\n", filepath.c_str());
			}
			else
			{
				printf("[TEXTURE][ERROR] Failed to load texture at filepath: (%s). Make sure its the correct file path!\n", filepath.c_str());
			}

			stbi_image_free(data);
		}

		unsigned int GetId() const { return m_texture; };

		void use()
		{
			glActiveTexture(m_textureUnit);
			glBindTexture(GL_TEXTURE_2D, m_texture);
		}
	private:
		unsigned int m_texture;
		unsigned int m_textureUnit;
	};
}