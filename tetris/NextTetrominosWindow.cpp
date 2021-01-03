#include "NextTetrominosWindow.h"

NextTetrominosWindow::NextTetrominosWindow(glm::mat4* projectionMatrix, int cellSize, int cellGapSize, glm::vec3 backgroundColor, int offsetX, int offsetY)
	: shader("shaders/rect.vert.glsl", "shaders/rect.frag.glsl"),
	  backgroundColor(backgroundColor)
{
	/* Shader preparations */
	projection.matrix = projectionMatrix;
	projection.loc = glGetUniformLocation(shader.ID, "projection");
	backgroundTranslation = new Matrix;
	backgroundTranslation->loc = glGetUniformLocation(shader.ID, "backgroundTranslation");
	backgroundTranslation->matrix = new glm::mat4(
		1.0f, 0.0f, 0.0f, offsetX,
		0.0f, 1.0f, 0.0f, offsetY,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	isCellLoc = glGetUniformLocation(shader.ID, "isCell");
	GLuint positionLoc = glGetAttribLocation(shader.ID, "aPos");
	GLuint colorLoc = glGetAttribLocation(shader.ID, "aColor");
	GLuint translationLoc = glGetAttribLocation(shader.ID, "translation");
	GLuint newColorLoc = glGetAttribLocation(shader.ID, "newColor");

	/* Make cover */
	float coverWidth = cellSize * COLS_NUM + cellGapSize * (COLS_NUM + 1);
	float coverHeight = cellSize * ROWS_NUM + cellGapSize * (ROWS_NUM + 1);
	cover.vertices = MakeRect(coverWidth, coverHeight, backgroundColor);

	glGenVertexArrays(1, &cover.vao);
	glBindVertexArray(cover.vao);
	glGenBuffers(1, &cover.vbo);
	
	glBindBuffer(GL_ARRAY_BUFFER, cover.vbo);
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(float), &cover.vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(colorLoc);
	glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	/* Make cell */
	glm::vec3 nothingColor(1.0f, 1.0f, 1.0f);
	cell.vertices = MakeRect(cellSize, cellSize, nothingColor);

	glGenVertexArrays(1, &cell.vao);
	glGenBuffers(1, &cell.vbo);

	glBindVertexArray(cell.vao);
	glBindBuffer(GL_ARRAY_BUFFER, cell.vbo);

	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(float), cell.vertices, GL_STATIC_DRAW);

	/*glEnableVertexAttribArray(positionLoc);
	glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(colorLoc);
	glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));*/

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	/* Create model matrices for all the rows*cols cells */
	translations = new glm::mat4[ROWS_NUM * COLS_NUM];

	for (int i = 0; i < ROWS_NUM; i++)
	{
		for (int j = 0; j < COLS_NUM; j++)
		{
			translations[i * COLS_NUM + j] = glm::transpose(glm::mat4(
				1.0f, 0.0f, 0.0f, offsetX + cellGapSize * (j + 1) + cellSize * j,
				0.0f, 1.0f, 0.0f, offsetY + cellGapSize * (i + 1) + cellSize * i,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			));
		}
	}

	glGenBuffers(1, &cell.translationsVbo);
	glBindBuffer(GL_ARRAY_BUFFER, cell.translationsVbo);
	glBufferData(GL_ARRAY_BUFFER, ROWS_NUM * COLS_NUM * sizeof(glm::mat4), &translations[0], GL_STATIC_DRAW);
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

	cellsColors = new glm::vec3[ROWS_NUM * COLS_NUM];
	glGenBuffers(1, &cell.colorsVbo);
	glBindBuffer(GL_ARRAY_BUFFER, cell.colorsVbo);
	glBufferData(GL_ARRAY_BUFFER, ROWS_NUM * COLS_NUM * sizeof(glm::vec3), &cellsColors[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);

}

NextTetrominosWindow::~NextTetrominosWindow()
{
	delete cover.vertices;
	delete cell.vertices;
	delete backgroundTranslation;
	delete translations;
	delete cellsColors;
}

rectvertices_t NextTetrominosWindow::MakeRect(float width, float height, glm::vec3 color)
{
	float* vertices = new float[36]{
		/* Triangle 1 */
		0.0f , 0.0f  , 0.0f, color.x, color.y, color.z,
		width, 0.0f  , 0.0f, color.x, color.y, color.z,
		0.0f , height, 0.0f, color.x, color.y, color.z,
		/* Triangle 2 */
		width, 0.0f  , 0.0f, color.x, color.y, color.z,
		0.0f , height, 0.0f, color.x, color.y, color.z,
		width, height, 0.0f, color.x, color.y, color.z,
	};
	return (rectvertices_t)vertices;
}

void NextTetrominosWindow::SetNextTetrominos(nexttetrominos_t* nextTetrominos)
{
	this->nextTetrominos = nextTetrominos;
}

void NextTetrominosWindow::Update()
{
	using namespace TetrominoPackage;

	for (int i = 0; i < ROWS_NUM; i++)
		for (int j = 0; j < COLS_NUM; j++)
			cellsColors[i * COLS_NUM + j] = backgroundColor;

	int offsetRow = 1, offsetCol = 1;

	for (Tetromino* tetr : *nextTetrominos)
	{
		for (int k = 0; k < tetr->COORDINATES_NUM; k++)
		{
			int i = tetr->coordinates->at(k)[0];
			int j = tetr->coordinates->at(k)[1];
			cellsColors[(i + offsetRow) * COLS_NUM + (j + offsetCol)] = tetr->color;
		}
		offsetRow += 5;
	}

	glBindBuffer(GL_ARRAY_BUFFER, cell.colorsVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, ROWS_NUM * COLS_NUM * sizeof(glm::vec3), &cellsColors[0]);
}

void NextTetrominosWindow::Render()
{
	shader.use();
	glUniformMatrix4fv(projection.loc, 1, GL_TRUE, glm::value_ptr(*projection.matrix));

	{
		glUniform1i(isCellLoc, false);
		glUniformMatrix4fv(backgroundTranslation->loc, 1, GL_TRUE, glm::value_ptr(*backgroundTranslation->matrix));
		glBindVertexArray(cover.vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	{
		glUniform1i(isCellLoc, true);
		glBindVertexArray(cell.vao);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, ROWS_NUM * COLS_NUM);
		glBindVertexArray(0);
	}
}