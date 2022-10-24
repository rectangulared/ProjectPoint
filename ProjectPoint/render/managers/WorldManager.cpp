#include "WorldManager.h"

#include "../../utils/FileReadUtils.hpp"

WorldManager* WorldManager::worldManager_ = nullptr;

WorldManager* WorldManager::getInstance()
{
	if (worldManager_ == nullptr)
		worldManager_ = new WorldManager();
	return worldManager_;
}

WorldManager::WorldManager() 
{
	window = nullptr;
};

void WorldManager::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project Point", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Creating Window Error" << std::endl;
		glfwTerminate();
		return;
	}

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	gladLoadGL();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	mainShader = ShaderProgram(readTextFromFile("./shaders/defaultLighting.vert"), readTextFromFile("./shaders/defaultLighting.frag"));
	opacityShader = ShaderProgram(readTextFromFile("./shaders/LightingOpacity.vert"), readTextFromFile("./shaders/LightingOpacity.frag"));
	cubeModel = Model("./models/Cube/cube.obj");

	lightManager = LightManager();
	lightManager.addPointLight(PointLight());
	lightManagerUi.setup(&lightManager);
	camera = Camera(glm::vec3(0.0f, 2.0f, 20.0f));
	objectManager.addObject(new Object(cubeModel, glm::mat4(1.0f)));
}

void WorldManager::update()
{
	while (!glfwWindowShouldClose(window))
	{
		//TODO: Make separate render manager
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();

		mainShader.use();
		mainShader.setVec3f("_camPos", camera.position);
		mainShader.setFloat("material.specularStrength", 32.0f);
		mainShader.setMat4f("projection", projection);
		mainShader.setMat4f("view", view);
		
		opacityShader.use();
		opacityShader.setVec3f("_camPos", camera.position);
		opacityShader.setMat4f("view", view);
		opacityShader.setMat4f("projection", projection);
		opacityShader.setFloat("material.specularStrength", 32.0f);

		lightManager.drawLights(mainShader);
		objectManager.draw(camera.position, mainShader, opacityShader);

		//TODO: Make UI manager
		if (isMenuOpen)
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			lightManagerUi.draw();
			ImGui::Render();
			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void WorldManager::clear()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void WorldManager::processInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (!isMenuOpen)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.processKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.processKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.processKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.processKeyboard(RIGHT, deltaTime);
	}
}
