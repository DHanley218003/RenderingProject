#version 330

in vec4 Colour0;

out vec4 FragColor;

void main()
{
	FragColor = Colour0; //normal
    //FragColor = vec4(1.0f, 1.0f, 1.0f, 0.0f) - Colour0; // inverted colour
}