#ifndef LAYOUT_CONTAINER_H
#define LAYOUT_CONTAINER_H

#include "LayoutElement.h"
#include "MenuObject.h"
#include <list>
#include "LayoutEntity.h"

namespace Layout {

	enum class e_Alignment
	{
		LEFT,
		CENTER,
		RIGHT
	};

	enum class e_Flow
	{
		HORIZONTAL,
		VERTICAL
	};

	class LayoutContainer : public LayoutEntity
	{
	public:
		LayoutContainer();
		LayoutContainer(e_Flow flow);
		LayoutContainer(int width, int height);
		LayoutContainer(e_Flow flow, int width, int height);
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
		int originX, originY, nextX, nextY, maxElemWidth, maxElemHeight;
		std::list<LayoutEntity*> entities;
		MenuObject* capturedObject;
		bool isSizeFixed, isWidthFixed, isHeightFixed;
		int fixedWidth, fixedHeight;
		e_Flow flow;
	};

}

#endif // !LAYOUT_CONTAINER_H