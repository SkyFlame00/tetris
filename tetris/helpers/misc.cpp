#include "misc.h"

void GLError()
{
	std::cout << "Error: " << glGetError() << std::endl;
}

char* GL::GetErrorTextByNumber(int error)
{
	switch (error)
	{
	case GL_NO_ERROR:
		return (char*)"No error";
	case GL_INVALID_ENUM:
		return (char*)"Invalid enum";
	case GL_INVALID_VALUE:
		return (char*)"Invalid value";
	case GL_INVALID_OPERATION:
		return (char*)"Invalid operation";
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return (char*)"Invalid framebuffer operation";
	case GL_OUT_OF_MEMORY:
		return (char*)"Out of memory";
	}
}

void GL::PrintAllErrors()
{
	int error;
	while ((error = glGetError()) > 0)
	{
		std::cout << "Error: " << GL::GetErrorTextByNumber(error) << std::endl;
	}
}