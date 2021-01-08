#include "Dropdown.h"

using namespace DropdownPackage;

Dropdown::Dropdown(Matrix* projection, Shader* shader, TextRenderer* textRenderer, Settings* settings, void* eventData)
	: MenuObject(projection, shader, 0, 0),
	  textRenderer(textRenderer),
	  isExpanded(false),
	  expandedHeight(0),
	  eventData(eventData)
{
	float maxGlyphHeight = static_cast<float>(textRenderer->GetMaxGlyphHeight());
	contentWidth = static_cast<float>(settings->width);
	float triangularVertices[6];
	glm::vec3 borderColor;
	Options::iterator chosenOption = std::find_if(settings->options.begin(), settings->options.end(),
		[settings](std::pair<std::string, std::string> option) { return option.second == settings->chosenKey; });
	
	if (settings->theme == Theme::DEFAULT)
	{
		triangularVertices[0] = 0.0f;
		triangularVertices[1] = 0.0f;
		triangularVertices[2] = 10.0f;
		triangularVertices[3] = 0.0f;
		triangularVertices[4] = 5.0f;
		triangularVertices[5] = 10.0f;

		borderThickness = 1.0f;
		padding = 10.0f;
		contentHeight = 2 * padding + maxGlyphHeight;

		triangularBoxHeight = contentHeight;
		triangularBoxWidth = triangularBoxHeight;
		float triangularWidth = triangularVertices[2] - triangularVertices[0];
		float triangularHeight = triangularVertices[5] - triangularVertices[1];

		float dx = 2 * borderThickness + contentWidth + triangularBoxWidth / 2 - triangularWidth / 2;
		float dy = borderThickness + triangularBoxHeight / 2 - triangularHeight / 2;
		triangularVertices[0] += dx;
		triangularVertices[2] += dx;
		triangularVertices[4] += dx;
		triangularVertices[1] += dy;
		triangularVertices[3] += dy;
		triangularVertices[5] += dy;

		this->width = 3 * borderThickness + contentWidth + triangularBoxWidth;
		this->height = 2 * borderThickness + contentHeight;
		this->choice.textX = originX + borderThickness + padding;
		this->choice.textY = originY + borderThickness + contentHeight / 2 + textRenderer->baselineShift;

		themeOptions.borderColor = glm::vec3(0.0, 0.5, 0.0);
		themeOptions.textColor = glm::vec3(0.0, 0.5, 0.0);
		themeOptions.bgColor = glm::vec3(1.0, 1.0, 1.0);
		themeOptions.chosenOptionBgColor = glm::vec3(0.9, 0.9, 0.9);
		themeOptions.chosenOptionTextColor = glm::vec3(0.5, 0.5, 0.5);
		themeOptions.hoveredOptionBgColor = glm::vec3(0.9, 0.9, 0.9);
	}

	/* ---- Choice borders ---- */

	choice.borderVertices = new float[12 * 5 + 6] {
		// Border 1
		0.0f                                                   , 0.0f,
		0.0f                                                   , borderThickness,
		3 * borderThickness + contentWidth + triangularBoxWidth, 0.0f,
		0.0f                                                   , borderThickness,
		3 * borderThickness + contentWidth + triangularBoxWidth, 0.0f,
		3 * borderThickness + contentWidth + triangularBoxWidth, borderThickness,

		// Border 2
		0.0f, borderThickness,
		0.0f, borderThickness + contentHeight,
		borderThickness, borderThickness,
		0.0f, borderThickness + contentHeight,
		borderThickness, borderThickness,
		borderThickness, borderThickness + contentHeight,

		// Border 3
		borderThickness + contentWidth, borderThickness,
		borderThickness + contentWidth, borderThickness + contentHeight,
		2 * borderThickness + contentWidth, borderThickness,
		borderThickness + contentWidth, borderThickness + contentHeight,
		2 * borderThickness + contentWidth, borderThickness,
		2 * borderThickness + contentWidth, borderThickness + contentHeight,

		// Border 4
		2 * borderThickness + contentWidth + triangularBoxWidth, borderThickness,
		2 * borderThickness + contentWidth + triangularBoxWidth, borderThickness + contentHeight,
		3 * borderThickness + contentWidth + triangularBoxWidth, borderThickness,
		2 * borderThickness + contentWidth + triangularBoxWidth, borderThickness + contentHeight,
		3 * borderThickness + contentWidth + triangularBoxWidth, borderThickness,
		3 * borderThickness + contentWidth + triangularBoxWidth, borderThickness + contentHeight,

		// Border 5
		0.0f, borderThickness + contentHeight,
		0.0f, 2 * borderThickness + contentHeight,
		3 * borderThickness + contentWidth + triangularBoxWidth, borderThickness + contentHeight,
		0.0f, 2 * borderThickness + contentHeight,
		3 * borderThickness + contentWidth + triangularBoxWidth, borderThickness + contentHeight,
		3 * borderThickness + contentWidth + triangularBoxWidth, 2 * borderThickness + contentHeight,

		// Triangular
		triangularVertices[0], triangularVertices[1],
		triangularVertices[2], triangularVertices[3],
		triangularVertices[4], triangularVertices[5]
	};

	glGenVertexArrays(1, &choice.borderVAO);
	glGenBuffers(1, &choice.borderVBO);

	glBindVertexArray(choice.borderVAO);
	glBindBuffer(GL_ARRAY_BUFFER, choice.borderVBO);

	glBufferData(GL_ARRAY_BUFFER, (12 * 5 + 6) * sizeof(float), &choice.borderVertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	/* ---- Choice background ---- */

	choice.bgVertices = new float[12]{
		0.0f, 0.0f,
		0.0f, 2 * borderThickness + contentHeight,
		3 * borderThickness + contentWidth + triangularBoxWidth, 0.0f,

		0.0f, 2 * borderThickness + contentHeight,
		3 * borderThickness + contentWidth + triangularBoxWidth, 0.0f,
		3 * borderThickness + contentWidth + triangularBoxWidth, 2 * borderThickness + contentHeight
	};

	glGenVertexArrays(1, &choice.bgVAO);
	glGenBuffers(1, &choice.bgVBO);

	glBindVertexArray(choice.bgVAO);
	glBindBuffer(GL_ARRAY_BUFFER, choice.bgVBO);

	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), &choice.bgVertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	/* ---- Option borders ---- */
	optionObject.borderVertices = new float[] {
		// Border 1
		0.0f, 0.0f,
		0.0f, contentHeight + borderThickness,
		borderThickness, 0.0f,
		0.0f, contentHeight + borderThickness,
		borderThickness, 0.0f,
		borderThickness, contentHeight + borderThickness,

		// Border 2
		borderThickness, contentHeight,
		borderThickness, contentHeight + borderThickness,
		2 * borderThickness + contentWidth + triangularBoxWidth, contentHeight,
		borderThickness, contentHeight + borderThickness,
		2 * borderThickness + contentWidth + triangularBoxWidth, contentHeight,
		2 * borderThickness + contentWidth + triangularBoxWidth, contentHeight + borderThickness,

		// Border 3
		2 * borderThickness + contentWidth + triangularBoxWidth, 0.0f,
		2 * borderThickness + contentWidth + triangularBoxWidth, contentHeight + borderThickness,
		3 * borderThickness + contentWidth + triangularBoxWidth, 0.0f,
		2 * borderThickness + contentWidth + triangularBoxWidth, contentHeight + borderThickness,
		3 * borderThickness + contentWidth + triangularBoxWidth, 0.0f,
		3 * borderThickness + contentWidth + triangularBoxWidth, contentHeight + borderThickness
	};

	glGenVertexArrays(1, &optionObject.borderVAO);
	glGenBuffers(1, &optionObject.borderVBO);

	glBindVertexArray(optionObject.borderVAO);
	glBindBuffer(GL_ARRAY_BUFFER, optionObject.borderVBO);

	glBufferData(GL_ARRAY_BUFFER, (12 * 3) * sizeof(float), &optionObject.borderVertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	/* ---- Option background ---- */

	optionObject.bgVertices = new float[] {
		borderThickness, 0.0f,
		borderThickness, contentHeight,
		2 * borderThickness + contentWidth + triangularBoxWidth, 0.0f,
		borderThickness, contentHeight,
		2 * borderThickness + contentWidth + triangularBoxWidth, 0.0f,
		2 * borderThickness + contentWidth + triangularBoxWidth, contentHeight
	};

	glGenVertexArrays(1, &optionObject.bgVAO);
	glGenBuffers(1, &optionObject.bgVBO);

	glBindVertexArray(optionObject.bgVAO);
	glBindBuffer(GL_ARRAY_BUFFER, optionObject.bgVBO);

	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), &optionObject.bgVertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	/* ---- Option instances ---- */
	Options* opts = &settings->options;
	int nextY = originY + 2 * borderThickness + contentHeight;
	Options::iterator iter;
	int textX = borderThickness + padding;

	for (iter = opts->begin(); iter != opts->end(); iter++)
	{
		glm::mat4 modelMatrix = {
			1.0f, 0.0f, 0.0f, originX,
			0.0f, 1.0f, 0.0f, nextY,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		OptionInstance opt = {
			modelMatrix,
			iter->first,
			iter->second,
			iter->second == chosenOption->second,
			textX,
			nextY + contentHeight / 2 + textRenderer->baselineShift
		};
		options.push_back(opt);

		float shift = contentHeight + borderThickness;
		nextY += shift;
		expandedHeight += shift;

		if (opt.chosen)
		{
			choice.option = &options.back();
		}
	}
}

bool Dropdown::OverlapsPoint(float x, float y)
{
	if (isExpanded)
	{
		return (x > originX && x < originX + width)
			&& (y > originY && y < originY + height + expandedHeight);
	}
	else
	{
		return (x > originX && x < originX + width)
			&& (y > originY && y < originY + height);
	}
}

void Dropdown::HandleLeftMousePressed(float x, float y)
{
}

void Dropdown::HandleLeftMouseReleased(bool releasedOnObject, float x, float y)
{
	if (!releasedOnObject)
	{
		isExpanded = false;
		return;
	}

	float choiceWidth = 3 * borderThickness + contentWidth + triangularBoxWidth;
	float choiceHeight = 2 * borderThickness + contentHeight;
	
	if (originX <= x && x <= originX + choiceWidth && originY <= y && y <= originY + choiceHeight)
	{
		isExpanded = !isExpanded;
		return;
	}

	float optWidth = choiceWidth;
	float optHeight = contentHeight + borderThickness;

	std::list<OptionInstance>::iterator iter;
	for (iter = options.begin(); iter != options.end(); iter++)
	{
		float optX = iter->modelMatrix[0][3];
		float optY = iter->modelMatrix[1][3];
		if (optX <= x && x <= optX + optWidth && optY <= y && y <= optY + optHeight)
		{
			if (&(*iter) == choice.option)
			{
				/* Don't do anything if clicked on already chosen option */
				break;
			}

			choice.option->chosen = false;
			iter->chosen = true;
			choice.option = &(*iter);
			Change(std::pair<std::string, std::string>{iter->text, iter->value});
			HandleFocusLost();
			break;
		}
	}

	
}

void Dropdown::HandleFocus()
{
}

void Dropdown::HandleFocusLost()
{
	isExpanded = false;
}

void Dropdown::HandleHover(float x, float y)
{
	float optWidth = 3 * borderThickness + contentWidth + triangularBoxWidth;
	float optHeight = contentHeight + borderThickness;

	std::list<OptionInstance>::iterator iter;
	for (iter = options.begin(); iter != options.end(); iter++)
	{
		float optX = iter->modelMatrix[0][3];
		float optY = iter->modelMatrix[1][3];
		if (optX <= x && x <= optX + optWidth && optY <= y && y <= optY + optHeight)
		{
			hoveredOption = &(*iter);
		}
	}
}

void Dropdown::HandleHoverLost()
{
}

void Dropdown::SetOriginX(int originX)
{
	int shift = (-this->originX) + originX;
	std::list<OptionInstance>::iterator iter;

	for (iter = options.begin(); iter != options.end(); iter++)
	{
		iter->modelMatrix[0][3] += shift;
		iter->textX += shift;
	}

	this->choice.textX = originX + borderThickness + padding;
	this->originX = originX;
}

void Dropdown::SetOriginY(int originY)
{
	int shift = (-this->originY) + originY;
	std::list<OptionInstance>::iterator iter;

	for (iter = options.begin(); iter != options.end(); iter++)
	{
		iter->modelMatrix[1][3] += shift;
		iter->textY += shift;
	}

	this->choice.textY = originY + borderThickness + contentHeight / 2 + textRenderer->baselineShift;
	this->originY = originY;
}

void Dropdown::OnChange(void (*onChangeCallback)(Option, void*))
{
	this->onChangeCallback = onChangeCallback;
}

void Dropdown::Change(Option option)
{
	if (onChangeCallback)
	{
		onChangeCallback(option, eventData);
	}
}

void Dropdown::Render()
{
	glm::mat4 modelMatrix = {
		1.0f, 0.0f, 0.0f, originX,
		0.0f, 1.0f, 0.0f, originY,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	/* First render the main widget */
	{
		shader->use();
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_TRUE, glm::value_ptr(*projection->matrix));
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_TRUE, glm::value_ptr(modelMatrix));
		glUniform3fv(glGetUniformLocation(shader->ID, "aColor"), 1, glm::value_ptr(themeOptions.bgColor));
		glBindVertexArray(choice.bgVAO);
		glDrawArrays(GL_TRIANGLES, 0, 12);
		glBindVertexArray(0);

		shader->use();
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_TRUE, glm::value_ptr(*projection->matrix));
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_TRUE, glm::value_ptr(modelMatrix));
		glUniform3fv(glGetUniformLocation(shader->ID, "aColor"), 1, glm::value_ptr(themeOptions.borderColor));
		glBindVertexArray(choice.borderVAO);
		glDrawArrays(GL_TRIANGLES, 0, 12 * 5 + 6);
		glBindVertexArray(0);

		textRenderer->Render(choice.option->text, choice.textX, choice.textY, 1.0f, themeOptions.textColor);
	}

	/* Render options */
	if (isExpanded)
	{
		std::list<OptionInstance>::iterator iter;
		for (iter = options.begin(); iter != options.end(); iter++)
		{
			glm::vec3 bgColor, textColor;

			if (iter->chosen)
			{
				bgColor = themeOptions.chosenOptionBgColor;
				textColor = themeOptions.chosenOptionTextColor;
			}
			else
			{
				bgColor = themeOptions.bgColor;
				textColor = themeOptions.textColor;
			}

			if (!iter->chosen && &(*iter) == hoveredOption)
			{
				bgColor = themeOptions.hoveredOptionBgColor;
			}

			shader->use();
			glBindVertexArray(optionObject.borderVAO);
			glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_TRUE, glm::value_ptr(*projection->matrix));
			glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_TRUE, glm::value_ptr(iter->modelMatrix));
			glUniform3fv(glGetUniformLocation(shader->ID, "aColor"), 1, glm::value_ptr(themeOptions.borderColor));
			glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
			glBindVertexArray(0);

			shader->use();
			glBindVertexArray(optionObject.bgVAO);
			glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_TRUE, glm::value_ptr(*projection->matrix));
			glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_TRUE, glm::value_ptr(iter->modelMatrix));
			glUniform3fv(glGetUniformLocation(shader->ID, "aColor"), 1, glm::value_ptr(bgColor));
			glDrawArrays(GL_TRIANGLES, 0, 12);
			glBindVertexArray(0);

			textRenderer->Render(iter->text, iter->textX, iter->textY, 1.0f, textColor);
		}
	}

}
