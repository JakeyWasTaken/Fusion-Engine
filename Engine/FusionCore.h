#pragma once

// Misc headers
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

// Rendering headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Namespace level information
namespace Fusion
{
	float DeltaTime = 0;

	glm::vec3 LightPosition(0.0f, 1.5f, -3.0f);
	glm::vec3 LightColor(1.0f, 1.0f, 1.0f);

	enum ShaderType
	{
		Vertex,
		Fragment
	};

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