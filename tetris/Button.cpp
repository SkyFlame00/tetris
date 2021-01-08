#include "Button.h"

ButtonPackage::Button::Button(Matrix* projection, Shader* shader, TextRenderer* textRenderer, s_AppearanceSettings* aSettings, s_TextSettings* tSettings, s_BorderSettings* bSettings, void* eventData)
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
	  textRenderer(textRenderer),
	  hasBorder(false),
	  textSize(tSettings->size)
{
	float borderWidth = 0.0f;
	float width = (float)aSettings->width;
	float height = (float)aSettings->height;

	/* Border */
	if (bSettings)
	{
		hasBorder = true;
		borderWidth = (float)bSettings->thickness;
		this->width += 2 * borderWidth;
		this->height += 2 * borderWidth;

		for (int i = 0; i < 6; i++)
		{
			vertices[i * 6] += borderWidth;
			vertices[i * 6 + 1] += borderWidth;
		}

		borderVertices = new float[]{
			// Top rect
			0.0f                   , 0.0f                    , 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,
			2 * borderWidth + width, 0.0f                    , 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,
			0.0f                   , borderWidth             , 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,
			2 * borderWidth + width, 0.0f                    , 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,
			0.0f                   , borderWidth             , 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,
			2 * borderWidth + width, borderWidth             , 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,

			// Bottom rect
			0.0f                   , borderWidth + height    , 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,
			0.0f                   , 2 * borderWidth + height, 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,
			2 * borderWidth + width, borderWidth + height    , 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,
			0.0f                   , 2 * borderWidth + height, 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,
			2 * borderWidth + width, borderWidth + height    , 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,
			2 * borderWidth + width, 2 * borderWidth + height, 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,

			// Left rect
			0.0f                   , borderWidth         , 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,
			0.0f                   , borderWidth + height, 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,
			borderWidth            , borderWidth         , 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,
			0.0f                   , borderWidth + height, 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,
			borderWidth            , borderWidth         , 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,
			borderWidth            , borderWidth + height, 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,

			// Right rect
			borderWidth + width    , borderWidth         , 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,
			borderWidth + width    , borderWidth + height, 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,
			2 * borderWidth + width, borderWidth         , 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,
			borderWidth + width    , borderWidth + height, 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,
			2 * borderWidth + width, borderWidth         , 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,
			2 * borderWidth + width, borderWidth + height, 0.0f, bSettings->color.x, bSettings->color.y, bSettings->color.z,
		};

		glGenVertexArrays(1, &borderVAO);
		glGenBuffers(1, &borderVBO);

		glBindVertexArray(borderVAO);
		glBindBuffer(GL_ARRAY_BUFFER, borderVBO);

		glBufferData(GL_ARRAY_BUFFER, 36 * 4 * sizeof(float), borderVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

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

	glBindVertexArray(borderVAO);
	glUniformMatrix4fv(projection->loc, 1, GL_TRUE, glm::value_ptr(*projection->matrix));
	glUniformMatrix4fv(model.loc, 1, GL_TRUE, glm::value_ptr(modelMatrix));
	glUniform1i(pressed.loc, pressed.val);
	glDrawArrays(GL_TRIANGLES, 0, 24);
	glBindVertexArray(0);

	textRenderer->Render(text, textX, textY, 1.0f, pressed.val ? textColorPressed : textColor);
}

bool ButtonPackage::Button::OverlapsPoint(float xpos, float ypos)
{
	return (xpos > originX && xpos < originX + width)
		&& (ypos > originY && ypos < originY + height);
}

void ButtonPackage::Button::HandleLeftMousePressed(float x, float y)
{
	pressed.val = true;
}

void ButtonPackage::Button::HandleLeftMouseReleased(bool releasedOnObject, float x, float y)
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
