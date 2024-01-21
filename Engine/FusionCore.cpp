#include "FusionCore.h" // Includes all the important stuff we need to startup

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput();

Fusion::Window window;
Fusion::Camera camera(glm::vec3(0.0f), window);
Fusion::Scene scene;

float lastMouseX;
float lastMouseY;

float lastFrame = glfwGetTime();

int main()
{
	GLFWwindow* glfwWindow = window.GetWindow();
	Fusion::CurrentWindow = glfwWindow;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();

	// Set window callbacks
	glfwSetFramebufferSizeCallback(glfwWindow, framebuffer_size_callback);
	glfwSetScrollCallback(glfwWindow, scroll_callback);
	glfwSetMouseButtonCallback(glfwWindow, mouse_button_callback);
	glfwSetCursorPosCallback(glfwWindow, mouse_callback);

	Fusion::Shader ourShader("res:/shaders/basic.vert", "res:/shaders/basic.frag");
	Fusion::Model ourMeshModel("res:/models/backpack/backpack.obj", "backpack", ourShader);
	Fusion::Object object1(ourMeshModel, "Object1");
	Fusion::Object object2(ourMeshModel, "Object2");

	scene.PushObject(&object1);
	scene.PushObject(&object2);
	
	object1.position = glm::vec3(0, 5, 0);
	object1.scale = glm::vec3(0.5f);

	object2.position = glm::vec3(5, 5, 0);
	object2.scale = glm::vec3(1.5f);

	while (!glfwWindowShouldClose(glfwWindow))
	{ 
		glfwPollEvents();

		// ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
#if FS_SHOW_IMGUI_DEMO
		ImGui::ShowDemoWindow(); // Show demo window! :)
#endif

		Fusion::Debug::DrawImGui(scene);

		object2.rotation = glm::vec3(0.0f, glm::radians(sin(glfwGetTime()) * 15.0f), 0.0f);

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

		ourShader.use();

		// view/proj transformations
		glm::mat4 proj = camera.GetCameraMat4(window);
		glm::mat4 view = camera.GetViewMat4();
		ourShader.setMat4("projection", proj);
		ourShader.setMat4("view", view);

		// render scene
		scene.Draw(camera);

		// ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Check and call events and swap the buffers
		glfwSwapBuffers(glfwWindow);

		// Call NewFrame
		//window.NewFrame(); 
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(glfwWindow);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* _, int width, int height)
{
	window.SetSize(width, height);
}

void scroll_callback(GLFWwindow* glfwWindow, double xOffset, double yOffset)
{
	ImGui_ImplGlfw_ScrollCallback(glfwWindow, xOffset, yOffset);

	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse) {
		return;
	}

	camera.ProcessScroll(window, yOffset);
}

void mouse_callback(GLFWwindow* glfwWindow, double xpos, double ypos)
{
	if (camera.IsMouseLocked())
	{
		ImGui_ImplGlfw_CursorPosCallback(glfwWindow, -1, -1);
	}
	else
		ImGui_ImplGlfw_CursorPosCallback(glfwWindow, xpos, ypos);

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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse) {
		return;
	}
}