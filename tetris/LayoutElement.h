#ifndef LAYOUT_ELEMENT_H
#define LAYOUT_ELEMENT_H

#include "MenuObject.h"

class LayoutElement
{
public:
	LayoutElement(MenuObject* elem, int mLeft, int mRight, int mTop, int mBottom);
	~LayoutElement();

	int GetWidth();
	int GetHeight();
	void Render();
	void SetOriginX(int originX);
	void SetOriginY(int originY);
	int GetOriginX();
	int GetOriginY();

	MenuObject* elem;
	int marginLeft, marginRight, marginTop, marginBottom;
private:
};

#endif // !LAYOUT_ELEMENT_H