#pragma once

// OpenGl definitions
#define FS_GL_VERSION_MAJOR 3
#define FS_GL_VERSION_MINOR 3
#define FS_GL_PROFILE GLFW_OPENGL_CORE_PROFILE

// Application definitions
#define FS_APPLICATION_NAME "Fusion Engine"
#define FS_APPLICATION_WIDTH 800
#define FS_APPLICATION_HEIGHT 600
#define FS_LOCK_MOUSE_ON_FOCUS 1
#define FS_MAX_CHAR_SIZE 512

// File definitions
#define FS_RESOURCE_LOCATION "../Engine/res"

// Rendering definitions
#define FS_DO_DEPTH_TEST 1
#define FS_DRAW_WIRE_FRAME 0

// Input definitions
#define FS_MOUSE_SENS 0.1f

// Camera definitions
#define FS_FIELD_OF_VIEW 45.0f
#define FS_MAX_FOV 90.0f
#define FS_MIN_FOV 10.0f
#define FS_NEAR_PLANE 0.1f
#define FS_FAR_PLANE 1000.0f