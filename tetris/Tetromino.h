#ifndef TETROMINO_H
#define TETROMINO_H

#include <unordered_map>
#include <glm/glm.hpp>
#include <array>

namespace TetrominoPackage
{
	enum class EType
	{
		I,
		T,
		L,
		J,
		O,
		Z,
		S
	};

	enum class EColor
	{
		PURPLE,
		RED,
		YELLOW,
		GREEN,
		BLUE
	};

	enum class EOrientation
	{
		UPWARD,
		DOWNWARD,
		LEFTWARD,
		RIGHTWARD
	};

	enum class EDirection
	{
		DOWN,
		LEFT,
		RIGHT
	};

	enum class ERotationOrder
	{
		CLOCKWISE,
		COUNTER_CLOCKWISE
	};

	/* Specific hashmap type for storing various tetraminos' positions */
	typedef std::unordered_map<
		EType,
		std::unordered_map<EOrientation, std::array<glm::vec2, 4>>
	> tetrpositions_t;

	class Tetromino
	{
	public:
		Tetromino(EType type, EOrientation orientation, EColor color, int rows, int cols);
		~Tetromino();
		
		void Rotate(bool clockwise);
		void Move(EDirection direction);
		void Drop();
		void SetOffsetRow(int row);
		void SetOffsetCol(int col);
		int GetOffsetRow();
		int GetOffsetCol();
		void SetGlobalCoordinates(int offsetRow, int offsetCol);
		std::array<glm::vec2, 4>* GetGlobalCoordinates();
		int Rows();
		int Cols();

		static const int COORDINATES_NUM = 4;
		static tetrpositions_t types;
		static std::unordered_map<
			ERotationOrder,
			std::unordered_map<EOrientation, EOrientation>
		> rotationMapping;

		EType type;
		EOrientation orientation;
		glm::vec3 color;
		std::array<glm::vec2, 4>* coordinates;
		std::array<glm::vec2, 4>* globalCoordinates;
	private:
		void SetOrientation(EOrientation orientation);

		int offsetRow, offsetCol;
		int gridRows, gridCols;
	};

	EType getRandomType();
	EOrientation getRandomOrientation();
	EColor getRandomColor();
}

#endif // !TETROMINO_H
