#ifndef TETRIS_GRID_H
#define TETRIS_GRID_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "Shader.h"
#include "helpers/MatrixTool.h"
#include "data_structures/misc.h"
#include "Tetromino.h"

struct Occupancy {
	bool occupied = false;
	glm::vec3 color = glm::vec3(0.9f, 0.9f, 0.9f);
};

class TetrisGrid
{
public:
	TetrisGrid(glm::mat4 *projectionMatrix, short rows, short cols, int cellSize, int borderWidth, glm::vec3 cellColor, glm::vec3 borderColor, int offsetX, int offsetY);
	~TetrisGrid();

	int GetWidth();
	int GetHeight();
	void Update(TetrominoPackage::Tetromino* tetromino, Occupancy* occupancyMatrix);
	void Render();

	glm::vec3* cellsColors;
private:
	Shader* shader;
	short rows, cols;
	int width, height, offsetX, offsetY;
	GLuint backgroundVAO;
	GLuint cellVAO;
	float* backgroundVertices;
	float *cellVertices;
	glm::mat4* translations;
	Matrix backgroundTranslation;
	GLuint isCellLoc;
	Matrix projection;

	GLuint VBO3;
	
	float* GenRectVerticesArray(float width, float height, glm::vec3& borderColor);
};

#endif // !TETRIS_GRID_H
