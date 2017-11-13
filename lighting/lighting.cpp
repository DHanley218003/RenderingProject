#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Windows.h>
#include <string>

using namespace std;
using namespace glm;

const int windowWidth = 1024; 
const int windowHeight = 768;

GLuint VBO;
int NUMVERTS = 0;

// Transform uniforms location
GLuint gModelToWorldTransformLoc;
GLuint gWorldToViewToProjectionTransformLoc;

// Lighting uniforms location
GLuint gAmbientLightIntensityLoc;
GLuint gDirectionalLightIntensityLoc;
GLuint gDirectionalLightDirectionLoc;

// Materials uniform location
GLuint gKaLoc;
GLuint gKdLoc;

// Our simple vertex container
struct SimpleVertex
{
    vec3		pos;	// Position
	vec3		normal; // Vertex Normal
	vec4		colour;	// Colour
};

static void renderSceneCallBack()
{
	// Clear the back buffer and the z-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Create our world space to view space transformation matrix
	mat4 worldToViewTransform = lookAt(
		vec3(0.0f,0.0f,10.0f), // The position of your camera, in world space
		vec3(0.0f,0.0f,0.0f), // where you want to look at, in world space
		vec3(0.0f,1.0f,0.0f)  // Camera up direction (set to 0,-1,0 to look upside-down)
		); 

    // Create out projection transform
    mat4 projectionTransform = perspective(45.0f, (float)windowWidth / (float)windowHeight, 1.0f, 100.0f);

	// Combine the world space to view space transformation matrix and the projection transformation matrix
	mat4 worldToViewToProjectionTransform = projectionTransform * worldToViewTransform;

    // Update the transforms in the shader program on the GPU
    glUniformMatrix4fv(gWorldToViewToProjectionTransformLoc, 1, GL_FALSE, &worldToViewToProjectionTransform[0][0]);

    glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (const GLvoid*)12);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (const GLvoid*)24);

    // Set the material properties
    glUniform1f(gKaLoc, 0.2); 
    glUniform1f(gKdLoc, 0.8);

	// Draw triangle
	mat4 modelToWorldTransform = mat4(1.0f);
	static float angle = 0.0f;
	angle+=1.0f;
	modelToWorldTransform = rotate(modelToWorldTransform, angle, vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(gModelToWorldTransformLoc, 1, GL_FALSE, &modelToWorldTransform[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NUMVERTS);

    glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

    glutSwapBuffers();
}

static void initializeGlutCallbacks()
{
    glutDisplayFunc(renderSceneCallBack);
    glutIdleFunc(renderSceneCallBack);
}

static void createVertexBuffer()
{
	// Create some vertices to put in our VBO.
	// Create vertex buffer
	NUMVERTS = 24;
	SimpleVertex vertices[] =
	{
        // Side 1 Front
		{vec3(-1.0f, 1.0f, 1.0f), vec3( 0.0f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},
		{vec3( 1.0f, 1.0f, 1.0f), vec3( 0.0f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},
		{vec3( 1.0f,-1.0f, 1.0f), vec3( 0.0f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},
        {vec3(-1.0f, 1.0f, 1.0f), vec3( 0.0f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},
		{vec3( 1.0f,-1.0f, 1.0f), vec3( 0.0f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},
		{vec3(-1.0f,-1.0f, 1.0f), vec3( 0.0f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},

        // Side 2 Right Side
		{vec3( 1.0f, 1.0f, 1.0f), vec3( 1.0f, 0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},
		{vec3( 1.0f, 1.0f,-1.0f), vec3( 1.0f, 0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},
		{vec3( 1.0f,-1.0f,-1.0f), vec3( 1.0f, 0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},
        {vec3( 1.0f, 1.0f, 1.0f), vec3( 1.0f, 0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},
		{vec3( 1.0f,-1.0f,-1.0f), vec3( 1.0f, 0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},
		{vec3( 1.0f,-1.0f, 1.0f), vec3( 1.0f, 0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},

        // Side 3 Back
		{vec3( 1.0f, 1.0f,-1.0f), vec3( 0.0f, 0.0f,-1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},
		{vec3(-1.0f, 1.0f,-1.0f), vec3( 0.0f, 0.0f,-1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},
		{vec3(-1.0f,-1.0f,-1.0f), vec3( 0.0f, 0.0f,-1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},
        {vec3( 1.0f, 1.0f,-1.0f), vec3( 0.0f, 0.0f,-1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},
		{vec3(-1.0f,-1.0f,-1.0f), vec3( 0.0f, 0.0f,-1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},
		{vec3( 1.0f,-1.0f,-1.0f), vec3( 0.0f, 0.0f,-1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},

        // Side 4 Left Side
		{vec3(-1.0f, 1.0f,-1.0f), vec3(-1.0f, 0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},
		{vec3(-1.0f, 1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},
		{vec3(-1.0f,-1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},
        {vec3(-1.0f, 1.0f,-1.0f), vec3(-1.0f, 0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},
		{vec3(-1.0f,-1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},
		{vec3(-1.0f,-1.0f,-1.0f), vec3(-1.0f, 0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)}

	};
    
 	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleVertex) * NUMVERTS, vertices, GL_STATIC_DRAW);
}

static void initLights()
{
	// Setup the ambient light
	vec3 ambientLightIntensity = vec3(1.0f, 1.0f, 1.0f);
	glUniform3fv(gAmbientLightIntensityLoc, 1, &ambientLightIntensity[0]);
	
	// Setup the direactional light
	vec3 directionalLightDirection = vec3(0.0f, 0.0f, -1.0f);
	normalize(directionalLightDirection);
	glUniform3fv(gDirectionalLightDirectionLoc, 1, &directionalLightDirection[0]);
	vec3 directionalLightIntensity = vec3(0.8f, 0.8f, 0.8f);
	glUniform3fv(gDirectionalLightIntensityLoc, 1, &directionalLightIntensity[0]);
}

static void addShader(GLuint shaderProgram, const char* pShaderText, GLenum shaderType)
{
    GLuint shaderObj = glCreateShader(shaderType);

    if (shaderObj == 0) 
	{
        cerr<<"Error creating shader type "<<shaderType<<endl;
        exit(0);
    }

    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0]= strlen(pShaderText);
    glShaderSource(shaderObj, 1, p, Lengths);
    glCompileShader(shaderObj);
    GLint success;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
    if (!success) 
	{
        GLchar InfoLog[1024];
        glGetShaderInfoLog(shaderObj, 1024, NULL, InfoLog);
        cerr<<"Error compiling shader type "<<shaderType<<": "<<InfoLog<<endl;
        exit(1);
    }

    glAttachShader(shaderProgram, shaderObj);
}

const string readFileToString(char* filename)
{
	ifstream file (filename, ios::in);
	if (file.is_open())
	{
        stringstream continut;
        continut << file.rdbuf();
        continut << '\0';
		return continut.str();
	}
    return "";
}

static void buildShaders()
{
    GLuint shaderProgram = glCreateProgram();

    if (shaderProgram == 0) 
	{
        cerr<<"Error creating shader program\n";
        exit(1);
    }

	string VS = readFileToString("vertexShader.glsl");
	string FS = readFileToString("fragmentShader.glsl");

    addShader(shaderProgram, VS.c_str(), GL_VERTEX_SHADER);
    addShader(shaderProgram, FS.c_str(), GL_FRAGMENT_SHADER);

    GLint success = 0;
    GLchar errorLog[1024] = { 0 };

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (success == 0) 
	{
		glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
		cerr<<"Error linking shader program: "<<errorLog<<endl;
        exit(1);
	}

    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
    if (!success) 
	{
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        cerr<<"Error linking shader program: "<<errorLog<<endl;
        exit(1);
    }

    glUseProgram(shaderProgram);

    gModelToWorldTransformLoc = glGetUniformLocation(shaderProgram, "gModelToWorldTransform");
    //assert(gModelToWorldTransformLoc != 0xFFFFFFFF);
    gWorldToViewToProjectionTransformLoc = glGetUniformLocation(shaderProgram, "gWorldToViewToProjectionTransform");
   // assert(gWorldToViewToProjectionTransformLoc != 0xFFFFFFFF);
	gAmbientLightIntensityLoc = glGetUniformLocation(shaderProgram, "gAmbientLightIntensity");
	//assert(gAmbientLightIntensityLoc != 0xFFFFFFFF);
	gDirectionalLightIntensityLoc = glGetUniformLocation(shaderProgram, "gDirectionalLightIntensity");
	//assert(gDirectionalLightIntensityLoc != 0xFFFFFFFF);
	gDirectionalLightDirectionLoc = glGetUniformLocation(shaderProgram, "gDirectionalLightDirection");
	//assert(gDirectionalLightDirectionLoc != 0xFFFFFFFF);
    gKaLoc = glGetUniformLocation(shaderProgram, "gKa");
	//assert(gDirectionalLightDirectionLoc != 0xFFFFFFFF);
    gKdLoc = glGetUniformLocation(shaderProgram, "gKd");
	//assert(gDirectionalLightDirectionLoc != 0xFFFFFFFF);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Lighting");

    initializeGlutCallbacks();

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) 
    {
		cerr<<"Error: "<<glewGetErrorString(res)<<"\n";
		return 1;
    }

	buildShaders();

	// Enable the z-buffer
	glEnable(GL_DEPTH_TEST);
	// GL_LESS - Passes if the incoming depth value is less than the stored depth value
	glDepthFunc(GL_LESS);

	//Enable backface culling
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW); 

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 

	// Initialise lights
	initLights();

	// Create a vertex buffer
	createVertexBuffer();

    glutMainLoop();
    
    return 0;
}