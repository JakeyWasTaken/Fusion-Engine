#pragma once

// Misc headers
#include <iostream>
#include <format>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <memory>
#include <fstream>
#include <sstream>
#include <ShlObj.h>
#include <atlbase.h>
#include <stdlib.h>

// Rendering headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/string_cast.hpp>

// DearImGui headers
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Namespace level information
namespace Fusion
{
	float DeltaTime = 0;

    bool SceneReady = false;

	unsigned int NextModelIndex = 0;

	unsigned int GetModelIndex()
	{
		return NextModelIndex++;
	}

	GLFWwindow* CurrentWindow = nullptr;

	glm::vec3 LightPosition(0.0f, 1.5f, -3.0f);
	glm::vec3 LightColor(1.0f, 1.0f, 1.0f);

    glm::mat4 CameraProj;
    glm::mat4 CameraView;

	enum ShaderType
	{
		Vertex,
		Fragment
	};

}

char* wchar_to_char(const wchar_t* pwchar)
{
    // get the number of characters in the string.
    int currentCharIndex = 0;
    char currentChar = pwchar[currentCharIndex];

    while (currentChar != '\0')
    {
        currentCharIndex++;
        currentChar = pwchar[currentCharIndex];
    }

    const int charCount = currentCharIndex + 1;

    // allocate a new block of memory size char (1 byte) instead of wide char (2 bytes)
    char* filePathC = (char*)malloc(sizeof(char) * charCount);

    for (int i = 0; i < charCount; i++)
    {
        // convert to char (1 byte)
        char character = pwchar[i];

        *filePathC = character;

        filePathC += sizeof(char);

    }
    filePathC += '\0';

    filePathC -= (sizeof(char) * charCount);

    return filePathC;
}

// Engine headers, the order of these is important.
#include "FusionConfig.h"
#include "FusionFile.h"
#include "FusionShaderFactory.h"
#include "FusionShader.h"
#include "FusionWindow.h"
#include "FusionCamera.h"
#include "FusionTexture.h"
#include "FusionMesh.h"
#include "FusionModel.h"
#include "FusionObject.h"
#include "FusionScene.h"
#include "FusionSceneSerialization.h"
#include "DebugInterface.h"