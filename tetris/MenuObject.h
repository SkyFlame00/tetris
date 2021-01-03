#ifndef MENU_OBJECT_H
#define MENU_OBJECT_H

#include "Shader.h"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "data_structures/misc.h"
#include "TextRenderer.h"

class MenuObject
{
public:
	MenuObject(Matrix* projection, Shader *shader, float originX, float originY, int width, int height);

	virtual bool OverlapsPoint(float x, float y) = 0;
	virtual void HandleLeftMousePressed() = 0;
	virtual void HandleLeftMouseReleased(bool releasedOnObject) = 0;
	virtual void Render() = 0;
	virtual void SetOriginX(int originX);
	virtual void SetOriginY(int originY);
	virtual int GetOriginX();
	virtual int GetOriginY();
	virtual int GetWidth();
	virtual int GetHeight();
protected:
	Shader *shader;
	double originX, originY;
	unsigned int VAO, VBO;
	struct {
		int loc;
		glm::mat4 matrix;
	} model;
	struct {
		int loc;
		bool val;
	} pressed;
	Matrix* projection;
	int width, height;
};

//class Cursor : public MenuObject
//{
//public:
//	Cursor();
//private:
//
//};

//class PlainText : MenuObject
//{
//
//};

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
		Button(Matrix* projection, Shader* shader, TextRenderer* textRenderer, s_AppearanceSettings *aSettings, s_TextSettings *tSettings, void* eventData);

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
	};
}

#endif // !MENU_OBJECT_H
