#include <iostream>
#include "MenuWindow.h"
#include "MenuObject.h"

MenuWindow::MenuWindow()
	: mouseX(0),
	  mouseY(0),
	  xdt(0),
	  ydt(0),
	  pressedObject(nullptr)
{
	objects = new List<MenuObject*>;
}

MenuWindow::~MenuWindow()
{
	delete objects;
}

void MenuWindow::Render()
{
	if (objects->length > 0)
	{
		for (auto item = objects->head; item; item = item->next)
		{
			item->val->Render();
		}
	}
}

void MenuWindow::AddObject(MenuObject* object)
{
	objects->Push(object);
}

void MenuWindow::HandleHover()
{
	if (objects->length < 1)
	{
		return;
	}

	MenuObject* hoveredObject = nullptr;

	for (auto item = objects->head; item; item = item->next)
	{
		MenuObject* object = item->val;

		if (object->OverlapsPoint(mouseX, mouseY))
		{
			hoveredObject = object;
			break;
		}
	}

	if (hoveredObject)
	{
		if (this->hoveredObject && this->hoveredObject != hoveredObject)
		{
			this->hoveredObject->HandleHoverLost();
		}

		hoveredObject->HandleHover(mouseX, mouseY);
		this->hoveredObject = hoveredObject;
	}
	else
	{
		if (this->hoveredObject)
		{
			this->hoveredObject->HandleHoverLost();
			this->hoveredObject = nullptr;
		}
	}
}

void MenuWindow::HandleLeftMousePressed()
{
	if (objects->length < 1)
	{
		return;
	}

	for (auto item = objects->head; item; item = item->next)
	{
		MenuObject* object = item->val;

		if (object->OverlapsPoint(mouseX, mouseY))
		{
			pressedObject = object;
			object->HandleLeftMousePressed(mouseX, mouseY);
			break;
		}
	}

	if (focusedObject && pressedObject != focusedObject)
	{
		focusedObject->HandleFocusLost();
		focusedObject = nullptr;
	}
}

void MenuWindow::HandleLeftMouseReleased()
{
	if (pressedObject == nullptr)
	{
		return;
	}

	pressedObject->HandleLeftMouseReleased(pressedObject->OverlapsPoint(mouseX, mouseY), mouseX, mouseY);
	focusedObject = pressedObject;
	pressedObject = nullptr;
}

void MenuWindow::HandleEscPressed()
{
}

void MenuWindow::SetMouseX(double newX)
{
	xdt = newX - mouseX;
	mouseX = newX;
}

void MenuWindow::SetMouseY(double newY)
{
	ydt = newY - mouseY;
	mouseY = newY;
}
