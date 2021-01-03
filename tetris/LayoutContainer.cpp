#include "LayoutContainer.h"

LayoutContainer::LayoutContainer()
	: nextY(0),
	  alignment(e_Alignment::LEFT)
{
}

void LayoutContainer::AddElement(LayoutElement* elem)
{
	elements.push_back(elem);
	elem->SetOriginY(nextY + elem->marginTop);
	int width = elem->GetWidth();
	maxElemWidth = width > maxElemWidth ? width : maxElemWidth;

	switch (alignment)
	{
	case e_Alignment::CENTER:
		elem->SetOriginX(offsetX + (maxElemWidth / 2) - (elem->GetWidth() / 2));
		break;
	case e_Alignment::RIGHT:
		elem->SetOriginX(offsetX + maxElemWidth - elem->GetWidth());
		break;
	default:
	case e_Alignment::LEFT:
		elem->SetOriginX(offsetX);
		break;
	}

	nextY += elem->marginTop + elem->GetHeight() + elem->marginBottom;
}

void LayoutContainer::SetAlignment(e_Alignment alignment)
{
	std::list<LayoutElement*>::iterator iter;

	switch (alignment)
	{
	case e_Alignment::CENTER:
		for (iter = elements.begin(); iter != elements.end(); iter++)
		{
			(*iter)->SetOriginX(offsetX + (maxElemWidth / 2) - ((*iter)->GetWidth() / 2));
		}
		break;
	case e_Alignment::RIGHT:
		for (iter = elements.begin(); iter != elements.end(); iter++)
		{
			(*iter)->SetOriginX(offsetX + maxElemWidth - (*iter)->GetWidth());
		}
		break;
	default:
	case e_Alignment::LEFT:
		for (iter = elements.begin(); iter != elements.end(); iter++)
		{
			(*iter)->SetOriginX(offsetX);
		}
		break;
	}
}

void LayoutContainer::SetOffsetX(int offsetX)
{
	int diff = offsetX - this->offsetX;
	std::list<LayoutElement*>::iterator iter;
	for (iter = elements.begin(); iter != elements.end(); iter++)
	{
		auto elem = (*iter);
		elem->SetOriginX(elem->GetOriginX() + diff);
	}

	this->offsetX = offsetX;
}

void LayoutContainer::SetOffsetY(int offsetY)
{
	int diff = offsetY - this->offsetY;
	std::list<LayoutElement*>::iterator iter;
	for (iter = elements.begin(); iter != elements.end(); iter++)
	{
		auto elem = (*iter);
		elem->SetOriginY(elem->GetOriginY() + diff);
	}

	this->offsetY = offsetY;
}

int LayoutContainer::GetWidth()
{
	return maxElemWidth;
}

int LayoutContainer::GetHeight()
{
	std::list<LayoutElement*>::iterator iter;
	int y = 0;

	for (iter = elements.begin(); iter != elements.end(); iter++)
	{
		auto elem = *iter;
		y += elem->marginTop + elem->GetHeight() + elem->marginBottom;
	}

	return y;
}

void LayoutContainer::Render()
{
	std::list<LayoutElement*>::iterator iter;
	int y = offsetY;

	for (iter = elements.begin(); iter != elements.end(); iter++)
	{
		auto elem = *iter;
		elem->Render();
		y += elem->marginTop + elem->GetHeight() + elem->marginBottom;
	}
}