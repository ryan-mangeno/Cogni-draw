#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;

out vec4 Color;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * vec4(aPos, 1.0f);
	Color = aColor;
}


#shader fragment
#version 330 core


in vec4 Color;
out vec4 FragColor;

void main()
{
	FragColor = Color;
}
