#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

GLuint VBO;


static void renderSceneCallBack()
{
    glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f,0.0f,0.0f);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);

    glDrawArrays(GL_POINT, 0, 2);
	glDrawArrays(GL_LINE, 3, 2);
	glDrawArrays(GL_POLYGON, 5, 3);

    glDisableVertexAttribArray(0);

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
	vec3 vertices[7];
    vertices[0] = vec3(1.0f, 0.0f, 0.0f);
	vertices[1] = vec3(0.25f, 0.0f, 0.0f);
	vertices[2] = vec3(1.0f, 1.0f, 1.0f);
	vertices[3] = vec3(-1.0f, -1.0f, -1.0f);
	vertices[4] = vec3(0.0f, 0.0f, 0.0f);
	vertices[5] = vec3(0.0f, 0.5f, 0.0f);
	vertices[6] = vec3(0.5f, 0.0f, 0.0f);

    
 	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * 7, vertices, GL_STATIC_DRAW);
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Vertex Buffer Objects");

    initializeGlutCallbacks();

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) 
    {
		cerr<<"Error: "<<glewGetErrorString(res)<<"\n";
		return 1;
    }

    glClearColor(0.0f, 1.0f, 0.0f, 0.0f); 

	// Create a vertex buffer
	createVertexBuffer();

    glutMainLoop();
    
    return 0;
}