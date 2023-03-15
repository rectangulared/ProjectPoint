#include "WorldManager.h"

#include "utils/FileReadUtils.hpp"
#include "render/entities/framebuffer/Framebuffer.h"

float randf()
{
	return -1.0f + (rand() / (RAND_MAX / 2.0f));
}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = 0;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
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

		instanceMatrix.push_back(trans * rot * sca);
	}

	//cubeModel = Model("./models/Cube/cube.obj", instanceMatrix, 10000);

	lightManager = LightManager();
	lightManager.addPointLight(PointLight());
	lightManagerUi.setup(&lightManager);
	camera = Camera(glm::vec3(0.0f, 2.0f, 10.0f));
	//objectManager.addObject(new Object(cubeModel, glm::mat4(1.0f), true));
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
	//ShaderProgram test = ShaderProgram(readTextFromFile("./shaders/default.vert"), readTextFromFile("./shaders/postEffect.frag"));
	//ShaderProgram test2 = ShaderProgram(readTextFromFile("./shaders/default.vert"), readTextFromFile("./shaders/postEffectNegative.frag"));

	std::vector<std::string> faces
	{
		"models/skybox/right.jpg",
		"models/skybox/left.jpg",
		"models/skybox/top.jpg",
		"models/skybox/bottom.jpg",
		"models/skybox/front.jpg",
		"models/skybox/back.jpg"
	};

	GLuint cubemapTexture = loadCubemap(faces);


	Framebuffer fbo;
	fbo.bind();

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	fbo.unbind();

	Framebuffer fbo1;
	fbo1.bind();

	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture1, 0);

	fbo1.unbind();

	Model sponza = Model("./models/Sponza/sponza.obj");
	objectManager.addObject(new Object(sponza, glm::mat4(1.0f)));
	objectManager.objects[0]->Scale(glm::vec3(0.01f, 0.01f, 0.01f));

	GLuint uniformBlockIndexMatrices = glGetUniformBlockIndex(mainShader.getProgramID(), "Matrices");
	GLuint uniformBlockIndexMatricesInstansing = glGetUniformBlockIndex(instancingShader.getProgramID(), "Matrices");
	glUniformBlockBinding(mainShader.getProgramID(), uniformBlockIndexMatrices, 1);
	glUniformBlockBinding(instancingShader.getProgramID(), uniformBlockIndexMatricesInstansing, 1);

	GLuint uniformBlockIndexDirectionalLight = glGetUniformBlockIndex(mainShader.getProgramID(), "DirectionalLight");
	GLuint uniformBlockIndexDirectionalLightInstansing = glGetUniformBlockIndex(instancingShader.getProgramID(), "DirectionalLight");
	glUniformBlockBinding(mainShader.getProgramID(), uniformBlockIndexDirectionalLight, 2);
	glUniformBlockBinding(instancingShader.getProgramID(), uniformBlockIndexDirectionalLightInstansing, 2);

	GLuint uboProjectionView;
	glGenBuffers(1, &uboProjectionView);
	glBindBuffer(GL_UNIFORM_BUFFER, uboProjectionView);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	GLuint uboDirectionalLight;
	glGenBuffers(1, &uboDirectionalLight);
	glBindBuffer(GL_UNIFORM_BUFFER, uboDirectionalLight);
	//Magic number
	glBufferData(GL_UNIFORM_BUFFER, 2640, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboProjectionView);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboDirectionalLight);

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

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		
		glm::mat4 view = camera.getViewMatrix();
		glBindBuffer(GL_UNIFORM_BUFFER, uboProjectionView);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		mainShader.use();
		mainShader.setVec3f("_camPos", camera.position);
		mainShader.setFloat("material.specularStrength", 32.0f);
		instancingShader.use();
		instancingShader.setVec3f("_camPos", camera.position);
		instancingShader.setFloat("material.specularStrength", 32.0f);
		lightManager.drawLights(mainShader, uboDirectionalLight);
		lightManager.drawLights(instancingShader, uboDirectionalLight);
		objectManager.draw(camera.position, mainShader, opacityShader, instancingShader);

		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.getViewMatrix()));
		skyboxShader.setMat4f("view", view);
		skyboxShader.setMat4f("projection", projection);

		skyboxVAO.bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);

		fbo.unbind();
		//fbo1.bind();
		glDisable(GL_DEPTH_TEST);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		screenShader.use();
		screenVAO.bind();
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		screenVAO.unbind();
		//fbo1.unbind();

		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);

		//test2.use();
		//screenVAO.bind();
		//glBindTexture(GL_TEXTURE_2D, texture1);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//screenVAO.unbind();

		//TODO: Make UI manager
		if (isMenuOpen)
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			lightManagerUi.draw();
			ImGui::Image((void*)(intptr_t)texture, ImVec2(320, 240));
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
