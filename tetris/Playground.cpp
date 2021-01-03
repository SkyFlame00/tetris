#include "Playground.h"

Playground::Playground(glm::mat4 *projection, int xunits, int yunits)
	: projection(projection),
	  timePassed(0.0f)
{
	/* Create tetris grid */
	rows = 17;
	cols = 8;
	const int cellSize = 23;
	const int borderWidth = 1;
	const int gridWidth = (int)cols * cellSize + ((int)cols + 1) * borderWidth;
	const int gridHeight = (int)rows * cellSize + ((int)rows + 1) * borderWidth;
	const int offsetX = (xunits / 2) - (gridWidth / 2);
	const int offsetY = (yunits / 2) - (gridHeight / 2);
	this->emptyCellColor = glm::vec3(0.9f, 0.9f, 0.9f);
	this->borderColor = glm::vec3(0.2f, 0.2f, 0.2f);
	tetrisGrid = new TetrisGrid(projection, rows, cols, cellSize, borderWidth, emptyCellColor, borderColor, offsetX, offsetY);

	/* Create occupancy matrix */
	occupancyMatrix = new Occupancy[rows * cols];
	int arr[] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 1, 1, 1, 0,
		1, 1, 1, 1, 1, 1, 1, 0,
		1, 1, 1, 1, 1, 1, 1, 0,
	};

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			int idx = i * cols + j;
			if (arr[idx] == 1)
			{
				occupancyMatrix[idx].occupied = true;
				occupancyMatrix[idx].color = glm::vec3(0.0f, 0.0f, 1.0f);
			}
		}
	}

	/* Create first tetromino */
	using namespace TetrominoPackage;
	activeTetromino = new Tetromino(getRandomType(), getRandomOrientation(), getRandomColor(), rows, cols);
	activeTetromino = new Tetromino(EType::I, EOrientation::DOWNWARD, getRandomColor(), rows, cols);

	/* Populate next tetrominos */
	for (int i = 0; i < 3; i++)
	{
		nextTetrominos.push_back(new Tetromino(getRandomType(), getRandomOrientation(), getRandomColor(), rows, cols));
	}

	/* Set up next tetrominos window */
	int rows = NextTetrominosWindow::ROWS_NUM;
	int cols = NextTetrominosWindow::COLS_NUM;
	int cellGapSize = 1;
	int ntwWidth = cellSize * cols + cellGapSize * (cols + 1);
	int ntwHeight = cellSize * rows + cellGapSize * (rows + 1);
	int ntwOffsetX = xunits - ntwWidth - 50;
	int ntwOffsetY = (yunits / 2) - (ntwHeight / 2);
	ntWindow = new NextTetrominosWindow(projection, cellSize, cellGapSize, glm::vec3(0.9f, 0.9f, 0.9f), ntwOffsetX, ntwOffsetY);
	ntWindow->SetNextTetrominos(&nextTetrominos);
}

Playground::~Playground()
{
	delete tetrisGrid;
	delete occupancyMatrix;
	delete activeTetromino;
	delete ntWindow;
}

bool Playground::CanMoveTetromino(TetrominoPackage::EDirection direction)
{
	using namespace TetrominoPackage;

	for (int k = 0; k < activeTetromino->COORDINATES_NUM; k++)
	{
		int i = activeTetromino->globalCoordinates->at(k)[0];
		int j = activeTetromino->globalCoordinates->at(k)[1];
		int new_i, new_j;

		switch (direction)
		{
		case EDirection::DOWN:
			new_i = i + 1;
			if ((new_i > -1) && (new_i >= rows || occupancyMatrix[new_i * cols + j].occupied))
				return false;
			break;
		case EDirection::LEFT:
			new_j = j - 1;
			if ((new_j < 0) || (i > -1 && occupancyMatrix[i * cols + new_j].occupied))
				return false;
			break;
		case EDirection::RIGHT:
			new_j = j + 1;
			if ((new_j >= cols) || (i > -1 && occupancyMatrix[i * cols + new_j].occupied))
				return false;
			break;
		/* No default */
		}
	}

	return true;
}

void Playground::MoveTetromino(TetrominoPackage::EDirection direction)
{
	using namespace TetrominoPackage;

	switch (direction)
	{
	case EDirection::DOWN:
		activeTetromino->SetOffsetRow(activeTetromino->GetOffsetRow() + 1);
		break;
	case EDirection::LEFT:
		activeTetromino->SetOffsetCol(activeTetromino->GetOffsetCol() - 1);
		break;
	case EDirection::RIGHT:
		activeTetromino->SetOffsetCol(activeTetromino->GetOffsetCol() + 1);
		break;
	/* No default */
	}
}

void Playground::RotateTetromino(TetrominoPackage::ERotationOrder order)
{
	using namespace TetrominoPackage;

	activeTetromino->orientation = Tetromino::rotationMapping[order][activeTetromino->orientation];
	std::array<glm::vec2, 4>* newCoordinates = &Tetromino::types[activeTetromino->type][activeTetromino->orientation];

	int oldMaxRow = 0, newMaxRow = 0;

	for (int i = 0; i < activeTetromino->COORDINATES_NUM; i++)
	{
		int oldRow = activeTetromino->coordinates->at(i)[0];
		int newRow = newCoordinates->at(i)[0];
		oldMaxRow = oldRow > oldMaxRow ? oldRow : oldMaxRow;
		newMaxRow = newRow > newMaxRow ? newRow : newMaxRow;
	}

	int shift = oldMaxRow - newMaxRow;

	//activeTetromino->coordinates = &Tetromino::types[activeTetromino->type][activeTetromino->orientation];
	activeTetromino->coordinates = newCoordinates;
	int offsetRow = activeTetromino->GetOffsetRow() + shift;
	int offsetCol = activeTetromino->GetOffsetCol();

	if (IsPushOutNeeded(activeTetromino->coordinates, offsetRow, offsetCol))
	{
		PushOutTetromino(activeTetromino->coordinates, offsetRow, offsetCol);
	}
	else
	{
		activeTetromino->SetOffsetRow(offsetRow);
		activeTetromino->SetOffsetCol(offsetCol);
	}
}

bool Playground::CanRotateTetromino(TetrominoPackage::ERotationOrder order)
{
	using namespace TetrominoPackage;

	EOrientation newOrientation = Tetromino::rotationMapping[order][activeTetromino->orientation];
	std::array<glm::vec2, 4>* newCoordinates = &Tetromino::types[activeTetromino->type][newOrientation];
	int oldMaxRow = 0, newMaxRow = 0;

	for (int i = 0; i < activeTetromino->COORDINATES_NUM; i++)
	{
		int oldRow = activeTetromino->coordinates->at(i)[0];
		int newRow = newCoordinates->at(i)[0];
		oldMaxRow = oldRow > oldMaxRow ? oldRow : oldMaxRow;
		newMaxRow = newRow > newMaxRow ? newRow : newMaxRow;
	}

	int shift = oldMaxRow - newMaxRow;
	int offsetRow = activeTetromino->GetOffsetRow() + shift;
	int offsetCol = activeTetromino->GetOffsetCol();

	for (int i = 0; i < activeTetromino->COORDINATES_NUM; i++)
	{
		if (IsPushOutNeeded(newCoordinates, offsetRow, offsetCol) && !CanPushOutTetromino(newCoordinates, offsetRow, offsetCol))
		{
			return false;
		}
	}

	return true;
}

bool Playground::IsPushOutNeeded(std::array<glm::vec2, 4>* coordinates, int offsetRow, int offsetCol)
{
	for (int i = 0; i < activeTetromino->COORDINATES_NUM; i++)
	{
		int row = coordinates->at(i)[0] + offsetRow;
		int col = coordinates->at(i)[1] + offsetCol;
		int idx = row * cols + col;

		if ((row > -1) && (row >= rows || col >= cols || occupancyMatrix[idx].occupied))
		{
			return true;
		}
	}

	return false;
}

bool Playground::CanPushOutTetromino(std::array<glm::vec2, 4>* coordinates, int offsetRow, int offsetCol)
{
	int maxRow = 0, maxCol = 0;

	for (int i = 0; i < activeTetromino->COORDINATES_NUM; i++)
	{
		int row = coordinates->at(i)[0];
		int col = coordinates->at(i)[1];
		maxRow = row > maxRow ? row : maxRow;
		maxCol = col > maxCol ? col : maxCol;
	}

	/* Make total of rows (not coords) and cols from these vars */
	int totalRows = maxRow + 1;
	int totalCols = maxCol + 1;

	/* Check if tetro goes out of the borders */
	int minOccupiedRow = totalRows; /* It means nothing is occupied yet */
	int minOccupiedCol = totalCols;
	bool isHorizontal = maxCol > maxRow;
	int diff, shift;

	if (isHorizontal)
	{
		diff = (offsetCol + maxCol) - cols;
		diff = diff >= 0 ? diff + 1 : 0;
		minOccupiedCol = totalCols - diff;
	}
	else
	{
		diff = (offsetRow + maxRow) - rows;
		diff = diff >= 0 ? diff + 1 : 0;
		minOccupiedRow = totalRows - diff;
	}

	/* Here we find min occupied row and col to define how large our push out should be */
	for (int i = 0; i < activeTetromino->COORDINATES_NUM; i++)
	{
		int row = coordinates->at(i)[0];
		int col = coordinates->at(i)[1];

		if (occupancyMatrix[(row + offsetRow) * cols + (col + offsetCol)].occupied)
		{
			if (isHorizontal)
				minOccupiedCol = col < minOccupiedCol ? col : minOccupiedCol;
			else
				minOccupiedRow = row < minOccupiedRow ? row : minOccupiedRow;		
		}
	}

	/* Finally, we check if we can place tetro with the new shifted coords */
	for (int i = 0; i < activeTetromino->COORDINATES_NUM; i++)
	{
		int shiftedRow = coordinates->at(i)[0] + offsetRow;
		int shiftedCol = coordinates->at(i)[1] + offsetCol;

		if (isHorizontal)
		{
			shiftedCol -= (totalCols - minOccupiedCol);
		}
		else
		{
			shiftedRow -= (totalRows - minOccupiedRow);
		}

		if (shiftedCol < 0 || shiftedRow >= rows || occupancyMatrix[shiftedRow * cols + shiftedCol].occupied)
		{
			return false;
		}
	}

	return true;
}

void Playground::PushOutTetromino(std::array<glm::vec2, 4>* nextCoordinates, int offsetRow, int offsetCol)
{
	int maxRow = 0, maxCol = 0;

	for (int i = 0; i < activeTetromino->COORDINATES_NUM; i++)
	{
		int row = nextCoordinates->at(i)[0];
		int col = nextCoordinates->at(i)[1];
		maxRow = row > maxRow ? row : maxRow;
		maxCol = col > maxCol ? col : maxCol;
	}

	int totalRows = maxRow + 1;
	int totalCols = maxCol + 1;

	/* Check if tetro goes out of the borders */
	int minOccupiedRow = totalRows; /* It means nothing is occupied yet */
	int minOccupiedCol = totalCols;
	bool isHorizontal = maxCol > maxRow;

	int diff;
	if (isHorizontal)
	{
		diff = (offsetCol + maxCol) - cols;
		diff = diff >= 0 ? diff + 1 : 0;
		minOccupiedCol -= diff;
	}
	else
	{
		diff = (offsetRow + maxRow) - rows;
		diff = diff >= 0 ? diff + 1 : 0;
		minOccupiedRow -= diff;
	}

	for (int i = 0; i < activeTetromino->COORDINATES_NUM; i++)
	{
		int row = nextCoordinates->at(i)[0];
		int col = nextCoordinates->at(i)[1];

		if (occupancyMatrix[(row + offsetRow) * cols + (col + offsetCol)].occupied)
		{
			if (isHorizontal)
				minOccupiedCol = col < minOccupiedCol ? col : minOccupiedCol;
			else
				minOccupiedRow = row < minOccupiedRow ? row : minOccupiedRow;
		}
	}

	int nextOffsetRow = offsetRow - (totalRows - minOccupiedRow);
	int nextOffsetCol = offsetCol - (totalCols - minOccupiedCol);

	activeTetromino->SetOffsetRow(nextOffsetRow);
	activeTetromino->SetOffsetCol(nextOffsetCol);
}

void Playground::ProcessUserInput(UserInput* userInput)
{
	using namespace TetrominoPackage;

	if (userInput->leftArrowPressed && CanMoveTetromino(EDirection::LEFT))
	{
		MoveTetromino(EDirection::LEFT);
	}

	if (userInput->rightArrowPressed && CanMoveTetromino(EDirection::RIGHT))
	{
		MoveTetromino(EDirection::RIGHT);
	}

	if (userInput->downArrowPressed)
	{
		if (CanMoveTetromino(EDirection::DOWN))
		{
			MoveTetromino(EDirection::DOWN);
		}
		else
		{
			timePassed = 0;
			isNextStep = false;
			TryPutTetromino();
		}
	}

	if (userInput->upArrowPressed && CanRotateTetromino(ERotationOrder::CLOCKWISE))
	{
		RotateTetromino(ERotationOrder::CLOCKWISE);
	}

	if (userInput->spacePressed)
	{
		while (CanMoveTetromino(EDirection::DOWN))
		{
			MoveTetromino(EDirection::DOWN);
		}
		timePassed = 0;
		isNextStep = false;
		TryPutTetromino();
	}
}

void Playground::PutTetromino()
{
	glm::vec3* color = &activeTetromino->color;

	for (int i = 0; i < activeTetromino->COORDINATES_NUM; i++)
	{
		int idx = activeTetromino->globalCoordinates->at(i)[0] * cols + activeTetromino->globalCoordinates->at(i)[1];
		occupancyMatrix[idx].occupied = true;
		occupancyMatrix[idx].color = glm::vec3(color->x, color->y, color->z);
	}
}

bool Playground::ShouldGameEnd(bool shouldPut)
{
	if (shouldPut)
	{
		return activeTetromino->GetOffsetRow() < 0;
	}

	for (int k = 0; k < activeTetromino->COORDINATES_NUM; k++)
	{
		int i = activeTetromino->globalCoordinates->at(k)[0];
		int j = activeTetromino->globalCoordinates->at(k)[1];

		if ((i > -1) && (occupancyMatrix[i * cols + j].occupied))
		{
			return true;
		}
	}

	return false;
}

void Playground::EndGame()
{
	std::cout << "Game over" << std::endl;
	gameOver = true;
	DeleteTetromino();
}

void Playground::SetRemovingLines()
{
	int offset = activeTetromino->GetOffsetRow();
	int endrow = offset + activeTetromino->Rows();

	for (int row = offset, i = 0; row < endrow; row++, i++)
	{
		removeLinesData.rows[i] = row;
		removeLinesData.totalRows++;
		for (int col = 0; col < cols; col++)
		{
			if (!occupancyMatrix[row * cols + col].occupied)
			{
				removeLinesData.rows[i] = -1;
				removeLinesData.totalRows--;
				break;
			}
		}
	}
}

bool Playground::ShouldRemoveLines()
{
	for (int i = 0; i < 4; i++)
	{
		if (removeLinesData.rows[i] > -1)
		{
			return true;
		}
	}

	return false;
}

void Playground::DeleteTetromino()
{
	delete activeTetromino;
	activeTetromino = nullptr;
}

int Playground::PopMaxRowFromRemovingLines()
{
	int max = 0;

	for (int i = 1; i < 4; i++)
	{
		if (removeLinesData.rows[i] > removeLinesData.rows[max])
		{
			max = i;
		}
	}

	int tmp = removeLinesData.rows[max];
	removeLinesData.rows[max] = -1;
	return tmp;
}

void Playground::DoLinesRemovingWork(float deltaTime)
{
	int row;

	switch (removeLinesData.stage)
	{
	case ERemovingLinesStages::INIT:
		row = 0;
		removeLinesData.cellsColorsDiff = new glm::vec3[removeLinesData.totalRows * cols];

		for (int i = 0; i < 4; i++)
		{
			if (removeLinesData.rows[i] > -1)
			{
				for (int j = 0; j < cols; j++)
				{
					glm::vec3 initColor = occupancyMatrix[removeLinesData.rows[i] * cols + j].color;
					glm::vec3* diff = &removeLinesData.cellsColorsDiff[row * cols + j];
					diff->x = fadeColor.x - initColor.x;
					diff->y = fadeColor.y - initColor.y;
					diff->z = fadeColor.z - initColor.z;
				}
				row++;
			}
		}

		removeLinesData.stage = ERemovingLinesStages::FADE;
		/* No break. Proceed to the fade stage immediately */
	case ERemovingLinesStages::FADE:
		if (removeLinesData.timePassed + deltaTime > removeLinesData.fadeTime)
		{
			for (int i = 0; i < 4; i++)
			{
				if (removeLinesData.rows[i] > -1)
				{
					for (int j = 0; j < cols; j++)
					{
						int idx = removeLinesData.rows[i] * cols + j;
						occupancyMatrix[idx].occupied = false;
						occupancyMatrix[idx].color = emptyCellColor;
					}
				}
			}

			removeLinesData.timePassed += deltaTime - removeLinesData.fadeTime;
			removeLinesData.stage = ERemovingLinesStages::DROP;
		}
		else /* Continue fading */
		{
			row = 0;
			for (int i = 0; i < 4; i++)
			{
				if (removeLinesData.rows[i] > -1)
				{
					for (int j = 0; j < cols; j++)
					{
						glm::vec3* color = &occupancyMatrix[removeLinesData.rows[i] * cols + j].color;
						glm::vec3* diff = &removeLinesData.cellsColorsDiff[row * cols + j];
						color->x += diff->x * (deltaTime / removeLinesData.fadeTime);
						color->y += diff->y * (deltaTime / removeLinesData.fadeTime);
						color->z += diff->z * (deltaTime / removeLinesData.fadeTime);
					}
					row++;
				}
			}

			removeLinesData.timePassed += deltaTime;
		}
		break;
	case ERemovingLinesStages::DROP:
		//std::cout << "Time passed: " << removeLinesData.timePassed << std::endl;
		if (removeLinesData.timePassed + deltaTime >= removeLinesData.dropTime)
		{
			int maxRow = PopMaxRowFromRemovingLines();
			
			for (int i = maxRow - 1; i > 0; i--)
			{
				for (int j = 0; j < cols; j++)
				{
					int idx1 = i * cols + j;
					int idx2 = (i + 1) * cols + j;
					occupancyMatrix[idx2].color = occupancyMatrix[idx1].color;
					occupancyMatrix[idx2].occupied = occupancyMatrix[idx1].occupied;
					occupancyMatrix[idx1].color = emptyCellColor;
					occupancyMatrix[idx1].occupied = false;
				}
			}

			for (int i = 0; i < 4; i++)
			{
				if (removeLinesData.rows[i] > -1)
				{
					removeLinesData.rows[i]++;
				}
			}

			removeLinesData.timePassed -= removeLinesData.dropTime;
			removeLinesData.droppedRows++;
		}

		if (removeLinesData.droppedRows >= removeLinesData.totalRows)
		{
			/* reset data */
			removeLinesData.totalRows = 0;
			removeLinesData.droppedRows = 0;
			removeLinesData.timePassed = 0.0f;
			removeLinesData.stage = ERemovingLinesStages::INIT;
			delete removeLinesData.cellsColorsDiff;

			/* we're done with lines being removed */
			isLinesRemoving = false;
		}

		removeLinesData.timePassed += deltaTime;
		break;
	/* No default */
	}
}

void Playground::TryPutTetromino()
{
	if (ShouldGameEnd(true))
	{
		for (int k = 0; k < activeTetromino->COORDINATES_NUM; k++)
		{
			int i = activeTetromino->globalCoordinates->at(k)[0];
			int j = activeTetromino->globalCoordinates->at(k)[1];
			if (i > -1)
			{
				int idx = i * cols + j;
				occupancyMatrix[idx].occupied = true;
				occupancyMatrix[idx].color = activeTetromino->color;
			}
		}
		EndGame();
		return;
	}
	else
	{
		PutTetromino();
		SetRemovingLines();
		DeleteTetromino();
		isLinesRemoving = ShouldRemoveLines();
	}
}

void Playground::Update(float deltaTime, UserInput* userInput)
{
	if (gameOver)
	{
		/* do gameover stuff */
		return;
	}

	timePassed += deltaTime;

	if (timePassed >= STEP_GAP)
	{
		isNextStep = true;
		timePassed = 0;
	}

	if (isLinesRemoving)
	{
		DoLinesRemovingWork(deltaTime);
		tetrisGrid->Update(nullptr, occupancyMatrix);
		return;
	}

	if (!activeTetromino)
	{
		using namespace TetrominoPackage;
		activeTetromino = nextTetrominos.front();
		nextTetrominos.pop_front();
		nextTetrominos.push_back(new Tetromino(getRandomType(), getRandomOrientation(), getRandomColor(), rows, cols));
	}

	if (ShouldGameEnd(false))
	{
		EndGame();
		return;
	}

	ProcessUserInput(userInput);

	if (isNextStep)
	{
		if (CanMoveTetromino(TetrominoPackage::EDirection::DOWN))
		{
			MoveTetromino(TetrominoPackage::EDirection::DOWN);
		}
		else
		{
			TryPutTetromino();
		}

		isNextStep = false;
	}

	tetrisGrid->Update(activeTetromino, occupancyMatrix);
	ntWindow->Update();
}

void Playground::Render()
{
	ntWindow->Render();
	tetrisGrid->Render();
	
}