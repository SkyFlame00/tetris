#ifndef MAIN_MENU_WINDOW_H
#define MAIN_MENU_WINDOW_H

#include "MenuWindow.h"

class MainMenuWindow : public MenuWindow
{
public:
	MainMenuWindow(GLFWwindow* appWindow);

	void HandleEscPressed();
private:
	GLFWwindow* appWindow;
};

#endif // !MAIN_MENU_WINDOW_H
