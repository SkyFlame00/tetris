#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in mat4 translation;
layout (location = 6) in vec3 newColor;

out vec3 color;

uniform mat4 projection;
uniform bool isCell;
uniform mat4 backgroundTranslation;

void main()
{
	if (isCell)
	{
		gl_Position = projection * translation * vec4(aPos, 1.0);
		color = newColor;
	}
	else {
		gl_Position = projection * backgroundTranslation * vec4(aPos, 1.0);
		color = aColor;
	}
}