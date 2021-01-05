#include "TextRenderer.h"

TextRenderer::TextRenderer(glm::mat4* projection, Shader* shader, std::map<char, Character>* charset)
    : shader(shader),
      projection(projection),
      charset(charset),
	  maxAscent(0),
	  maxDescent(0)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	/* Calculate some metrics */
	maxHeight = 0;
	for (unsigned int c = 0; c < 128; c++)
	{
		Character ch = (*charset)[c];

		/* Calc maxHeight */
		if (ch.Size.y > maxHeight)
			maxHeight = ch.Size.y;

		/* Calc maxAscent and maxDescent */
		int ascent = ch.Bearing.y;
		int descent = ch.Size.y - ch.Bearing.y;
		maxAscent = ascent > maxAscent ? ascent : maxAscent;
		maxDescent = descent > maxDescent ? descent : maxDescent;
	}

	if (maxAscent > maxDescent)
	{
		baselineShift = maxHeight / 2 - maxDescent;
	}
	else
	{
		baselineShift = -(maxHeight / 2 - maxAscent);
	}
}

TextRenderer::~TextRenderer()
{
    delete shader;
    delete charset;
}

void TextRenderer::GetMeasurements(std::string text, int* textWidth, int* textHeight)
{
	int width = 0;
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = (*charset)[*c];
		width += ch.Advance >> 6;
	}

	*textWidth = width;
	*textHeight = maxHeight;
}


void TextRenderer::Render(std::string text, float x, float y, float scale, glm::vec3 color)
{
	shader->use();

	glUniform3f(glGetUniformLocation(shader->ID, "textColor"), color.x, color.y, color.z);
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_TRUE, glm::value_ptr(*projection));
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = (*charset)[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y + (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos - h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos - h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos - h,   1.0f, 0.0f }
		};

		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}