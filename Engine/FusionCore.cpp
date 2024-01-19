#include "FusionCore.h" // Includes all the important stuff we need to startup

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput();

Fusion::Window window;
Fusion::Camera camera(glm::vec3(0.0f));

float lastMouseX;
float lastMouseY;

float lastFrame = glfwGetTime();

int main()
{
	GLFWwindow* glfwWindow = window.GetWindow();

	// Set window callbacks
	glfwSetFramebufferSizeCallback(glfwWindow, framebuffer_size_callback);
	glfwSetScrollCallback(glfwWindow, scroll_callback);
	glfwSetCursorPosCallback(glfwWindow, mouse_callback);

	Fusion::Shader ourShader("res:/shaders/1.model_loading.vert", "res:/shaders/1.model_loading.frag");
	Fusion::Model ourModel("res:/models/backpack/backpack.obj");

	while (!glfwWindowShouldClose(glfwWindow))
	{ 
		// Process input
		processInput(); 

		// Update Delta Time
		float currentFrame = glfwGetTime();
		Fusion::DeltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

#if FS_DO_DEPTH_TEST
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
#if FS_DRAW_WIRE_FRAME
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

		ourShader.use();

		// view/proj transformations
		glm::mat4 proj = camera.GetProjMat4(window);
		glm::mat4 view = camera.GetViewMat4();
		ourShader.setMat4("projection", proj);
		ourShader.setMat4("view", view);

		// render loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);

		// Check and call events and swap the buffers
		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();

		// Call NewFrame
		//window.NewFrame(); 
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* _, int width, int height)
{
	window.SetSize(width, height);
}

void scroll_callback(GLFWwindow* _, double xOffset, double yOffset)
{
	camera.ProcessScroll(window, yOffset);
}

void mouse_callback(GLFWwindow* _, double xpos, double ypos)
{
	float deltaX = xpos - lastMouseX;
	float deltaY = lastMouseY - ypos; // reversed since y-coordinates range from bottom to top
	lastMouseX = xpos;
	lastMouseY = ypos;

	deltaX *= FS_MOUSE_SENS;
	deltaY *= FS_MOUSE_SENS;

	camera.ProcessMouse(deltaX, deltaY);
}

void processInput()
{
	GLFWwindow* glfwWindow = window.GetWindow();

	camera.ProcessInput(window);

	if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		printf("Application manually quit via ESC\n");
		glfwSetWindowShouldClose(glfwWindow, true);
	}
}