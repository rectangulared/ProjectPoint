#include "WorldManager.h"

#include "utils/FileReadUtils.hpp"
#include "render/entities/framebuffer/Framebuffer.h"
#include <render/entities/object/model/texture/Cubemap.h>
#include <render/entities/renderbuffer/Renderbuffer.h>

float randf()
{
	return -1.0f + (rand() / (RAND_MAX / 2.0f));
}

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
	glfwSwapInterval(0);

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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mainShader = ShaderProgram(readTextFromFile("./shaders/defaultLighting.vert"), readTextFromFile("./shaders/defaultLighting.frag"));
	opacityShader = ShaderProgram(readTextFromFile("./shaders/LightingOpacity.vert"), readTextFromFile("./shaders/LightingOpacity.frag"));

	std::vector <glm::mat4> instanceMatrix;

	for (unsigned int i = 0; i < 10000; i++)
	{
		float x = randf();
		float finalRadius = randf() * 25;
		float y = ((rand() % 2) * 2 - 1) * sqrt(1.0f - x * x);

		glm::vec3 tempTranslation;
		glm::quat tempRotation;
		glm::vec3 tempScale;

		if (randf() > 0.5f)
		{
			tempTranslation = glm::vec3(y * finalRadius, randf(), x * finalRadius);
		}
		else
		{
			tempTranslation = glm::vec3(x * finalRadius, randf(), y * finalRadius);
		}

		tempRotation = glm::quat(1.0f, randf(), randf(), randf());
		tempScale = 0.1f * glm::vec3(randf(), randf(), randf());

		glm::mat4 trans = glm::mat4(1.0f);
		glm::mat4 rot = glm::mat4(1.0f);
		glm::mat4 sca = glm::mat4(1.0f);

		trans = glm::translate(trans, tempTranslation);
		rot = glm::mat4_cast(tempRotation);
		sca = glm::scale(sca, tempScale);

		instanceMatrix.push_back(trans);
	}

	cubeModel = Model("./models/Cube/cube.obj", instanceMatrix, 100000);

	lightManager = LightManager();
	lightManager.addPointLight(PointLight());
	lightManagerUi.setup(&lightManager);
	camera = Camera(glm::vec3(0.0f, 2.0f, 10.0f));
	objectManager.addObject(new Object(cubeModel, glm::mat4(1.0f), true));
}

void WorldManager::update()
{
	std::vector<Vertex> screenVertices = {
		Vertex(glm::vec3(-1.0f,  1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f))
	};

	VAO screenVAO;
	VBO screenVBO(screenVertices, GL_STATIC_DRAW);

	screenVAO.bind();
	screenVBO.bind();

	screenVAO.linkAttrib(screenVBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	screenVAO.linkAttrib(screenVBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, textureUV));

	screenVAO.unbind();
	screenVBO.unbind();

	std::vector<Vertex> skyboxVertices = {
		Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f))
	};

	VAO skyboxVAO;
	VBO skyboxVBO(skyboxVertices, GL_STATIC_DRAW);

	skyboxVAO.bind();
	skyboxVBO.bind();

	skyboxVAO.linkAttrib(skyboxVBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	skyboxVAO.linkAttrib(skyboxVBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, textureUV));

	skyboxVAO.unbind();
	skyboxVBO.unbind();

	ShaderProgram instancingShader = ShaderProgram(readTextFromFile("./shaders/instancing.vert"), readTextFromFile("./shaders/defaultLighting.frag"));
	ShaderProgram screenShader = ShaderProgram(readTextFromFile("./shaders/default.vert"), readTextFromFile("./shaders/default.frag"));
	ShaderProgram skyboxShader = ShaderProgram(readTextFromFile("./shaders/skybox.vert"), readTextFromFile("./shaders/skybox.frag"));

	std::vector<std::string> faces
	{
		"models/skybox/right.jpg",
		"models/skybox/left.jpg",
		"models/skybox/top.jpg",
		"models/skybox/bottom.jpg",
		"models/skybox/front.jpg",
		"models/skybox/back.jpg"
	};

	Cubemap cubemapTexture = Cubemap(faces);

	Framebuffer fbo;
	fbo.bind();

	Texture framebufferTexture = Texture(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB);
	framebufferTexture.attachToCurrentFramebuffer(GL_COLOR_ATTACHMENT0);

	Renderbuffer rbo = Renderbuffer(GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, SCREEN_WIDTH, SCREEN_HEIGHT);
	rbo.bind();

	fbo.unbind();

	stbi_set_flip_vertically_on_load(true);
	Model sponza = Model("./models/Sponza/sponza.obj");
	stbi_set_flip_vertically_on_load(false);
	objectManager.addObject(new Object(sponza, glm::mat4(1.0f)));
	objectManager.objects[1]->Scale(glm::vec3(0.01f, 0.01f, 0.01f));

	GLuint uniformBlockIndexMatrices = glGetUniformBlockIndex(mainShader.getProgramID(), "Matrices");
	GLuint uniformBlockIndexMatricesInstansing = glGetUniformBlockIndex(instancingShader.getProgramID(), "Matrices");
	glUniformBlockBinding(mainShader.getProgramID(), uniformBlockIndexMatrices, 1);
	glUniformBlockBinding(instancingShader.getProgramID(), uniformBlockIndexMatricesInstansing, 1);

	GLuint uniformBlockIndexDirectionalLight = glGetUniformBlockIndex(mainShader.getProgramID(), "DirectionalLight");
	GLuint uniformBlockIndexDirectionalLightInstansing = glGetUniformBlockIndex(instancingShader.getProgramID(), "DirectionalLight");
	glUniformBlockBinding(mainShader.getProgramID(), uniformBlockIndexDirectionalLight, 2);
	glUniformBlockBinding(instancingShader.getProgramID(), uniformBlockIndexDirectionalLightInstansing, 2);

	GLuint uniformBlockIndexPointLights = glGetUniformBlockIndex(mainShader.getProgramID(), "PointLights");
	GLuint uniformBlockIndexPointLightsInstansing = glGetUniformBlockIndex(instancingShader.getProgramID(), "PointLights");
	glUniformBlockBinding(mainShader.getProgramID(), uniformBlockIndexPointLights, 3);
	glUniformBlockBinding(instancingShader.getProgramID(), uniformBlockIndexPointLightsInstansing, 3);

	GLuint uniformBlockIndexSpotLights = glGetUniformBlockIndex(mainShader.getProgramID(), "SpotLights");
	GLuint uniformBlockIndexSpotLightsInstansing = glGetUniformBlockIndex(instancingShader.getProgramID(), "SpotLights");
	glUniformBlockBinding(mainShader.getProgramID(), uniformBlockIndexSpotLights, 4);
	glUniformBlockBinding(instancingShader.getProgramID(), uniformBlockIndexSpotLights, 4);

	GLuint uboProjectionView;
	glGenBuffers(1, &uboProjectionView);
	glBindBuffer(GL_UNIFORM_BUFFER, uboProjectionView);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	GLuint uboDirectionalLight;
	glGenBuffers(1, &uboDirectionalLight);
	glBindBuffer(GL_UNIFORM_BUFFER, uboDirectionalLight);
	glBufferData(GL_UNIFORM_BUFFER, 80, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	GLuint uboPointLights;
	glGenBuffers(1, &uboPointLights);
	glBindBuffer(GL_UNIFORM_BUFFER, uboPointLights);
	glBufferData(GL_UNIFORM_BUFFER, 2576, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	GLuint uboSpotLights;
	glGenBuffers(1, &uboSpotLights);
	glBindBuffer(GL_UNIFORM_BUFFER, uboSpotLights);
	glBufferData(GL_UNIFORM_BUFFER, 3088, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboProjectionView);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboDirectionalLight);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, uboPointLights);
	glBindBufferBase(GL_UNIFORM_BUFFER, 4, uboSpotLights);

	glm::mat4 projection = glm::perspective(glm::radians(camera.fov), static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 100.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uboProjectionView);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	while (!glfwWindowShouldClose(window))
	{
		//TODO: Make separate render manager
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput();

		fbo.bind();
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		
		glm::mat4 view = camera.getViewMatrix();
		glBindBuffer(GL_UNIFORM_BUFFER, uboProjectionView);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);


		instancingShader.use();
		instancingShader.setVec3f("_camPos", camera.position);
		instancingShader.setFloat("material.specularStrength", 32.0f);
		cubemapTexture.bind();
		lightManager.drawLights(mainShader, uboDirectionalLight, uboPointLights, uboSpotLights);
		lightManager.drawLights(instancingShader, uboDirectionalLight, uboPointLights, uboSpotLights);
		mainShader.use();
		cubemapTexture.bind();
		mainShader.setVec3f("_camPos", camera.position);
		mainShader.setFloat("material.specularStrength", 32.0f);
		mainShader.setMat4f("view", view);
		mainShader.setMat4f("projection", projection);
		objectManager.draw(camera.position, mainShader, opacityShader, instancingShader);

		glDepthFunc(GL_LEQUAL);
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.getViewMatrix()));
		skyboxShader.setMat4f("view", view);
		skyboxShader.setMat4f("projection", projection);

		skyboxVAO.bind();
		cubemapTexture.bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);
		fbo.unbind();

		glDisable(GL_DEPTH_TEST);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		screenShader.use();
		screenVAO.bind();
		framebufferTexture.bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		screenVAO.unbind();

		if (isMenuOpen)
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			lightManagerUi.draw();
			ImGui::Image((void*)(intptr_t)framebufferTexture._id, ImVec2(320, 240));
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
