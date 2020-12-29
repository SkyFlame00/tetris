#include "TetrisGrid.h"
#include <glm/gtc/matrix_access.hpp>

TetrisGrid::TetrisGrid(glm::mat4 *projectionMatrix, short rows, short cols, int cellSize, int borderWidth, glm::vec3 cellColor, glm::vec3 borderColor, int offsetX, int offsetY)
	: rows(rows),
	  cols(cols),
	  offsetX(offsetX),
	  offsetY(offsetY)
{
	GLuint VBO, VBO2;
	width = (int)cols * cellSize + ((int)cols + 1) * borderWidth;
	height = (int)rows * cellSize + ((int)rows + 1) * borderWidth;

	/* Shader initialization */
	shader = new Shader("shaders/rect.vert.glsl", "shaders/rect.frag.glsl");
	projection.loc = glGetUniformLocation(shader->ID, "projection");
	projection.matrix = projectionMatrix;

	/* Grid background vertices */
	backgroundVertices = GenRectVerticesArray((float)width, (float)height, borderColor);

	glGenVertexArrays(1, &backgroundVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(backgroundVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(float), backgroundVertices, GL_STATIC_DRAW);

	GLuint positionLoc = glGetAttribLocation(shader->ID, "aPos");
	GLuint colorLoc = glGetAttribLocation(shader->ID, "aColor");

	glEnableVertexAttribArray(positionLoc);
	glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(colorLoc);
	glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);

	/* Grid background model matrix */
	backgroundTranslation.matrix = new glm::mat4(
		1.0f, 0.0f, 0.0f, offsetX,
		0.0f, 1.0f, 0.0f, offsetY,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	backgroundTranslation.loc = glGetUniformLocation(shader->ID, "backgroundTranslation");
	isCellLoc = glGetUniformLocation(shader->ID, "isCell");

	/* Cell vertices */
	cellVertices = GenRectVerticesArray((float)cellSize, (float)cellSize, cellColor);

	glGenVertexArrays(1, &cellVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(cellVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(float), cellVertices, GL_STATIC_DRAW);

	/*glEnableVertexAttribArray(positionLoc);
	glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(colorLoc);
	glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));*/

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	/* Create model matrices for all the rows*cols cells */
	translations = new glm::mat4[rows*cols];

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			translations[i * cols + j] = glm::transpose(glm::mat4(
				1.0f, 0.0f, 0.0f, offsetX + borderWidth * (j + 1) + cellSize * j,
				0.0f, 1.0f, 0.0f, offsetY + borderWidth * (i + 1) + cellSize * i,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			));
		}
	}

	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, rows * cols * sizeof(glm::mat4), &translations[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	cellsColors = new glm::vec3[rows * cols];
	glGenBuffers(1, &VBO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, rows * cols * sizeof(glm::vec3), &cellsColors[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
}

TetrisGrid::~TetrisGrid()
{
	delete shader;
	delete cellVertices;
	delete backgroundVertices;
	delete translations;
	delete cellsColors;
}

float* TetrisGrid::GenRectVerticesArray(float width, float height, glm::vec3& borderColor)
{
	float *rectVertices = new float[36]{
		/* Triangle 1 */
		0.0f , 0.0f  , 0.0f, borderColor.x, borderColor.y, borderColor.z,
		width, 0.0f  , 0.0f, borderColor.x, borderColor.y, borderColor.z,
		0.0f , height, 0.0f, borderColor.x, borderColor.y, borderColor.z,
		/* Triangle 2 */
		width, 0.0f  , 0.0f, borderColor.x, borderColor.y, borderColor.z,
		0.0f , height, 0.0f, borderColor.x, borderColor.y, borderColor.z,
		width, height, 0.0f, borderColor.x, borderColor.y, borderColor.z,
	};
	return rectVertices;
}

void TetrisGrid::Update(TetrominoPackage::Tetromino* tetromino, Occupancy* occupancyMatrix)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			int idx = i * cols + j;
			cellsColors[idx] = occupancyMatrix[idx].color;
		}
	}

	if (tetromino)
	{
		for (int i = 0; i < tetromino->COORDINATES_NUM; i++)
		{
			glm::vec2 coordinates = tetromino->globalCoordinates->at(i);
			if (coordinates.x < 0) continue;
			int idx = coordinates.x * cols + coordinates.y;
			cellsColors[idx] = tetromino->color;
		}
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferSubData(GL_ARRAY_BUFFER, 0, rows * cols * sizeof(glm::vec3), &cellsColors[0]);
}

void TetrisGrid::Render()
{
	shader->use();
	glUniformMatrix4fv(projection.loc, 1, GL_TRUE, glm::value_ptr(*projection.matrix));

	{
		glUniform1i(isCellLoc, true);
		glBindVertexArray(cellVAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, rows * cols);
		glBindVertexArray(0);
	}

	{
		glUniform1i(isCellLoc, false);
		glUniformMatrix4fv(backgroundTranslation.loc, 1, GL_TRUE, glm::value_ptr(*backgroundTranslation.matrix));
		glBindVertexArray(backgroundVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
}