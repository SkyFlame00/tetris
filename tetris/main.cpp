#include <iostream>
#include <cstdlib>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "WindowManager.h"
#include "Game.h"
#include "data_structures/misc.h"

#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image.h"

void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseMoved(GLFWwindow* window, double xpos, double ypos);
void mousePressed(GLFWwindow* window, int button, int action, int mods);
void resetUserInput(UserInput* userInput);
void cleanup();

int main()
{
	const int SCR_WIDTH = 800;
	const int SCR_HEIGHT = 600;
	const int GAME_XUNITS = 800;
	const int GAME_YUNITS = 600;

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

	stbi_set_flip_vertically_on_load(true);
	Game game(window, GAME_XUNITS, GAME_YUNITS);

	glm::vec4 color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	int clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	
	UserInput* userInput = new UserInput;
	glfwSetWindowUserPointer(window, userInput);
	glfwSetCursorPosCallback(window, mouseMoved);
	glfwSetKeyCallback(window, keyPressed);
	glfwSetMouseButtonCallback(window, mousePressed);

	/* Time delta */
	float lastFrame = (float)glfwGetTime();

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