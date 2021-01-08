#include "Sprite.h"

Sprite::Sprite(Matrix* projection, Shader* shader, float originX, float originY, int width, int height, char* texPath)
	: MenuObject(projection, shader, originX, originY, width, height)
{
	/* First come vertices */
	vertices = new float[24]{
		// pos                       // tex
		0.0f        , 0.0f         , 0.0f, 1.0f,
		(float)width, 0.0f         , 1.0f, 1.0f,
		(float)width, float(height), 1.0f, 0.0f,

		0.0f        , 0.0f         , 0.0f, 1.0f,
		0.0f        , float(height), 0.0f, 0.0f,
		(float)width, float(height), 1.0f, 0.0f
	};


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	/* Load the texture */
	data = stbi_load(texPath, &imageWidth, &imageHeight, &nrChannels, 0);

	if (!data)
	{
		std::cout << "SPRITE::CONSTRUCTOR: Failed to load image" << std::endl;
		return;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

Sprite::~Sprite()
{
	delete vertices;
}

bool Sprite::OverlapsPoint(float x, float y)
{
	return false;
}

void Sprite::HandleLeftMousePressed(float x, float y)
{
}

void Sprite::HandleLeftMouseReleased(bool releasedOnObject, float x, float y)
{
}

void Sprite::Render()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glm::mat4 modelMatrix = {
		1.0f, 0.0f, 0.0f, originX,
		0.0f, 1.0f, 0.0f, originY,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
	};

	glBindTexture(GL_TEXTURE_2D, texture);
	shader->use();
	glUniformMatrix4fv(projection->loc, 1, GL_TRUE, glm::value_ptr(*projection->matrix));
	glUniformMatrix4fv(model.loc, 1, GL_TRUE, glm::value_ptr(modelMatrix));
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
