#ifndef MENU_OBJECT_H
#define MENU_OBJECT_H

#include "Shader.h"
#include <glm/glm.hpp>
#include <glad/glad.h>

class MenuObject
{
public:
	MenuObject(Shader *shader, float originX, float originY);

	virtual bool OverlapsPoint(float x, float y) = 0;
	virtual void HandleLeftMousePressed() = 0;
	virtual void HandleLeftMouseReleased(bool releasedOnObject) = 0;
	virtual void Render() = 0;
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

class Button : public MenuObject
{
public:
	Button(Shader *shader, float xpos, float ypos, float width, float height, glm::vec3 color, void *eventData);
	
	bool OverlapsPoint(float x, float y);
	void HandleLeftMousePressed();
	void HandleLeftMouseReleased(bool releasedOnObject);
	/* How to reimplement it differently with no void pointers being used? */
	void OnClick(void (*)(void *));
	void Click();
	void Render();
	
	static const int VERTICES_NUM = 6;

	float width, height;
	float vertices[36];
	void* eventData;
private:
	void (*onClickFn)(void *);
};

#endif // !MENU_OBJECT_H
