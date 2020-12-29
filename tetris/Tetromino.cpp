#include "Tetromino.h"

using namespace TetrominoPackage;

tetrpositions_t Tetromino::types = {
	{
		/* Tetramino Type-I */
		EType::I, {
			{
				EOrientation::UPWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 0),
					glm::vec2(1, 0),
					glm::vec2(2, 0),
					glm::vec2(3, 0)
				}
			},
			{
				EOrientation::DOWNWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 0),
					glm::vec2(1, 0),
					glm::vec2(2, 0),
					glm::vec2(3, 0)
				}
			},
			{
				EOrientation::LEFTWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 0),
					glm::vec2(0, 1),
					glm::vec2(0, 2),
					glm::vec2(0, 3)
				}
			},
			{
				EOrientation::RIGHTWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 0),
					glm::vec2(0, 1),
					glm::vec2(0, 2),
					glm::vec2(0, 3)
				}
			}
		}
	},
	{
		/* Tetramino Type-T */
		EType::T, {
			{
				EOrientation::UPWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 1),
					glm::vec2(1, 0),
					glm::vec2(1, 1),
					glm::vec2(1, 2)
				}
			},
			{
				EOrientation::DOWNWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 0),
					glm::vec2(0, 1),
					glm::vec2(0, 2),
					glm::vec2(1, 1)
				}
			},
			{
				EOrientation::LEFTWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 1),
					glm::vec2(1, 0),
					glm::vec2(1, 1),
					glm::vec2(2, 1)
				}
			},
			{
				EOrientation::RIGHTWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 0),
					glm::vec2(1, 0),
					glm::vec2(1, 1),
					glm::vec2(2, 0)
				}
			}
		}
	},
	{
		/* Tetramino Type-L */
		EType::L, {
			{
				EOrientation::UPWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 0),
					glm::vec2(1, 0),
					glm::vec2(1, 1),
					glm::vec2(1, 2)
				}
			},
			{
				EOrientation::DOWNWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 0),
					glm::vec2(0, 1),
					glm::vec2(0, 2),
					glm::vec2(1, 2)
				}
			},
			{
				EOrientation::LEFTWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 1),
					glm::vec2(1, 1),
					glm::vec2(2, 1),
					glm::vec2(2, 0)
				}
			},
			{
				EOrientation::RIGHTWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 0),
					glm::vec2(0, 1),
					glm::vec2(1, 0),
					glm::vec2(2, 0)
				}
			}
		}
	},
	{
		/* Tetramino Type-J */
		EType::J, {
			{
				EOrientation::UPWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 0),
					glm::vec2(0, 1),
					glm::vec2(0, 2),
					glm::vec2(1, 0)
				}
			},
			{
				EOrientation::DOWNWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 2),
					glm::vec2(1, 0),
					glm::vec2(1, 1),
					glm::vec2(1, 2)
				}
			},
			{
				EOrientation::LEFTWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 0),
					glm::vec2(1, 0),
					glm::vec2(2, 0),
					glm::vec2(2, 1)
				}
			},
			{
				EOrientation::RIGHTWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 0),
					glm::vec2(0, 1),
					glm::vec2(1, 1),
					glm::vec2(2, 1)
				}
			}
		}
	},
	{
		/* Tetramino Type-O */
		EType::O, {
			{
				EOrientation::UPWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 0),
					glm::vec2(0, 1),
					glm::vec2(1, 0),
					glm::vec2(1, 1)
				}
			},
			{
				EOrientation::DOWNWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 0),
					glm::vec2(0, 1),
					glm::vec2(1, 0),
					glm::vec2(1, 1)
				}
			},
			{
				EOrientation::LEFTWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 0),
					glm::vec2(0, 1),
					glm::vec2(1, 0),
					glm::vec2(1, 1)
				}
			},
			{
				EOrientation::RIGHTWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 0),
					glm::vec2(0, 1),
					glm::vec2(1, 0),
					glm::vec2(1, 1)
				}
			}
		}
	},
	{
		/* Tetramino Type-Z */
		EType::Z, {
			{
				EOrientation::UPWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 0),
					glm::vec2(0, 1),
					glm::vec2(1, 1),
					glm::vec2(1, 2)
				}
			},
			{
				EOrientation::DOWNWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 0),
					glm::vec2(0, 1),
					glm::vec2(1, 1),
					glm::vec2(1, 2)
				}
			},
			{
				EOrientation::LEFTWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 1),
					glm::vec2(1, 0),
					glm::vec2(1, 1),
					glm::vec2(2, 0)
				}
			},
			{
				EOrientation::RIGHTWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 1),
					glm::vec2(1, 0),
					glm::vec2(1, 1),
					glm::vec2(2, 0)
				}
			}
		}
	},
	{
		/* Tetramino Type-S */
		EType::S, {
			{
				EOrientation::UPWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 1),
					glm::vec2(0, 2),
					glm::vec2(1, 0),
					glm::vec2(1, 1)
				}
			},
			{
				EOrientation::DOWNWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 1),
					glm::vec2(0, 2),
					glm::vec2(1, 0),
					glm::vec2(1, 1)
				}
			},
			{
				EOrientation::LEFTWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 0),
					glm::vec2(1, 0),
					glm::vec2(1, 1),
					glm::vec2(2, 1)
				}
			},
			{
				EOrientation::RIGHTWARD,
				std::array<glm::vec2, 4>{
					glm::vec2(0, 0),
					glm::vec2(1, 0),
					glm::vec2(1, 1),
					glm::vec2(2, 1)
				}
			}
		}
	}
};

std::unordered_map<
	ERotationOrder,
	std::unordered_map<EOrientation, EOrientation>
> TetrominoPackage::Tetromino::rotationMapping = {
	{
		ERotationOrder::CLOCKWISE,
		{
			{ EOrientation::UPWARD, EOrientation::RIGHTWARD },
			{ EOrientation::RIGHTWARD, EOrientation::DOWNWARD },
			{ EOrientation::DOWNWARD, EOrientation::LEFTWARD },
			{ EOrientation::LEFTWARD, EOrientation::UPWARD }
		}
	},
	{
		ERotationOrder::COUNTER_CLOCKWISE,
		{
			{ EOrientation::UPWARD, EOrientation::LEFTWARD },
			{ EOrientation::LEFTWARD, EOrientation::DOWNWARD },
			{ EOrientation::DOWNWARD, EOrientation::RIGHTWARD },
			{ EOrientation::RIGHTWARD, EOrientation::UPWARD }
		}
	}
};

EType TetrominoPackage::getRandomType()
{
	int n = ((float)rand() / (float)RAND_MAX) * 7.0f;

	switch (n)
	{
	case 0:
		return EType::T;
	case 1:
		return EType::J;
	case 2:
		return EType::L;
	case 3:
		return EType::O;
	case 4:
		return EType::S;
	case 5:
		return EType::Z;
	case 6:
		return EType::I;
	default:
		/* No default */
		break;
	}
}

EOrientation TetrominoPackage::getRandomOrientation()
{
	int n = ((float)rand() / (float)RAND_MAX) * 4.0f;

	switch (n)
	{
	case 0:
		return EOrientation::DOWNWARD;
	case 1:
		return EOrientation::LEFTWARD;
	case 2:
		return EOrientation::RIGHTWARD;
	case 3:
		return EOrientation::UPWARD;
	default:
		/* No default */
		break;
	}
}

EColor TetrominoPackage::getRandomColor()
{
	short n = ((float)rand() / (float)RAND_MAX) * 5.0f;

	switch (n)
	{
	case 0:
		return EColor::BLUE;
	case 1:
		return EColor::GREEN;
	case 2:
		return EColor::PURPLE;
	case 3:
		return EColor::RED;
	case 4:
		return EColor::YELLOW;
	default:
		/* No default */
		break;
	}
}

Tetromino::Tetromino(EType type, EOrientation orientation, EColor color, int gridRows, int gridCols)
	: type(type),
	  orientation(orientation),
	  gridRows(gridRows),
	  gridCols(gridCols)
{
	coordinates = &types[type][orientation];

	switch (color)
	{
	case EColor::BLUE:
		this->color = glm::vec3(0.0f, 0.0f, 1.0f);
		break;
	case EColor::GREEN:
		this->color = glm::vec3(0.0f, 1.0f, 0.0f);
		break;
	case EColor::RED:
		this->color = glm::vec3(1.0f, 0.0f, 0.0f);
		break;
	case EColor::PURPLE:
		this->color = glm::vec3(1.0f, 0.0f, 1.0f);
		break;
	case EColor::YELLOW:
		this->color = glm::vec3(1.0f, 1.0f, 0.0f);
		break;
	/* No default */
	}

	/* Set global coordinates (regarding game grid) */
	int tetrCols = Cols();
	int tetrRows = Rows();
	offsetRow = -(tetrRows - 1);
	offsetCol = (gridCols / 2) - (tetrCols / 2);
	globalCoordinates = new std::array<glm::vec2, 4>;
	SetGlobalCoordinates(offsetRow, offsetCol);
}

Tetromino::~Tetromino()
{
	delete globalCoordinates;
}

void Tetromino::SetGlobalCoordinates(int offsetRow, int offsetCol)
{
	globalCoordinates->at(0) = glm::vec2(
		coordinates->at(0)[0] + offsetRow,
		coordinates->at(0)[1] + offsetCol
	);
	globalCoordinates->at(1) = glm::vec2(
		coordinates->at(1)[0] + offsetRow,
		coordinates->at(1)[1] + offsetCol
	);
	globalCoordinates->at(2) = glm::vec2(
		coordinates->at(2)[0] + offsetRow,
		coordinates->at(2)[1] + offsetCol
	);
	globalCoordinates->at(3) = glm::vec2(
		coordinates->at(3)[0] + offsetRow,
		coordinates->at(3)[1] + offsetCol
	);
}

int Tetromino::Rows()
{
	int max = 0;

	for (int k = 0; k < COORDINATES_NUM; k++)
	{
		int i = coordinates->at(k)[0];
		max = i > max ? i : max;
	}

	return max + 1;
}

int Tetromino::Cols()
{
	int max = 0;

	for (int k = 0; k < COORDINATES_NUM; k++)
	{
		int j = coordinates->at(k)[1];
		max = j > max ? j : max;
	}

	return max + 1;
}

void Tetromino::SetOffsetRow(int row)
{
	offsetRow = row;
	SetGlobalCoordinates(offsetRow, offsetCol);
}

void Tetromino::SetOffsetCol(int col)
{
	offsetCol = col;
	SetGlobalCoordinates(offsetRow, offsetCol);
}

int Tetromino::GetOffsetRow()
{
	return offsetRow;
}

int Tetromino::GetOffsetCol()
{
	return offsetCol;
}