#pragma once
#include "FusionCore.h"

namespace Fusion
{
	class Window
	{
	public:
		Window() {
			if (!glfwInit())
			{
				printf("[WINDOW] Failed to init GLFW\n");
				return;
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, FS_GL_VERSION_MAJOR);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, FS_GL_VERSION_MINOR);
			glfwWindowHint(GLFW_OPENGL_PROFILE, FS_GL_PROFILE);
#if FS_DO_MULTISAMPLING
			glfwWindowHint(GLFW_SAMPLES, FS_MULTISAMPLING_SAMPLES);
#endif

			m_window = glfwCreateWindow(m_width, m_height, FS_APPLICATION_NAME ,NULL,NULL);
			if (m_window == NULL)
			{
				printf("[WINDOW] Failed to create GLFW window\n");
				glfwTerminate();
				return;
			}

			glfwMakeContextCurrent(m_window);
			glfwSwapInterval(FS_VSYNC_ENABLED);

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				printf("[WINDOW] Failed to initialize GLAD\n");
				glfwTerminate();
				return;
			}

			glViewport(0, 0, m_width, m_height);

			int maxVertexAttributes;
			glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttributes);

			printf("[WINDOW] OpenGL Version: %s\n", glGetString(GL_VERSION));
			printf("[WINDOW] Max Shader Vertex Attributes: %d\n", maxVertexAttributes);
#if FS_DO_MULTISAMPLING
			printf("[WINDOW] Multisampling enabled at %d samples\n", FS_MULTISAMPLING_SAMPLES);
#endif
		}

		void NewFrame()
		{
			// Rendering commands here
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

#if FS_DO_DEPTH_TEST
			glEnable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
#if FS_DO_MULTISAMPLING
			glEnable(GL_MULTISAMPLE);
#endif
#if FS_DRAW_WIRE_FRAME
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

			// Check and call events and swap the buffers
			glfwSwapBuffers(m_window);
			glfwPollEvents();
		}

		GLFWwindow* GetWindow() const { return m_window; }

		void SetSize(int width, int height)
		{
			m_width = width;
			m_height = height;
			glViewport(0, 0, width, height);
		}

		int GetWidth() const { return m_width; }
		int GetHeight() const { return m_height; }

	private:
		int m_width = FS_APPLICATION_WIDTH;
		int m_height = FS_APPLICATION_HEIGHT;
		GLFWwindow* m_window = NULL;
	};
}