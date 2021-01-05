#include "LayoutContainer.h"

LayoutContainer::LayoutContainer()
	: nextY(0),
	  alignment(e_Alignment::LEFT)
{
}

void LayoutContainer::AddElement(LayoutEntity* entity)
{
	entities.push_back(entity);
	entity->SetOriginY(nextY + entity->marginTop);
	int width = entity->GetWidth();
	maxElemWidth = width > maxElemWidth ? width : maxElemWidth;

	switch (alignment)
	{
	case e_Alignment::CENTER:
		entity->SetOriginX(originX + (maxElemWidth / 2) - (entity->GetWidth() / 2));
		break;
	case e_Alignment::RIGHT:
		entity->SetOriginX(originX + maxElemWidth - entity->GetWidth());
		break;
	default:
	case e_Alignment::LEFT:
		entity->SetOriginX(originX);
		break;
	}

	nextY += entity->marginTop + entity->GetHeight() + entity->marginBottom;
}

void LayoutContainer::SetAlignment(e_Alignment alignment)
{
	std::list<LayoutEntity*>::iterator iter;

	switch (alignment)
	{
	case e_Alignment::CENTER:
		for (iter = entities.begin(); iter != entities.end(); iter++)
		{
			(*iter)->SetOriginX(originX + (maxElemWidth / 2) - ((*iter)->GetWidth() / 2));
		}
		break;
	case e_Alignment::RIGHT:
		for (iter = entities.begin(); iter != entities.end(); iter++)
		{
			(*iter)->SetOriginX(originX + maxElemWidth - (*iter)->GetWidth());
		}
		break;
	default:
	case e_Alignment::LEFT:
		for (iter = entities.begin(); iter != entities.end(); iter++)
		{
			(*iter)->SetOriginX(originX);
		}
		break;
	}
}

int LayoutContainer::GetOriginX()
{
	return originX;
}

int LayoutContainer::GetOriginY()
{
	return originY;
}

void LayoutContainer::SetOriginX(int originX)
{
	int diff = originX - this->originX;
	std::list<LayoutEntity*>::iterator iter;
	for (iter = entities.begin(); iter != entities.end(); iter++)
	{
		auto entity = (*iter);
		entity->SetOriginX(entity->GetOriginX() + diff);
	}

	this->originX = originX;
}

void LayoutContainer::SetOriginY(int originY)
{
	int diff = originY - this->originY;
	std::list<LayoutEntity*>::iterator iter;
	for (iter = entities.begin(); iter != entities.end(); iter++)
	{
		auto entity = (*iter);
		entity->SetOriginY(entity->GetOriginY() + diff);
	}

	this->originY = originY;
}

int LayoutContainer::GetWidth()
{
	return maxElemWidth;
}

int LayoutContainer::GetHeight()
{
	std::list<LayoutEntity*>::iterator iter;
	int y = 0;

	for (iter = entities.begin(); iter != entities.end(); iter++)
	{
		auto entity = *iter;
		y += entity->marginTop + entity->GetHeight() + entity->marginBottom;
	}

	return y;
}
