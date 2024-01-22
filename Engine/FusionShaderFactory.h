#pragma once
#include "FusionCore.h"

namespace Fusion
{
	namespace ShaderFactory
	{
		void CreateAndCompileVert(const char* shaderSource, unsigned int& shaderHandle)
		{
			shaderHandle = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(shaderHandle, 1, &shaderSource, NULL);
			glCompileShader(shaderHandle);

			int success;
			char infoLog[FS_MAX_CHAR_SIZE];

			glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(shaderHandle, 512, NULL, infoLog);
				printf("[SHADER FACTORY][ERROR] Failed to compile vertex shader\n%s\n", infoLog);
			}
			else
			{
				printf("[SHADER FACTORY] Created and compiled vertex shader\n");
			}
		}

		void CreateAndCompileFrag(const char* shaderSource, unsigned int& shaderHandle)
		{
			shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(shaderHandle, 1, &shaderSource, NULL);
			glCompileShader(shaderHandle);

			int success;
			char infoLog[FS_MAX_CHAR_SIZE];

			glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(shaderHandle, 512, NULL, infoLog);
				printf("[SHADER FACTORY][ERROR] Failed to compile fragment shader\n%s\n", infoLog);
			}
			else
			{
				printf("[SHADER FACTORY] Created and compiled frag shader\n");
			}
		}

		bool CreateShaderProg(unsigned int& shaderProgram, unsigned int vertexShader, unsigned int fragmentShader)
		{
			shaderProgram = glCreateProgram();

			glAttachShader(shaderProgram, vertexShader);
			glAttachShader(shaderProgram, fragmentShader);
			glLinkProgram(shaderProgram);

			int success;
			char infoLog[FS_MAX_CHAR_SIZE];

			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
				printf("[SHADER FACTORY][ERROR] Failed to create shader program\n%s\n", infoLog);
				return false;
			}
			else
			{
				printf("[SHADER FACTORY] Created shader program\n");
				return true;
			}

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
		}

		void CreateShader(const char* filePath, unsigned int& shaderHandle, ShaderType shaderType)
		{
			File shaderFile = FileLoader::GetFile(filePath);
			const char* shaderSource = shaderFile.FileContents.c_str();

			if (shaderType == ShaderType::Vertex)
			{
				CreateAndCompileVert(shaderSource, shaderHandle);

				return;
			}

			if (shaderType == ShaderType::Fragment)
			{
				CreateAndCompileFrag(shaderSource, shaderHandle);
				return;
			}

			printf("[ERROR] Invalid shader type for filepath: (%s)\n", shaderFile.FilePath.c_str());
		}
	}
}