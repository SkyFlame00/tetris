#include "MenuController.h"

using namespace MenuControllerPackage;

MenuController::MenuController()
	: active(""),
	  windows(),
	  parentWindows()
{
}

MenuController::~MenuController()
{
}

//void MenuController::SetActive(std::string alias)
//{
//	this->active = alias;
//}

void MenuController::AddWindow(MenuWindow* window, std::string alias)
{
	windows[alias] = window;
}

void MenuController::Update(UpdateData* data)
{
	MenuWindow* window = windows[this->active];

	window->SetMouseX(data->mouseX);
	window->SetMouseY(data->mouseY);

	if (data->mousePressed)
	{
		window->HandleLeftMousePressed();
	}

	if (data->mouseReleased)
	{
		window->HandleLeftMouseReleased();
	}

	if (data->escPressed)
	{
		window->HandleEscPressed();
	}
}

void MenuController::Open(std::string alias)
{
	parentWindows[alias] = active;
	active = alias;
}

void MenuController::GoBack()
{
	std::string parent = parentWindows[active];
	parentWindows[active] = "";
	active = parent;
}

void MenuController::Render()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	windows[this->active]->Render();
}