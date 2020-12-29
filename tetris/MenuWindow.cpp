#include <iostream>
#include "MenuWindow.h"
#include "MenuObject.h"

MenuWindow::MenuWindow()
	: mouseX(0),
	  mouseY(0),
	  xdt(0),
	  ydt(0),
	  capturedObject(nullptr)
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

//void MenuWindow::CheckHover(Cursor* cursor)
//{
//
//}

void MenuWindow::AddObject(Button* object)
{
	objects->Push(object);
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
			capturedObject = object;
			object->HandleLeftMousePressed();
			break;
		}
	}
}

void MenuWindow::HandleLeftMouseReleased()
{
	if (capturedObject == nullptr)
	{
		return;
	}

	capturedObject->HandleLeftMouseReleased(capturedObject->OverlapsPoint(mouseX, mouseY));
	capturedObject = nullptr;
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
