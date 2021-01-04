#ifndef BUTTON_H
#define BUTTON_H

#include <glm/glm.hpp>
#include "MenuObject.h"

namespace ButtonPackage
{
	struct s_AppearanceSettings
	{
		int xpos, ypos, width, height;
		glm::vec3 color, colorPressed;
	};

	struct s_TextSettings
	{
		int size;
		char* text;
		glm::vec3 color, colorPressed;
	};


	class Button : public MenuObject
	{
	public:
		Button(Matrix* projection, Shader* shader, TextRenderer* textRenderer, s_AppearanceSettings* aSettings, s_TextSettings* tSettings, void* eventData);

		bool OverlapsPoint(float x, float y);
		void HandleLeftMousePressed();
		void HandleLeftMouseReleased(bool releasedOnObject);
		/* How to reimplement it differently with no void pointers being used? */
		void OnClick(void (*)(void*));
		void Click();
		void Render();
		void SetOriginX(int originX);
		void SetOriginY(int originY);

		static const int VERTICES_NUM = 6;

		float vertices[36];
		void* eventData;
	private:
		void (*onClickFn)(void*);

		TextRenderer* textRenderer;
		char* text;
		int textX, textY, textWidth, textHeight;
		glm::vec3 textColor, textColorPressed;
		struct {
			int loc;
			bool val;
		} pressed;
	};
}

#endif // !BUTTON_H
