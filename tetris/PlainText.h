#ifndef PLAIN_TEXT_H
#define PLAIN_TEXT_H

#include "MenuObject.h"
#include "TextRenderer.h"
#include <string>
#include <glm/glm.hpp>

class PlainText : public MenuObject
{
public:
	PlainText(TextRenderer* textRenderer, std::string text, glm::vec3 color, float originX = 0, float originY = 0);
	~PlainText();

	bool OverlapsPoint(float xpos, float ypos);
	void HandleLeftMousePressed();
	void HandleLeftMouseReleased(bool releasedOnObject);
	void Render();
private:
	TextRenderer* textRenderer;
	std::string text;
	glm::vec3 color;
};

#endif // !PLAIN_TEXT_H
