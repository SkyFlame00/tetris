#pragma once

#include <glad/glad.h>
#include <iostream>

void GLError();

class GL
{
public:
	static void PrintAllErrors();
private:
	static char* GetErrorTextByNumber(int error);
};