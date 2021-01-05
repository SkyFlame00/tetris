#ifndef MENU_CONTROLLER_H
#define MENU_CONTROLLER_H

#include "MenuWindow.h"
#include <string>
#include <map>

namespace MenuControllerPackage
{

	struct UpdateData
	{
		float mouseX, mouseY;
		bool mousePressed, mouseReleased, escPressed;
	};

	class MenuController
	{
	public:
		MenuController();
		~MenuController();

		//void SetActive(std::string alias);
		void AddWindow(MenuWindow* window, std::string alias);
		void Update(UpdateData* data);
		void Open(std::string alias);
		void GoBack();
		void Render();
	private:
		std::string active;
		std::map<std::string, MenuWindow*> windows;
		std::map<std::string, std::string> parentWindows;
	};

}

#endif // !MENU_CONTROLLER_H