#include "PlainText.h"

PlainText::PlainText(TextRenderer* textRenderer, std::string text, glm::vec3 color, float originX, float originY)
	: MenuObject(originX, originY),
	  textRenderer(textRenderer),
	  text(text),
	  color(color)
{
	textRenderer->GetMeasurements(text, &width, &height);
}

bool PlainText::OverlapsPoint(float xpos, float ypos)
{
	return (xpos > originX && xpos < originX + width)
		&& (ypos > originY && ypos < originY + height);
}

void PlainText::HandleLeftMousePressed(float x, float y)
{
}

void PlainText::HandleLeftMouseReleased(bool releasedOnObject, float x, float y)
{
}

void PlainText::Render()
{
	textRenderer->Render(text, originX, textRenderer->maxAscent + originY, 1.0f, color);
}
