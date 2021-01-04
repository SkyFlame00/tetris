#ifndef MISC_H
#define MISC_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <list>

struct Matrix
{
	int loc;
	glm::mat4 *matrix;
};

struct UserInput
{
	float
		mouseX,
		mouseY;
	bool
		mousePressed,
		mouseReleased,
		escPressed,
		escReleased,
		leftArrowPressed,
		leftArrowReleased,
		rightArrowPressed,
		rightArrowReleased,
		upArrowPressed,
		upArrowReleased,
		downArrowPressed,
		downArrowReleased,
		spacePressed,
		spaceReleased;

	UserInput()
		: mouseX(0),
		  mouseY(0),
		  mousePressed(false),
		  mouseReleased(false),
		  escPressed(false),
		  escReleased(false),
		  leftArrowPressed(false),
		  leftArrowReleased(false),
		  rightArrowPressed(false),
		  rightArrowReleased(false),
		  upArrowPressed(false),
		  upArrowReleased(false),
		  downArrowPressed(false),
		  downArrowReleased(false),
		  spacePressed(false),
		  spaceReleased(false)
	{};
};

struct Character {
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
	struct {
		int yMin, yMax;
	} bbox;
};

#endif // !MISC_H
