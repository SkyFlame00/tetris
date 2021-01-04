#include "Button.h"

ButtonPackage::Button::Button(Matrix* projection, Shader* shader, TextRenderer* textRenderer, s_AppearanceSettings* aSettings, s_TextSettings* tSettings, void* eventData)
	: MenuObject(projection, shader, aSettings->xpos, aSettings->ypos, aSettings->width, aSettings->height),
	  eventData(eventData),
	  vertices{
	    0                      , 0                       , 0.0f, aSettings->color.x, aSettings->color.y, aSettings->color.z,
	    (float)aSettings->width, 0                       , 0.0f, aSettings->color.x, aSettings->color.y, aSettings->color.z,
  	    0                      , (float)aSettings->height, 0.0f, aSettings->color.x, aSettings->color.y, aSettings->color.z,

	    0                      , (float)aSettings->height, 0.0f, aSettings->color.x, aSettings->color.y, aSettings->color.z,
	    (float)aSettings->width, (float)aSettings->height, 0.0f, aSettings->color.x, aSettings->color.y, aSettings->color.z,
	    (float)aSettings->width, 0                       , 0.0f, aSettings->color.x, aSettings->color.y, aSettings->color.z,
      },
	  textRenderer(textRenderer)
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

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/* Adjust the model matrix. The origin of a button is the top-left corner */
	model.matrix[0][3] = aSettings->xpos;
	model.matrix[1][3] = aSettings->ypos;

	if (tSettings->text)
	{
		text = tSettings->text;
		textRenderer->GetMeasurements(tSettings->text, &textWidth, &textHeight);
		textX = (aSettings->xpos + aSettings->width / 2) - textWidth / 2;
		textY = (aSettings->ypos + aSettings->height / 2) + textRenderer->baselineShift;
	}

	textColor = tSettings->color;
	textColorPressed = tSettings->colorPressed;

	/* Pressed */
	pressed.val = false;
	pressed.loc = glGetUniformLocation(shader->ID, "pressed");

	if (pressed.loc == -1)
	{
		std::cout << "ERROR::MENU_OBJECT::CONSTRUCTOR: Location of 'pressed' uniform cannot be found." << std::endl;
	}
}

void ButtonPackage::Button::SetOriginX(int x)
{
	originX = x;
	textX = (x + width / 2) - textWidth / 2;
}

void ButtonPackage::Button::SetOriginY(int y)
{
	originY = y;
	textY = (y + height / 2) + textRenderer->baselineShift;
}

void ButtonPackage::Button::Render()
{
	glm::mat4 modelMatrix = {
		1.0f, 0.0f, 0.0f, originX,
		0.0f, 1.0f, 0.0f, originY,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
	};

	shader->use();
	glBindVertexArray(VAO);
	glUniformMatrix4fv(projection->loc, 1, GL_TRUE, glm::value_ptr(*projection->matrix));
	glUniformMatrix4fv(model.loc, 1, GL_TRUE, glm::value_ptr(modelMatrix));
	glUniform1i(pressed.loc, pressed.val);
	glDrawArrays(GL_TRIANGLES, 0, VERTICES_NUM);
	glBindVertexArray(0);

	textRenderer->Render(text, textX, textY, 1.0f, pressed.val ? textColorPressed : textColor);
}

bool ButtonPackage::Button::OverlapsPoint(float xpos, float ypos)
{
	return (xpos > originX && xpos < originX + width)
		&& (ypos > originY && ypos < originY + height);
}

void ButtonPackage::Button::HandleLeftMousePressed()
{
	pressed.val = true;
}

void ButtonPackage::Button::HandleLeftMouseReleased(bool releasedOnObject)
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

void ButtonPackage::Button::OnClick(void (*callback)(void*))
{
	onClickFn = callback;
}

void ButtonPackage::Button::Click()
{
	onClickFn(eventData);
}
