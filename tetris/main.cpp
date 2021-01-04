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


	/*-----------------------------------------------------------------*/
	Shader ourShader("shaders/sprite.vert.glsl", "shaders/sprite.frag.glsl");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f,      1.0f, 1.0f, // top right
		 0.5f, -0.5f,      1.0f, 0.0f, // bottom right
		 -0.5f,  0.5f,      0.0f, 1.0f,  // top left 

		 0.5f, -0.5f,      1.0f, 0.0f, // bottom right
		-0.5f, -0.5f,      0.0f, 0.0f, // bottom left
		-0.5f,  0.5f,      0.0f, 1.0f  // top left 
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// load and create a texture 
	// -------------------------
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load("textures/wall.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

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

		//// bind Texture
		//glBindTexture(GL_TEXTURE_2D, texture);

		//// render container
		//ourShader.use();
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);

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