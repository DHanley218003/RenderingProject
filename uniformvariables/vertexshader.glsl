
// Starting
#version 330
                                       
layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 Colour;

out vec4 Colour0;

//uniform float gTransform;
//uniform float gTime;
uniform mat4 gMat4;

void main()
{  
	vec4 pos = vec4(Position.x, Position.y, Position.z, 1.0f);
    gl_Position = gMat4 * pos;
	Colour0 = Colour;
}