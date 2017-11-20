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

// TextureSampler uniform location
GLuint gTextureSamplerLoc;

// Texture ID
GLuint gTextureObject;

// Our simple vertex container
struct SimpleVertex
{
    vec3		pos;		// Position
	vec2		texcoord;	// Colour
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
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (const GLvoid*)12);

	// Bind the texture to the texture unit 0
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureObject);
	// Set our sampler to user Texture Unit 0
	glUniform1i(gTextureSamplerLoc, 0);

	// Draw triangle
	mat4 modelToWorldTransform = mat4(1.0f);
	static float angle = 0.0f;
	angle+=1.0f;
	modelToWorldTransform = rotate(modelToWorldTransform, angle, vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(gModelToWorldTransformLoc, 1, GL_FALSE, &modelToWorldTransform[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NUMVERTS);

    glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

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
        // Side 1 Front normal
		{vec3(-1.0f, 1.0f, 1.0f), vec2(0.2f, 0.8f)},
		{vec3( 1.0f, 1.0f, 1.0f), vec2(0.8f, 0.8f)},
		{vec3( 1.0f,-1.0f, 1.0f), vec2(0.8f, 0.2f)},
        {vec3(-1.0f, 1.0f, 1.0f), vec2(0.2f, 0.8f)},
		{vec3( 1.0f,-1.0f, 1.0f), vec2(0.8f, 0.2f)},
		{vec3(-1.0f,-1.0f, 1.0f), vec2(0.2f, 0.2f)},

        // Side 2 Right Side	  
		{vec3( 1.0f, 1.0f, 1.0f), vec2(-1.0f, 2.0f)},
		{vec3( 1.0f, 1.0f,-1.0f), vec2(2.0f, 2.0f)},
		{vec3( 1.0f,-1.0f,-1.0f), vec2(2.0f, -1.0f)},
        {vec3( 1.0f, 1.0f, 1.0f), vec2(-1.0f, 2.0f)},
		{vec3( 1.0f,-1.0f,-1.0f), vec2(2.0f, -1.0f)},
		{vec3( 1.0f,-1.0f, 1.0f), vec2(-1.0f, -1.0f)},
								  
        // Side 3 Back			  
		{vec3( 1.0f, 1.0f,-1.0f), vec2(0.0f, 2.0f)},
		{vec3(-1.0f, 1.0f,-1.0f), vec2(2.0f, 2.0f)},
		{vec3(-1.0f,-1.0f,-1.0f), vec2(2.0f, 0.0f)},
        {vec3( 1.0f, 1.0f,-1.0f), vec2(0.0f, 2.0f)},
		{vec3(-1.0f,-1.0f,-1.0f), vec2(2.0f, 0.0f)},
		{vec3( 1.0f,-1.0f,-1.0f), vec2(0.0f, 0.0f)},
								  
        // Side 4 Left Side		  
		{vec3(-1.0f, 1.0f,-1.0f), vec2(0.0f, 1.0f)},
		{vec3(-1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)},
		{vec3(-1.0f,-1.0f, 1.0f), vec2(1.0f, 0.0f)},
        {vec3(-1.0f, 1.0f,-1.0f), vec2(0.0f, 1.0f)},
		{vec3(-1.0f,-1.0f, 1.0f), vec2(1.0f, 0.0f)},
		{vec3(-1.0f,-1.0f,-1.0f), vec2(0.0f, 0.0f)}

	};
    
 	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleVertex) * NUMVERTS, vertices, GL_STATIC_DRAW);
}

static void initTexture(const char *imagepath)
{
	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath,"rb");
	if (!file)
	{
		printf("%s could not be opened. \n", imagepath); getchar();
	}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if ( fread(header, 1, 54, file)!=54 ){ 
		printf("Not a correct BMP file\n");
		return;
	}
	// A BMP files always begins with "BM"
	if ( header[0]!='B' || header[1]!='M' ){
		printf("Not a correct BMP file\n");
		return;
	}
	// Make sure this is a 24bpp file
	if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    return;}
	if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    return;}

	// Read the information about the image
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char [imageSize];

	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);

	// Everything is in memory now, the file wan be closed
	fclose (file);


	// Create one OpenGL texture
	glGenTextures(1, &gTextureObject);

	// Set the active texture to texture imaging unit 0
	glActiveTexture(GL_TEXTURE0);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, gTextureObject);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// OpenGL has now copied the data. Free our own version.
	delete [] data;

	// Poor filtering, or ...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// ... nice trilinear filtering.
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	//glGenerateMipmap(GL_TEXTURE_2D);
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

const string readFileToString(const char* filename)
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
    assert(gModelToWorldTransformLoc != 0xFFFFFFFF);
    gWorldToViewToProjectionTransformLoc = glGetUniformLocation(shaderProgram, "gWorldToViewToProjectionTransform");
    assert(gWorldToViewToProjectionTransformLoc != 0xFFFFFFFF);
	gTextureSamplerLoc = glGetUniformLocation(shaderProgram, "gTextureSampler");
	assert(gTextureSamplerLoc != 0xFFFFFFFF);
}



int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Texture Mapping");

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

	// Initialise texture
	initTexture("texture.bmp");

	// Create a vertex buffer
	createVertexBuffer();

    glutMainLoop();
    
    return 0;
}