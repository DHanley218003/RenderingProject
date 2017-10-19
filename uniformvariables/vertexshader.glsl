
// Starting
#version 330
                                       
layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 Colour;

out vec4 Colour0;

uniform float gTransform;
uniform float gTime;

void main()
{  
    gl_Position = vec4((sin(Position.x + gTime * 0.01) * 0.25) + gTransform, Position.y, Position.z, 1.0);
	Colour0 = Colour;
}