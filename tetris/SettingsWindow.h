#ifndef SETTINGS_WINDOW
#define SETTINGS_WINDOW

#include "MenuWindow.h"
#include "MenuController.h"

class SettingsWindow : public MenuWindow
{
public:
	SettingsWindow(MenuControllerPackage::MenuController* menuController);

	void HandleEscPressed();
private:
	MenuControllerPackage::MenuController* menuController;
};

#endif // !SETTINGS_WINDOW
