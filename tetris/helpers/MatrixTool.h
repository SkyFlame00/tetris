#ifndef MATRIX_TOOL_H
#define MATRIX_TOOL_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>

class MatrixTool
{
public:
	static void Print(glm::mat4& matrix);
private:
};

#endif // !MATRIX_TOOL_H