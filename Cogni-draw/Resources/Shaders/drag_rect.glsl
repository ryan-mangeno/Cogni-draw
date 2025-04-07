#shader vertex
#version 330 core

layout(location = 0) vec2 aPos;

void main()
{
	gl_position = vec4(aPos, 0.0f, 1.0f);
}


#shader fragment
#version 330 core

out vec4 FragColor

void main()
{
	// blue, alpha is 0.5 can change need to test
	FragColor = vec4(1.0f, 0.0f, 0.0f, 0.5f); 
}
