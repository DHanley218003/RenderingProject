#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;

out vec2 TexCoord0;

// Transforms
uniform mat4 gModelToWorldTransform;
uniform mat4 gWorldToViewToProjectionTransform;
                                                                   
void main()
{  
	// Transform the vertex from local space to homogeneous clip space 
	vec4 vertexPositionInModelSpace = vec4(Position, 1);
	vec4 vertexInWorldSpace = gModelToWorldTransform * vertexPositionInModelSpace;
	vec4 vertexInHomogeneousClipSpace = gWorldToViewToProjectionTransform * vertexInWorldSpace;
	gl_Position = vertexInHomogeneousClipSpace;

	// Pass the texture coordinate down the pipeline
	TexCoord0 = TexCoord;  
}