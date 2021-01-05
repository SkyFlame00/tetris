#ifndef PLAIN_TEXT_H
#define PLAIN_TEXT_H

#include "MenuObject.h"
#include "TextRenderer.h"

class PlainText : public MenuObject
{
public:
	PlainText(TextRenderer* textRenderer);
	~PlainText();
private:
	TextRenderer* textRenderer;
};

#endif // !PLAIN_TEXT_H
