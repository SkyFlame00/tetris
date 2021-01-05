#ifndef LAYOUT_ELEMENT_H
#define LAYOUT_ELEMENT_H

#include "MenuObject.h"
#include "LayoutEntity.h"

class LayoutElement : public LayoutEntity
{
public:
	LayoutElement(MenuObject* elem, int mLeft, int mRight, int mTop, int mBottom);
	~LayoutElement();

	int GetWidth();
	int GetHeight();
	void SetOriginX(int originX);
	void SetOriginY(int originY);
	int GetOriginX();
	int GetOriginY();

	MenuObject* elem;
private:
};

#endif // !LAYOUT_ELEMENT_H