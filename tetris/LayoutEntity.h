#ifndef LAYOUT_ENTITY_H
#define LAYOUT_ENTITY_H

class LayoutEntity
{
public:
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual int GetOriginX() = 0;
	virtual int GetOriginY() = 0;
	virtual void SetOriginX(int originX) = 0;
	virtual void SetOriginY(int originY) = 0;
	int marginLeft, marginRight, marginTop, marginBottom;
};

#endif // !LAYOUT_ENTITY_H
