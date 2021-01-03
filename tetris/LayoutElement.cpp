#include "LayoutElement.h"

LayoutElement::LayoutElement(MenuObject* elem, int mLeft, int mRight, int mTop, int mBottom)
	: elem(elem),
	  marginLeft(mLeft),
	  marginRight(mRight),
	  marginTop(mTop),
	  marginBottom(mBottom)
{
}

int LayoutElement::GetWidth()
{
	return elem->GetWidth();
}

int LayoutElement::GetHeight()
{
	return elem->GetHeight();
}

void LayoutElement::SetOriginX(int originX)
{
	elem->SetOriginX(originX);
}

void LayoutElement::SetOriginY(int originY)
{
	elem->SetOriginY(originY);
}

int LayoutElement::GetOriginX()
{
	return elem->GetOriginX();
}

int LayoutElement::GetOriginY()
{
	return elem->GetOriginY();
}

void LayoutElement::Render()
{
	elem->Render();
}