#version 330 core
in vec3 color;

out vec4 FragColor;

uniform bool pressed;

void main()
{
	if (pressed)
	{
		FragColor = vec4(0.0, 1.0, 1.0, 1.0);
	}
	else
	{
		FragColor = vec4(color, 1.0);
	}
}