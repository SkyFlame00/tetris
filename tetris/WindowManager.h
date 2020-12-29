#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class WindowManager
{
public:
	WindowManager(int majorVersion, int minorVersion);
	~WindowManager();

	void init(int width, int height, const char* title);
	void registerCallback(GLFWframebuffersizefun callback);
	bool ShouldClose();
	GLFWwindow* GetWindow();
private:
	GLFWwindow* window{};
	int clearMask;
};

#endif