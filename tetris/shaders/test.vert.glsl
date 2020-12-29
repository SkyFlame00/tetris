#version 330 core

layout (location = 0) in vec4 aPos;

//out vec3 color;

void main()
{
	gl_Position = vec4(aPos.xy, 0.0, 1.0);
	//color = aColor;
}