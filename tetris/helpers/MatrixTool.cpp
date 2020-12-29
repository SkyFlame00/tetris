#include "MatrixTool.h"

void MatrixTool::Print(glm::mat4& matrix)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << glm::column(matrix, i)[j] << " ";
		}
		std::cout << std::endl;
	}
}