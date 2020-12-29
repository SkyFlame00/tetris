#include "WindowManager.h"

#include <iostream>
#include <string>

WindowManager::WindowManager(int openglMajorVersion, int openglMinorVersion)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openglMajorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openglMinorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

WindowManager::~WindowManager()
{
	std::cout << "INFO: Destructor called" << std::endl;
	glfwTerminate();
}

void WindowManager::init(int width, int height, const char* title)
{
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL)
	{
		throw "ERROR::GLFW: Failed to create GLFW window";
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw "ERROR::GLAD: Failed to initialize GLAD";
	}

	registerCallback([](GLFWwindow* window, int width, int height) -> void
		{
			glViewport(0, 0, width, height);
		}
	);
}

void WindowManager::registerCallback(GLFWframebuffersizefun callback)
{
	glfwSetFramebufferSizeCallback(window, callback);
}

bool WindowManager::ShouldClose()
{
	return glfwWindowShouldClose(window);
}

GLFWwindow* WindowManager::GetWindow()
{
	return window;
}