#include "LayoutContainer.h"

using namespace Layout;

LayoutContainer::LayoutContainer()
	: LayoutContainer(e_Flow::HORIZONTAL)
{
}

LayoutContainer::LayoutContainer(e_Flow flow)
	: nextX(0),
	  nextY(0),
	  alignment(e_Alignment::LEFT),
	  flow(flow)
{
}

LayoutContainer::LayoutContainer(e_Flow flow, int width, int height)
	: LayoutContainer(flow)
{
	if (width > -1)
	{
		isWidthFixed = true;
		isSizeFixed = true;
		fixedWidth = width;
	}

	if (height > -1)
	{
		isHeightFixed = true;
		isSizeFixed = true;
		fixedHeight = height;
	}
}

LayoutContainer::LayoutContainer(int width, int height)
	: LayoutContainer(e_Flow::HORIZONTAL, width, height)
{
}

void LayoutContainer::AddElement(LayoutEntity* entity)
{
	entities.push_back(entity);

	if (flow == e_Flow::HORIZONTAL)
	{
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
	else if (flow == e_Flow::VERTICAL)
	{
		entity->SetOriginX(nextX + entity->marginLeft);
		int height = entity->GetHeight();
		maxElemHeight = height > maxElemHeight ? height : maxElemHeight;

		switch (alignment)
		{
		case e_Alignment::CENTER:
			entity->SetOriginY(originY + (maxElemHeight / 2) - (entity->GetHeight() / 2));
			break;
		case e_Alignment::RIGHT:
			entity->SetOriginY(originY + maxElemHeight - entity->GetHeight());
			break;
		default:
		case e_Alignment::LEFT:
			entity->SetOriginY(originY);
			break;
		}

		nextX += entity->marginLeft + entity->GetHeight() + entity->marginRight;
	}
}

void LayoutContainer::SetAlignment(e_Alignment alignment)
{
	std::list<LayoutEntity*>::iterator iter;

	switch (alignment)
	{
	case e_Alignment::CENTER:
		for (iter = entities.begin(); iter != entities.end(); iter++)
		{
			if (flow == e_Flow::HORIZONTAL)
			{
				(*iter)->SetOriginX(originX + (maxElemWidth / 2) - ((*iter)->GetWidth() / 2));
			}
			else if (flow == e_Flow::VERTICAL)
			{
				(*iter)->SetOriginY(originY + (maxElemHeight / 2) - ((*iter)->GetHeight() / 2));
			}
		}
		break;
	case e_Alignment::RIGHT:
		for (iter = entities.begin(); iter != entities.end(); iter++)
		{
			if (flow == e_Flow::HORIZONTAL)
			{
				(*iter)->SetOriginX(originX + maxElemWidth - (*iter)->GetWidth());
			}
			else if (flow == e_Flow::VERTICAL)
			{
				(*iter)->SetOriginY(originY + maxElemHeight - (*iter)->GetHeight());
			}
		}
		break;
	default:
	case e_Alignment::LEFT:
		for (iter = entities.begin(); iter != entities.end(); iter++)
		{
			if (flow == e_Flow::HORIZONTAL)
			{
				(*iter)->SetOriginX(originX);
			}
			else if (flow == e_Flow::VERTICAL)
			{
				(*iter)->SetOriginY(originY);
			}
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
	if (isWidthFixed)
	{
		return fixedWidth;
	}
	if (flow == e_Flow::HORIZONTAL)
	{
		return maxElemWidth;
	}
	else if (flow == e_Flow::VERTICAL)
	{
		std::list<LayoutEntity*>::iterator iter;
		int x = 0;

		for (iter = entities.begin(); iter != entities.end(); iter++)
		{
			auto entity = *iter;
			x += entity->marginLeft + entity->GetWidth() + entity->marginRight;
		}

		return x;
	}
}

int LayoutContainer::GetHeight()
{
	if (isHeightFixed)
	{
		return fixedHeight;
	}
	if (flow == e_Flow::HORIZONTAL)
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
	else if (flow == e_Flow::VERTICAL)
	{
		return maxElemHeight;
	}
}
