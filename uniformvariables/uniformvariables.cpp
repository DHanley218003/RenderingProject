#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Windows.h>
#include "aiterror.h"
#include <ctime>

using namespace std;
using namespace glm;

int old_t;
float dt;
GLuint VBO;
GLuint gTransformLocation;
GLuint gColourModifier;
GLuint gTime;
GLuint gMat4;
mat4 MVP = mat4(2.0f);
const int NUMVERTS = 3;
clock_t startTime;
double secondsPassed;

// Our simple vertex container
struct SimpleVertex
{
    vec3		pos;	// Position
	vec4		colour;	// Colour
};

void init(void) {
	old_t = glutGet(GLUT_ELAPSED_TIME);
	clock_t startTime = clock(); //Start timer
}

void idle(void) {
	secondsPassed = (clock() - startTime) / CLOCKS_PER_SEC;
	int t;
	/* Delta time in seconds. */
	t = glutGet(GLUT_ELAPSED_TIME);
	dt = (t - old_t) / 1000.0;
	old_t = t;
	glutPostRedisplay();
}

static void renderSceneCallBack()
{
	idle();
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (const GLvoid*)12);

	static float transform = 0.0f;
	transform += dt * 1;
	//for (unsigned int i = 1; i <= 10; i++)
	
		//glUniform1f(gTransformLocation, sin(transform+i));
		//glUniform1f(gColourModifier, sin(transform+i));
		//glUniform1f(gTime, GLUT_ELAPSED_TIME);
	MVP = translate(mat4(1.0f),vec3(transform,0.0f,0.0f));
	glUniformMatrix4fv(gMat4, 1, GL_FALSE, &MVP[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NUMVERTS);
	
    glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1); 

    glutSwapBuffers();
}



static void initializeGlutCallbacks()
{
	init();
    glutDisplayFunc(renderSceneCallBack);
    glutIdleFunc(renderSceneCallBack);
}

static void createVertexBuffer()
{
	// Create some vertices to put in our VBO.
	// Create vertex buffer
	SimpleVertex vertices[] =
	{
		{vec3(-0.5f, -0.5f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},
		{vec3(0.5f, -0.5f, 0.0f),  vec4(0.0f, 1.0f, 0.0f, 1.0f)},
		{vec3( 0.0f, 0.5f, 0.0f),  vec4(0.0f, 0.0f, 1.0f, 1.0f)}
	};
    
 	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleVertex) * 3, vertices, GL_STATIC_DRAW);
}

static void addShader(GLuint shaderProgram, const char* pShaderText, GLenum shaderType)
{
    GLuint shaderObj = glCreateShader(shaderType);

	if (shaderObj == 0) 
	{
		std::stringstream ss;
		ss<<"Error creating shader type "<<shaderType<<endl;
		AIT_ASSERT(0, ss.str());
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
		std::stringstream ss;
		ss <<"Error compiling shader type "<<shaderType<<": "<<InfoLog<<endl;
		AIT_ASSERT(0, ss.str());
    }

    glAttachShader(shaderProgram, shaderObj);
}

const char* readFileToBuffer(char* filename)
{
	ifstream file (filename, ios::in);
	if (file.is_open())
	{
		file.seekg(0, ios::end);
		unsigned int size = (unsigned int)file.tellg();
		char *contents = new char [size+1];
		file.seekg (0, ios::beg);
		file.read (contents, size);
		file.close();
		contents[size] = 0;
		return contents;
	}
    return NULL;
}

static void buildShaders()
{
    GLuint shaderProgram = glCreateProgram();

    if (shaderProgram == 0) 
	{
		AIT_ASSERT(0, "Error creating shader program\n");
    }

	const char* pVS = readFileToBuffer("vertexShader.glsl");
	const char* pFS = readFileToBuffer("fragmentShader.glsl");

    addShader(shaderProgram, pVS, GL_VERTEX_SHADER);
    addShader(shaderProgram, pFS, GL_FRAGMENT_SHADER);

	delete [] pVS;
	delete [] pFS;

    GLint success = 0;
    GLchar errorLog[1024] = { 0 };

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
 	if (success == 0) 
	{
		glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
		std::stringstream ss;
		ss<<"Error linking shader program: "<<errorLog<<endl;
		AIT_ASSERT(0, ss.str());
	}

    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
    if (!success) 
	{
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        cerr<<"Error linking shader program: "<<errorLog<<endl;
        std::stringstream ss;
		ss<<"Error linking shader program: "<<errorLog<<endl;
		AIT_ASSERT(0, ss.str());
    }

    glUseProgram(shaderProgram);

    //gTransformLocation = glGetUniformLocation(shaderProgram, "gTransform");
    //assert(gTransformLocation != 0xFFFFFFFF);
	//gColourModifier = glGetUniformLocation(shaderProgram, "gColourMod");
	//assert(gColourModifier != 0xFFFFFFFF);
	//gTime = glGetUniformLocation(shaderProgram, "gTime");
	//assert(gTime != 0xFFFFFFFF);
	gMat4 = glGetUniformLocation(shaderProgram, "gMat4");
	assert(gTime != 0xFFFFFFFF);
}

int main(int argc, char** argv)
{


	//create matrix4, send to vertexshader and multipy every vertex by this matrix
	
	
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Uniform Variables");
	init();
    initializeGlutCallbacks();

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) 
    {
		cerr<<"Error: "<<glewGetErrorString(res)<<"\n";
		return 1;
    }

	buildShaders();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 

	// Create a vertex buffer
	createVertexBuffer();

    glutMainLoop();
    return 0;
}

/*
//Starting
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <Windows.h>
#include "aiterror.h"

using namespace std;
using namespace glm;

GLuint VBO;
GLuint gScaleLocation;
const int NUMVERTS = 3;

// Our simple vertex container
struct SimpleVertex
{
    vec3		pos;	// Position
	vec4		colour;	// Colour
};

static void renderSceneCallBack()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Update the uniform varaible
    static float scale = 0.0f;
    scale += 0.01f;
    glUniform1f(gScaleLocation, sinf(scale));

    glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (const GLvoid*)12);

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
	SimpleVertex vertices[] =
	{
		{vec3(-0.5f, -0.5f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)},
		{vec3(0.5f, -0.5f, 0.0f),  vec4(0.0f, 1.0f, 0.0f, 1.0f)},
		{vec3( 0.0f, 0.5f, 0.0f),  vec4(0.0f, 0.0f, 1.0f, 1.0f)}
	};
    
 	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleVertex) * 3, vertices, GL_STATIC_DRAW);
}

static void addShader(GLuint shaderProgram, const char* pShaderText, GLenum shaderType)
{
    GLuint shaderObj = glCreateShader(shaderType);

	if (shaderObj == 0) 
	{
		std::stringstream ss;
		ss<<"Error creating shader type "<<shaderType<<endl;
		AIT_ASSERT(0, ss);
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
		std::stringstream ss;
		ss <<"Error compiling shader type "<<shaderType<<": "<<InfoLog<<endl;
		AIT_ASSERT(0, ss.str());
    }

    glAttachShader(shaderProgram, shaderObj);
}

const char* readFileToBuffer(char* filename)
{
	ifstream file (filename, ios::in);
	if (file.is_open())
	{
		file.seekg(0, ios::end);
		unsigned int size = (unsigned int)file.tellg();
		char *contents = new char [size+1];
		file.seekg (0, ios::beg);
		file.read (contents, size);
		file.close();
		contents[size] = 0;
		return contents;
	}
    return NULL;
}

static void buildShaders()
{
    GLuint shaderProgram = glCreateProgram();

    if (shaderProgram == 0) 
	{
		AIT_ASSERT(0, "Error creating shader program\n");
    }

	const char* pVS = readFileToBuffer("vertexShader.glsl");
	const char* pFS = readFileToBuffer("fragmentShader.glsl");

    addShader(shaderProgram, pVS, GL_VERTEX_SHADER);
    addShader(shaderProgram, pFS, GL_FRAGMENT_SHADER);

	delete [] pVS;
	delete [] pFS;

    GLint success = 0;
    GLchar errorLog[1024] = { 0 };

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
 	if (success == 0) 
	{
		glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
		std::stringstream ss;
		ss<<"Error linking shader program: "<<errorLog<<endl;
		AIT_ASSERT(0, ss);
	}

    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
    if (!success) 
	{
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        cerr<<"Error linking shader program: "<<errorLog<<endl;
        std::stringstream ss;
		ss<<"Error linking shader program: "<<errorLog<<endl;
		AIT_ASSERT(0, ss);
    }

    glUseProgram(shaderProgram);

    gScaleLocation = glGetUniformLocation(shaderProgram, "gScale");
    assert(gScaleLocation != 0xFFFFFFFF);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Uniform Variables");

    initializeGlutCallbacks();

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) 
    {
		cerr<<"Error: "<<glewGetErrorString(res)<<"\n";
		return 1;
    }

	buildShaders();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 

	// Create a vertex buffer
	createVertexBuffer();

    glutMainLoop();
    
    return 0;
}*/