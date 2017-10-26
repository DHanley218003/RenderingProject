/*#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 Colour;

out vec4 Colour0;

uniform mat4 gModelToWorldTransform;
uniform mat4 gWorldToViewTransform;
uniform mat4 gProjectionTransform;
                                                                   
void main()
{  
	vec4 vertexPositionInModelSpace = vec4(Position, 1);
	vec4 vertexInWorldSpace = gModelToWorldTransform * vertexPositionInModelSpace;
	vec4 vertexInViewSpace = gWorldToViewTransform * vertexInWorldSpace;
	vec4 vertexInHomogeneousClipSpace = gProjectionTransform * vertexInViewSpace;
	gl_Position = vertexInHomogeneousClipSpace;
	Colour0 = Colour;
}*/

#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 Colour;

out vec4 Colour0;

uniform mat4 gModelToWorldTransform;
uniform mat4 gWorldToViewTransform;
uniform mat4 gProjectionTransform;

                                                            
void main()
{  
	vec4 vertexPositionInModelSpace = vec4(Position, 1);

	vec4 vertexInWorldSpace = gModelToWorldTransform * vertexPositionInModelSpace;

	vec4 vertexInCameraSpace = gWorldToViewTransform * vertexInWorldSpace;

	vec4 vertexInClipSpace = gProjectionTransform * vertexInCameraSpace;

	gl_Position = vertexInClipSpace;

	Colour0 = Colour;
}