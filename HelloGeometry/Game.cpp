#include "Game.h"
#include "UI_Manager.h"

Game::Game(GLFWwindow* window) : //ui_Manager(window),
			   modeltest("model/newell_teaset/teapot.obj"),
			   modelShader("Shader/model_loading.vs", "Shader/model_loading.fs", "Shader/explode.gs"), 
			   testShader("Shader/test.vs", "Shader/test.fs", "Shader/test.gs"),
			   camera(glm::vec3(0.0f, 0.0f, 3.0f))
{

	_window = window;
	float poins[] = {
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
	};

	float transparentVertices[] = {
		-0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.0f,

		 0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f, 1.0f, 0.0f,
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(poins), &poins, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

}

Game::~Game()
{
}

void Game::Update(float delta)
{
	processInput(_window, delta);

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)Setting::SCR_WIDTH / (float)Setting::SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	//==================================
	modelShader.use();
	modelShader.setMat4("projection", projection);
	modelShader.setMat4("view", view);
	modelShader.setVec3("Color", Color);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians((float)glfwGetTime() * 10), RotatDir);
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	
	modelShader.setMat4("model", model);
	modelShader.setFloat("time", glfwGetTime() * Speed);
	//modelShader.setFloat("time", glfwGetTime());
	//==================================
	
}

void Game::Draw()
{
	//==================================
	glBindVertexArray(VAO);
	testShader.use();
	glDrawArrays(GL_POINTS, 0, 4);
	//==================================
	modelShader.use();
	modeltest.Draw(modelShader);
	//==================================

}

void Game::processInput(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}