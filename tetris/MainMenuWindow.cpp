#include "MainMenuWindow.h"

MainMenuWindow::MainMenuWindow(GLFWwindow* appWindow)
	: appWindow(appWindow)
{

}

void MainMenuWindow::HandleEscPressed()
{
	glfwSetWindowShouldClose(appWindow, true);
}
