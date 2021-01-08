#ifndef MENU_WINDOW_H
#define MENU_WINDOW_H

#include "data_structures/List.h"
#include "MenuObject.h"
#include "Button.h"

class MenuWindow
{
public:
	MenuWindow();
	~MenuWindow();

	//void CheckHover(Cursor* cursor);
	void AddObject(MenuObject* object);
	void Render();
	void SetMouseX(double newX);
	void SetMouseY(double newY);
	void HandleLeftMousePressed();
	void HandleLeftMouseReleased();
	void HandleHover();
	virtual void HandleEscPressed();

	double mouseX;
	double mouseY;
	double xdt;
	double ydt;
private:
	List<MenuObject*>* objects;
	MenuObject *pressedObject, *focusedObject, *hoveredObject;
};

#endif // !GAME_WINDOW_H
