#version 330
                                       
layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 Colour;
layout (location = 2) in vec2 multiplier;
layout (location = 3) in float ColourMul;

out vec4 Colour0;

void main()
{  
    // gl_Position = vec4(Position.x, Position.y, Position.z, 1.0); // normal
	Colour0 =  Colour * ColourMul; // normal
	//Colour0 = vec4(1.0f, 1.0f, 1.0f, 0.0f) - Colour; // invert colour
	//Colour0 = vec4(1.0f, 0.0f, 0.0f, 0.0f) + Colour; // "hot" colour
	gl_Position = vec4(Position.x * multiplier.x, Position.y * multiplier.y, Position.z, 1.0); // size modified
}