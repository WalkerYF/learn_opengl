#version 330 core
out vec4 FragColor;
in vec4 outColor;
void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = outColor;
}