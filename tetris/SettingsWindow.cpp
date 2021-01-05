#include "SettingsWindow.h"

SettingsWindow::SettingsWindow(MenuControllerPackage::MenuController* menuController)
	: menuController(menuController)
{
}

void SettingsWindow::HandleEscPressed()
{
	menuController->GoBack();
}