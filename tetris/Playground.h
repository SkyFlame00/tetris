#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include "TetrisGrid.h"
#include <glm/glm.hpp>
#include "data_structures/misc.h"
#include <cstdlib>
#include <list>
#include "NextTetrominosWindow.h"
#include "helpers/misc.h"

enum class ERemovingLinesStages
{
	INIT,
	FADE,
	DROP
};

class Playground
{
public:
	Playground(glm::mat4 *projection, int xunits, int yunits);
	~Playground();

	void Update(float deltaTime, UserInput* userInput);
	void Render();
private:
	Occupancy* occupancyMatrix;
	TetrisGrid *tetrisGrid;
	glm::mat4 *projection;
	float timePassed;
	bool isNextStep;
	TetrominoPackage::Tetromino* activeTetromino;
	int rows, cols;
	struct {
		int rows[4]{-1, -1, -1, -1};
		int totalRows = 0;
		int droppedRows = 0;
		float fadeTime = 0.3f;
		float dropTime = 0.2f;
		glm::vec3* cellsColorsDiff;
		float timePassed = 0.0f;
		ERemovingLinesStages stage;
	} removeLinesData;
	bool isLinesRemoving;
	bool gameOver = false;
	glm::vec3 fadeColor = glm::vec3(0.9f, 0.9f, 0.9f);
	glm::vec3 emptyCellColor;
	glm::vec3 borderColor;
	nexttetrominos_t nextTetrominos;
	NextTetrominosWindow* ntWindow;

	const float STEP_GAP = 1.0f;
	const float FADE_TIME = 1.0f;

	bool CanMoveTetromino(TetrominoPackage::EDirection direction);
	void MoveTetromino(TetrominoPackage::EDirection direction);
	bool CanRotateTetromino(TetrominoPackage::ERotationOrder clockwise);
	void RotateTetromino(TetrominoPackage::ERotationOrder clockwise);
	bool IsPushOutNeeded(std::array<glm::vec2, 4>* coordinates, int offsetRow, int offsetCol);
	bool CanPushOutTetromino(std::array<glm::vec2, 4>* coordinates, int offsetRow, int offsetCol);
	void PushOutTetromino(std::array<glm::vec2, 4>* nextCoordinates, int offsetRow, int offsetCol);
	void ProcessUserInput(UserInput* userInput);
	void PutTetromino();
	bool ShouldGameEnd(bool shouldPut);
	void EndGame();
	void SetRemovingLines();
	bool ShouldRemoveLines();
	void DeleteTetromino();
	void DoLinesRemovingWork(float deltaTime);
	int PopMaxRowFromRemovingLines();
	void TryPutTetromino();

	GLuint vao, vbo;
};

#endif // !PLAYGROUND_H
