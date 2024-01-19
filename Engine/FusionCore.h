#pragma once

// Namespace level information
namespace Fusion
{
	float DeltaTime = 0;

	enum ShaderType
	{
		Vertex,
		Fragment
	};

}

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

// Model importing headers
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>

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