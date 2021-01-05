#ifndef LAYOUT_CONTAINER_H
#define LAYOUT_CONTAINER_H

#include "LayoutElement.h"
#include "MenuObject.h"
#include <list>
#include "LayoutEntity.h"

enum class e_Alignment
{
	LEFT,
	CENTER,
	RIGHT
};

class LayoutContainer : public LayoutEntity
{
public:
	LayoutContainer();
	~LayoutContainer();

	void AddElement(LayoutEntity* entity);
	void SetAlignment(e_Alignment alignment);
	int GetWidth();
	int GetHeight();
	int GetOriginX();
	int GetOriginY();
	void SetOriginX(int originX);
	void SetOriginY(int originY);

	e_Alignment alignment;
	int marginLeft, marginRight, marginTop, marginBottom;
private:
	int originX, originY, nextY, maxElemWidth;
	std::list<LayoutEntity*> entities;
	MenuObject* capturedObject;
};

#endif // !LAYOUT_CONTAINER_H