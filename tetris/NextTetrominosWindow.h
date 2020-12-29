#ifndef NEXT_TETROMINOS_WINDOW_H
#define NEXT_TETROMINOS_WINDOW_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <list>
#include <glad/glad.h>
#include "Tetromino.h"
#include "Shader.h"
#include "data_structures/misc.h"

typedef std::list<TetrominoPackage::Tetromino*> nexttetrominos_t;
typedef float(*rectvertices_t)[36];

class NextTetrominosWindow
{
public:
	NextTetrominosWindow(glm::mat4* projectionMatrix, int cellSize, int cellGapSize, glm::vec3 backgroundColor, int offsetX, int offsetY);
	~NextTetrominosWindow();

	void SetNextTetrominos(nexttetrominos_t* nextTetrominos);
	void Update();
	void Render();

	static const int ROWS_NUM = 16;
	static const int COLS_NUM = 6;
private:
	rectvertices_t MakeRect(float width, float height, glm::vec3 color);
	Shader shader;
	Matrix projection, *backgroundTranslation;
	glm::mat4* translations;
	glm::vec3* cellsColors;
	GLuint isCellLoc;
	glm::vec3 backgroundColor;

	nexttetrominos_t* nextTetrominos;
	struct {
		GLuint vbo, vao;
		rectvertices_t vertices;
	} cover;
	struct {
		GLuint vbo, vao, translationsVbo, colorsVbo;
		rectvertices_t vertices;
	} cell;

	GLuint cellVAO;
};

#endif

//#ifndef NEXT_TETROMINOS_WINDOW_H
//#define NEXT_TETROMINOS_WINDOW_H
//
//#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <glad/glad.h>
//#include "Shader.h"
//#include "helpers/MatrixTool.h"
//#include "data_structures/misc.h"
//#include "Tetromino.h"
//#include "TetrisGrid.h"
//
//class NextTetrominosWindow
//{
//public:
//	NextTetrominosWindow(glm::mat4* projectionMatrix, short rows, short cols, int cellSize, int borderWidth, glm::vec3 cellColor, glm::vec3 borderColor, int offsetX, int offsetY);
//	~NextTetrominosWindow();
//
//	int GetWidth();
//	int GetHeight();
//	void Update(TetrominoPackage::Tetromino* tetromino, Occupancy* occupancyMatrix);
//	void Render();
//
//	glm::vec3* cellsColors;
//private:
//	Shader* shader;
//	short rows, cols;
//	int width, height, offsetX, offsetY;
//	GLuint backgroundVAO;
//	GLuint cellVAO;
//	float* backgroundVertices;
//	float* cellVertices;
//	glm::mat4* translations;
//	Matrix backgroundTranslation;
//	GLuint isCellLoc;
//	Matrix projection;
//
//	float* GenRectVerticesArray(float width, float height, glm::vec3& borderColor);
//};
//
//#endif // !TETRIS_GRID_H
