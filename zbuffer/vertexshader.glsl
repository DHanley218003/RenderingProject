#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 Colour;

out vec4 Colour0;

uniform mat4 gModelToWorldTransform;
uniform mat4 gWorldToViewToProjectionTransform;
                                                                   
void main()
{  
	vec4 vertexPositionInModelSpace = vec4(Position, 1);
	vec4 vertexInWorldSpace = gModelToWorldTransform * vertexPositionInModelSpace;
	vec4 vertexInHomogeneousClipSpace = gWorldToViewToProjectionTransform * vertexInWorldSpace;
	gl_Position = vertexInHomogeneousClipSpace;
	Colour0 = Colour;
}