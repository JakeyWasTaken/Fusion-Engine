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

	// test file loading
	Fusion::Shader testShader("res:/shaders/lighting.vert", "res:/shaders/lighting.frag");
	Fusion::Texture testTexture("res:/images/container2.png", false, GL_TEXTURE0);

	while (!glfwWindowShouldClose(glfwWindow))
	{ 
		// Process input
		processInput();

		// Update Delta Time
		float currentFrame = glfwGetTime();
		Fusion::DeltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Call NewFrame
		window.NewFrame(); 
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