#ifndef LAYOUT_CONTAINER_H
#define LAYOUT_CONTAINER_H

#include "LayoutElement.h"
#include "MenuObject.h"
#include <list>

enum class e_Alignment
{
	LEFT,
	CENTER,
	RIGHT
};

class LayoutContainer
{
public:
	LayoutContainer();
	~LayoutContainer();

	void AddElement(LayoutElement* elem);
	void SetAlignment(e_Alignment alignment);
	int GetWidth();
	int GetHeight();
	void SetOffsetX(int offsetX);
	void SetOffsetY(int offsetY);
	void Render();

	e_Alignment alignment;
private:
	int offsetX, offsetY, nextY, maxElemWidth;
	std::list<LayoutElement*> elements;
	MenuObject* capturedObject;
};

#endif // !LAYOUT_CONTAINER_H