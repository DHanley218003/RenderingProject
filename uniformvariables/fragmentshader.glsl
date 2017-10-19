#version 330

in vec4 Colour0;
uniform float gColourMod;
out vec4 FragColor;

void main()
{
    FragColor = Colour0 + gColourMod;
}