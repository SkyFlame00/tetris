#include "MenuObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "helpers/MatrixTool.h"

MenuObject::MenuObject(Shader *shader, float originX, float originY)
	: shader(shader),
	  VAO(0),
	  VBO(0),
	  originX(originX),
	  originY(originY)
{
	model.matrix = glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	model.loc = glGetUniformLocation(shader->ID, "model");

	if (model.loc == -1)
	{
		std::cout << "ERROR::MENU_OBJECT::CONSTRUCTOR: Location of model matrix cannot be found." << std::endl;
		return;
	}

	pressed.val = false;
	pressed.loc = glGetUniformLocation(shader->ID, "pressed");

	if (pressed.loc == -1)
	{
		std::cout << "ERROR::MENU_OBJECT::CONSTRUCTOR: Location of 'pressed' uniform cannot be found." << std::endl;
		return;
	}
}

Button::Button(Shader *shader, float xpos, float ypos, float width, float height, glm::vec3 color, void *eventData)
	: MenuObject(shader, xpos, ypos),
	  width(width),
	  height(height),
	  eventData(eventData),
	  vertices{
		0    , 0     , 0.0f, color.x, color.y, color.z,
		width, 0     , 0.0f, color.x, color.y, color.z,
		0    , height, 0.0f, color.x, color.y, color.z,

		0    , height, 0.0f, color.x, color.y, color.z,
		width, height, 0.0f, color.x, color.y, color.z,
		width, 0     , 0.0f, color.x, color.y, color.z,
	  }
{
	/* Generate vertices */
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	/* Adjust the model matrix. The origin of a button is the top-left corner */
	model.matrix[0][3] = xpos;
	model.matrix[1][3] = ypos;
}

void Button::Render()
{
	shader->use();
	glBindVertexArray(VAO);
	glUniformMatrix4fv(model.loc, 1, GL_TRUE, glm::value_ptr(model.matrix));
	glUniform1i(pressed.loc, pressed.val);
	glDrawArrays(GL_TRIANGLES, 0, VERTICES_NUM);
}

bool Button::OverlapsPoint(float xpos, float ypos)
{
	return (xpos > originX && xpos < originX + width)
		&& (ypos > originY && ypos < originY + height);
}

void Button::HandleLeftMousePressed()
{
	pressed.val = true;
}

void Button::HandleLeftMouseReleased(bool releasedOnObject)
{
	if (!pressed.val)
	{
		return;
	}

	if (releasedOnObject)
	{
		Click();
	}

	pressed.val = false;
}

void Button::OnClick(void (*callback)(void*))
{
	onClickFn = callback;
}

void Button::Click()
{
	onClickFn(eventData);
}