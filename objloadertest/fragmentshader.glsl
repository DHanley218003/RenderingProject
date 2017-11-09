#version 330

in vec2 TexCoord0;
in vec4 LightIntensity0;

out vec4 FragColor;
uniform vec4 colour;
uniform sampler2D gTextureSampler;

void main()
{
    FragColor = colour * LightIntensity0;
}