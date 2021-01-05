#include "MenuObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "helpers/MatrixTool.h"

MenuObject::MenuObject(float originX, float originY)
	: originX(originX),
	  originY(originY)
{
}

MenuObject::MenuObject(Matrix* projection, Shader *shader, float originX, float originY, int width, int height)
	: shader(shader),
	  VAO(0),
	  VBO(0),
	  originX(originX),
	  originY(originY),
	  projection(projection),
	  width(width),
	  height(height)
{
	model.matrix = glm::mat4(
		1.0f, 0.0f, 0.0f, originX,
		0.0f, 1.0f, 0.0f, originY,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	model.loc = glGetUniformLocation(shader->ID, "model");

	if (model.loc == -1)
	{
		std::cout << "ERROR::MENU_OBJECT::CONSTRUCTOR: Location of model matrix cannot be found." << std::endl;
	}
}

void MenuObject::SetOriginX(int x)
{
	originX = x;
}

void MenuObject::SetOriginY(int y)
{
	originY = y;
}

int MenuObject::GetOriginX()
{
	return originX;
}

int MenuObject::GetOriginY()
{
	return originY;
}

int MenuObject::GetWidth()
{
	return width;
}

int MenuObject::GetHeight()
{
	return height;
}
