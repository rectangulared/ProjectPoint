#include "WorldManager.h"

#include "utils/FileReadUtils.hpp"

WorldManager* WorldManager::_worldManager = nullptr;

WorldManager* WorldManager::getInstance()
{
	if (_worldManager == nullptr)
		_worldManager = new WorldManager();
	return _worldManager;
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

	shaderStorage.addShaderProgram("mainShader", ShaderProgram(readTextFromFile("./shaders/defaultLighting.vert"), readTextFromFile("./shaders/defaultLighting.frag")));
	shaderStorage.addShaderProgram("opacityShader", ShaderProgram(readTextFromFile("./shaders/defaultLighting.vert"), readTextFromFile("./shaders/LightingOpacity.frag")));
	shaderStorage.addShaderProgram("instancingShader", ShaderProgram(readTextFromFile("./shaders/instancing.vert"), readTextFromFile("./shaders/defaultLighting.frag")));
	shaderStorage.addShaderProgram("screenShader", ShaderProgram(readTextFromFile("./shaders/default.vert"), readTextFromFile("./shaders/default.frag")));
	shaderStorage.addShaderProgram("skyboxShader", ShaderProgram(readTextFromFile("./shaders/skybox.vert"), readTextFromFile("./shaders/skybox.frag")));
	shaderStorage.addShaderProgram("debugLightPositionShader", ShaderProgram(readTextFromFile("./shaders/debugLightCube.vert"), readTextFromFile("./shaders/debugLightCube.frag")));
	shaderStorage.addShaderProgram("shadowProgram", ShaderProgram(readTextFromFile("./shaders/depth.vert"), readTextFromFile("./shaders/depth.frag")));
	shaderStorage.addShaderProgram("shadowInstanceProgram", ShaderProgram(readTextFromFile("./shaders/depthInstance.vert"), readTextFromFile("./shaders/depth.frag")));

	lightManager = LightManager();
	lightManagerUi.setup(&lightManager);
	camera = Camera(glm::vec3(-2.0f, 0.0f, 0.0f));
}

void WorldManager::update()
{
	std::vector<Vertex> screenVertices 
	{
		Vertex(glm::vec3(-1.0f,  1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f))
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
		Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec3(0.0f))
	};

	VAO skyboxVAO;
	VBO skyboxVBO(skyboxVertices, GL_STATIC_DRAW);
	skyboxVAO.bind();
	skyboxVBO.bind();
	skyboxVAO.linkAttrib(skyboxVBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	skyboxVAO.linkAttrib(skyboxVBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, textureUV));
	skyboxVAO.unbind();
	skyboxVBO.unbind();

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

	stbi_set_flip_vertically_on_load(true);
	Model helmet = Model("./models/glTF/FlightHelmet.gltf");
	objectManager.addObject(new Object(helmet, glm::mat4(1.0f)));
	objectManager._objects[0]->scale(glm::vec3(5.0f, 5.0f, 5.0f));

	Framebuffer fbo;
	fbo.bind(GL_FRAMEBUFFER);
	Texture framebufferTexture = Texture(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA16F, 4, GL_TRUE);
	framebufferTexture.attachToCurrentFramebuffer(GL_COLOR_ATTACHMENT0);
	Renderbuffer rbo = Renderbuffer(GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, SCREEN_WIDTH, SCREEN_HEIGHT, 4);
	rbo.bind();
	fbo.unbind(GL_FRAMEBUFFER);

	Framebuffer postProcessingFBO;
	postProcessingFBO.bind(GL_FRAMEBUFFER);
	Texture postProcessingFBOTexture = Texture(SCREEN_WIDTH, SCREEN_HEIGHT, GL_SRGB, GL_RGB);
	postProcessingFBOTexture.attachToCurrentFramebuffer(GL_COLOR_ATTACHMENT0);
	postProcessingFBO.unbind(GL_FRAMEBUFFER);

	UBO uboProjectionView = UBO(2 * sizeof(glm::mat4), GL_DYNAMIC_DRAW);
	uboProjectionView.assignBindingPoint(1, shaderStorage.getShaderProgram("mainShader")->getProgramID(), "Matrices");
	uboProjectionView.assignBindingPoint(1, shaderStorage.getShaderProgram("instancingShader")->getProgramID(), "Matrices");
	uboProjectionView.assignBindingPoint(1, shaderStorage.getShaderProgram("debugLightPositionShader")->getProgramID(), "Matrices");
	uboProjectionView.bindToTargetBase(1);

	UBO uboDirectionalLight = UBO(96, GL_DYNAMIC_DRAW);
	uboDirectionalLight.assignBindingPoint(2, shaderStorage.getShaderProgram("mainShader")->getProgramID(), "DirectionalLight");
	uboDirectionalLight.assignBindingPoint(2, shaderStorage.getShaderProgram("instancingShader")->getProgramID(), "DirectionalLight");
	uboDirectionalLight.bindToTargetBase(2);

	UBO uboPointLights = UBO(2576, GL_DYNAMIC_DRAW);
	uboPointLights.assignBindingPoint(3, shaderStorage.getShaderProgram("mainShader")->getProgramID(), "PointLights");
	uboPointLights.assignBindingPoint(3, shaderStorage.getShaderProgram("instancingShader")->getProgramID(), "PointLights");
	uboPointLights.bindToTargetBase(3);

	UBO uboSpotLights = UBO(3088, GL_DYNAMIC_DRAW);
	uboSpotLights.assignBindingPoint(4, shaderStorage.getShaderProgram("mainShader")->getProgramID(), "SpotLights");
	uboSpotLights.assignBindingPoint(4, shaderStorage.getShaderProgram("instancingShader")->getProgramID(), "SpotLights");
	uboSpotLights.bindToTargetBase(4);

	glm::mat4 projection = glm::perspective(glm::radians(camera.fov), static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 100.0f);
	uboProjectionView.bind();
	uboProjectionView.updateSubsetData(glm::value_ptr(projection), sizeof(glm::mat4), 0);
	uboProjectionView.unbind();

	Framebuffer shadowFBO = Framebuffer(SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
	Texture shadowMap = Texture(SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT);
	shadowFBO.bind(GL_FRAMEBUFFER);
	shadowMap.bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	shadowMap.attachToCurrentFramebuffer(GL_DEPTH_ATTACHMENT);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	shadowFBO.unbind(GL_FRAMEBUFFER);

	glm::mat4 orthgonalProjection = glm::ortho(-7.0f, 7.0f, -7.0f, 7.0f, 1.0f, 7.0f);
	glm::mat4 lightView;
	glm::mat4 lightSpaceMatrix;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput();


		//SHADOW PASS START
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		glEnable(GL_DEPTH_TEST);

		glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
		shadowFBO.bind(GL_FRAMEBUFFER);
		glClear(GL_DEPTH_BUFFER_BIT);

		lightView = glm::lookAt(lightManager._directionalLight._position, lightManager._directionalLight._direction, glm::vec3(0.0f, 1.0f, 0.0f));
		lightSpaceMatrix = orthgonalProjection * lightView;

		shaderStorage.getShaderProgram("shadowProgram")->use();
		shaderStorage.getShaderProgram("shadowProgram")->setMat4f("lightSpaceMatrix", lightSpaceMatrix);

		shaderStorage.getShaderProgram("shadowInstanceProgram")->use();
		shaderStorage.getShaderProgram("shadowInstanceProgram")->setMat4f("lightSpaceMatrix", lightSpaceMatrix);
		
		objectManager.draw(lightManager._directionalLight._position, *shaderStorage.getShaderProgram("shadowProgram"), *shaderStorage.getShaderProgram("shadowProgram"), *shaderStorage.getShaderProgram("shadowInstanceProgram"));

		shadowFBO.unbind(GL_FRAMEBUFFER);
		
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//SHADOW PASS END

		//COLOUR PASS WITH MSAA 
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		fbo.bind(GL_FRAMEBUFFER);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		
		glm::mat4 view = camera.getViewMatrix();
		uboProjectionView.bind();
		uboProjectionView.updateSubsetData(glm::value_ptr(view), sizeof(glm::mat4), sizeof(glm::mat4));
		uboProjectionView.unbind();

		lightManager.drawLights(uboDirectionalLight, uboPointLights, uboSpotLights);
		shaderStorage.getShaderProgram("instancingShader")->use();
		shaderStorage.getShaderProgram("instancingShader")->setVec3f("camPos", camera.position);
		shaderStorage.getShaderProgram("instancingShader")->setFloat("material.specularStrength", 64.0f);
		shaderStorage.getShaderProgram("instancingShader")->setMat4f("lightSpaceMatrix", lightSpaceMatrix);
		shadowMap.assignTextureUnit(*shaderStorage.getShaderProgram("instancingShader"), "shadowMap", 3);
		shadowMap.bind();
		shaderStorage.getShaderProgram("mainShader")->use();
		shaderStorage.getShaderProgram("mainShader")->setVec3f("dirLightPos", lightManager._directionalLight._position);
		shaderStorage.getShaderProgram("mainShader")->setVec3f("camPos", camera.position);
		shaderStorage.getShaderProgram("mainShader")->setFloat("material.specularStrength", 64.0f);
		shaderStorage.getShaderProgram("mainShader")->setMat4f("lightSpaceMatrix", lightSpaceMatrix);
		shaderStorage.getShaderProgram("mainShader")->setBool("IS_SHADOW_PASS_ACTIVE", IS_SHADOW_PASS_ACTIVE);
		shaderStorage.getShaderProgram("mainShader")->setBool("IS_PHONG_SHADING_ACTIVE", IS_PHONG_SHADING_ACTIVE);
		shaderStorage.getShaderProgram("mainShader")->setBool("IS_NORMAL_MAPS_ACTIVE", IS_NORMAL_MAPS_ACTIVE);
		shaderStorage.getShaderProgram("mainShader")->setFloat("gamma", gamma);
		shadowMap.assignTextureUnit(*shaderStorage.getShaderProgram("mainShader"), "shadowMap", 3);
		shadowMap.bind();
		objectManager.draw(camera.position, *shaderStorage.getShaderProgram("mainShader"), *shaderStorage.getShaderProgram("opacityShader"), *shaderStorage.getShaderProgram("instancingShader"));

		shaderStorage.getShaderProgram("debugLightPositionShader")->use();
		for (auto& pl : lightManager._pointLights)
		{
			shaderStorage.getShaderProgram("debugLightPositionShader")->setVec3f("color", pl._diffuse);
			shaderStorage.getShaderProgram("debugLightPositionShader")->setMat4f("model", glm::scale(glm::translate(glm::mat4(1.0f), pl._position), glm::vec3(0.1f, 0.1f, 0.1f)));
			skyboxVAO.bind();
			glDrawArrays(GL_TRIANGLES, 0, skyboxVertices.size());
		}

		for (auto& sp : lightManager._spotLights)
		{
			shaderStorage.getShaderProgram("debugLightPositionShader")->setVec3f("color", sp._diffuse);
			shaderStorage.getShaderProgram("debugLightPositionShader")->setMat4f("model", glm::scale(glm::translate(glm::mat4(1.0f), sp._position), glm::vec3(0.1f, 0.1f, 0.1f)));
			skyboxVAO.bind();
			glDrawArrays(GL_TRIANGLES, 0, skyboxVertices.size());
		}

		if (IS_SKYBOX_ACTIVE)
		{

			glDepthFunc(GL_LEQUAL);
			shaderStorage.getShaderProgram("skyboxShader")->use();
			view = glm::mat4(glm::mat3(camera.getViewMatrix()));
			shaderStorage.getShaderProgram("skyboxShader")->setMat4f("view", view);
			shaderStorage.getShaderProgram("skyboxShader")->setMat4f("projection", projection);

			cubemapTexture.bind();
			skyboxVAO.bind();
			glDrawArrays(GL_TRIANGLES, 0, skyboxVertices.size());
			glDepthFunc(GL_LESS);

		}

		fbo.bind(GL_READ_FRAMEBUFFER);
		postProcessingFBO.bind(GL_DRAW_FRAMEBUFFER);
		glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//COROUR PASS WITH MSAA END


		//COLOUR TEXTURE TO PLANE TRANSFER PASS START
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		shaderStorage.getShaderProgram("screenShader")->use();
		glDisable(GL_DEPTH_TEST);
		screenVAO.bind();
		postProcessingFBOTexture.bind();
		glDrawArrays(GL_TRIANGLES, 0, screenVertices.size());
		screenVAO.unbind();
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//COLOUR TEXTURE TO PLANE TRANSFER PASS END

		if (isMenuOpen)
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			lightManagerUi.draw();
			ImGui::Begin("Render Manager");
			ImGui::Text("Shadow Map");
			ImGui::Image((void*)(intptr_t)shadowMap._id, ImVec2(320, 240));

			ImGui::Checkbox("Shadows", &IS_SHADOW_PASS_ACTIVE);
			ImGui::Checkbox("Blinn-Phong lighting", &IS_PHONG_SHADING_ACTIVE);
			ImGui::Checkbox("Normal maps", &IS_NORMAL_MAPS_ACTIVE);
			ImGui::Checkbox("Skybox", &IS_SKYBOX_ACTIVE);
			if (&IS_PHONG_SHADING_ACTIVE)
			{
				ImGui::SliderFloat("Gamma", &gamma, 0.0f, 10.0f);
			}

			ImGui::End();
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
