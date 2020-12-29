#include <iostream>
#include <cstdlib>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "WindowManager.h"
#include "Game.h"
#include "data_structures/misc.h"

void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseMoved(GLFWwindow* window, double xpos, double ypos);
void mousePressed(GLFWwindow* window, int button, int action, int mods);
void resetUserInput(UserInput* userInput);
void cleanup();

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;
const int GAME_XUNITS = 800;
const int GAME_YUNITS = 600;

int main()
{
	/* Seed random number generator */
	std::srand(std::time(nullptr));

	WindowManager windowManager(3, 3);
	
	try {
		windowManager.init(SCR_WIDTH, SCR_HEIGHT, "Tetris");
	}
	catch (const char *str) {
		std::cout << str << std::endl;
		return 1;
	}

	GLFWwindow* window = windowManager.GetWindow();

	Game game(window, GAME_XUNITS, GAME_YUNITS);

	glm::vec4 color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	int clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	UserInput* userInput = new UserInput;
	glfwSetWindowUserPointer(window, userInput);
	glfwSetCursorPosCallback(window, mouseMoved);
	glfwSetKeyCallback(window, keyPressed);
	glfwSetMouseButtonCallback(window, mousePressed);

	/* Time delta */
	float lastFrame = (float)glfwGetTime();

	Shader shader2("shaders/test.vert.glsl", "shaders/test.frag.glsl");
	GLuint boxVAO, boxVBO;
	float vertices[] = {
		0.130000, -0.223333, 0.000000, 16.000000,
		0.130000, -0.166667, 0.000000, -1.000000,
		0.147500, -0.166667, 0.000000, -8.000000,
		0.130000, -0.223333, 0.000000, 16.000000,
		0.147500, -0.166667, 0.000000, -8.000000,
		0.147500, -0.223333, 0.000000, 9.000000
	};
	float vertices2[] = {
		0.130000, 0.223333, 0.000000, -118.000000,
		0.130000, 0.166667, 0.000000, -101.000000,
		0.147500, 0.166667, 0.000000, -108.000000,
		0.130000, 0.223333, 0.000000, -118.000000,
		0.147500, 0.166667, 0.000000, -108.000000,
		0.147500, 0.223333, 0.000000, -125.000000
	};
	float vertices3[] = {
		0.130000, -0.223333, 0.000000, 0,
		0.130000, -0.166667, 0.000000, -0,
		0.147500, -0.166667, 0.000000, -0,
	};
	glGenVertexArrays(1, &boxVAO);
	glGenBuffers(1, &boxVBO);

	glBindVertexArray(boxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, boxVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), &vertices3, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/* Game loop */
	while (!windowManager.ShouldClose())
	{	
		float now = glfwGetTime();
		float deltaTime = now - lastFrame;
		lastFrame = now;

		glClearColor(color.x, color.y, color.z, color.w);
		glClear(clearMask);

		

		game.Update(deltaTime, userInput);
		game.Render();

		shader2.use();
		glBindVertexArray(boxVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		resetUserInput(userInput);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete userInput;
	cleanup();
	return 0;
}

void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	UserInput* userInput = static_cast<UserInput*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	
	if (key == GLFW_KEY_LEFT)
	{
		userInput->leftArrowPressed = action == GLFW_PRESS;
	}

	if (key == GLFW_KEY_RIGHT)
	{
		userInput->rightArrowPressed = action == GLFW_PRESS;
	}

	if (key == GLFW_KEY_UP)
	{
		userInput->upArrowPressed = action == GLFW_PRESS;
	}

	if (key == GLFW_KEY_DOWN)
	{
		userInput->downArrowPressed = action == GLFW_PRESS;
	}

	if (key == GLFW_KEY_SPACE)
	{
		userInput->spacePressed = action == GLFW_PRESS;
	}
}

void mouseMoved(GLFWwindow* window, double xpos, double ypos)
{
	UserInput* userInput = static_cast<UserInput*>(glfwGetWindowUserPointer(window));
	userInput->mouseX = xpos;
	userInput->mouseY = ypos;
	//std::cout << "x: " << game->mouseX << ", y = " << ypos << std::endl;
}

void mousePressed(GLFWwindow* window, int button, int action, int mods)
{
	UserInput* userInput = static_cast<UserInput*>(glfwGetWindowUserPointer(window));

	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		userInput->mousePressed = action == GLFW_PRESS;
		userInput->mouseReleased = action == GLFW_RELEASE;
	}
}

void resetUserInput(UserInput* userInput)
{
	userInput->mousePressed = false;
	userInput->mouseReleased = false;
	userInput->escPressed = false;
	userInput->escReleased = false;
	userInput->leftArrowPressed = false;
	userInput->leftArrowReleased = false;
	userInput->rightArrowPressed= false;
	userInput->rightArrowReleased = false;
	userInput->upArrowPressed = false;
	userInput->upArrowReleased = false;
	userInput->downArrowPressed = false;
	userInput->downArrowReleased = false;
	userInput->spacePressed = false;
	userInput->spaceReleased = false;
}

void cleanup()
{
	glfwTerminate();
}