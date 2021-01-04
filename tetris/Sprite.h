#ifndef SPRITE_H
#define SPRITE_H

#include <iostream>
#include "MenuObject.h"
#include <glm/gtc/type_ptr.hpp>
#include "vendor/stb_image.h"

class Sprite : public MenuObject
{
public:
	Sprite(Matrix* projection, Shader* shader, float originX, float originY, int width, int height, char* texPath);
	~Sprite();

	void Render();
	bool OverlapsPoint(float x, float y);
	void HandleLeftMousePressed();
	void HandleLeftMouseReleased(bool releasedOnObject);

	int imageWidth, imageHeight;
private:
	int nrChannels;
	unsigned char* data;
	GLuint texture;
	float *vertices;
};

#endif // !SPRITE_H
