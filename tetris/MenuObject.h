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

#endif // !MENU_OBJECT_H
