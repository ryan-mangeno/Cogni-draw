#shader vertex
#version 330 core

layout(location = 0) in vec2 aPos;

void main()
{
	gl_Position = vec4(aPos, 0.0f, 1.0f);
}


#shader fragment
#version 330 core

out vec4 FragColor;

void main()
{
	// blue, alpha is 0.4 can change need to test
	FragColor = vec4(0.0f, 0.478f, 1.0f, 0.4f);
}
