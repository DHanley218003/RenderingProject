#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace std;

static void renderSceneCallBack()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

static void processNormalKeys(unsigned char key, int x, int y) 
{

	if (key == 'q')
	{
		 glClearColor(1.0f, 0.0f, 0.0f, 0.0f); 
	}
	else if (key == 'w')
	{
		 glClearColor(1.0f, 1.0f, 0.0f, 0.0f); 
	}
	else if (key == 'e')
	{
		 glClearColor(0.0f, 1.0f, 0.0f, 0.0f); 
	}
	else if (key == 'r')
	{
		 glClearColor(0.0f, 1.0f, 1.0f, 0.0f); 
	}
	else if (key == 't')
	{
		 glClearColor(0.0f, 0.0f, 1.0f, 0.0f); 
	}
	else if (key == 'y')
	{
		 glClearColor(1.0f, 0.0f, 1.0f, 0.0f); 
	}
}

static void initializeGlutCallbacks()
{
    glutDisplayFunc(renderSceneCallBack);
	glutIdleFunc(renderSceneCallBack);
	glutKeyboardFunc(processNormalKeys);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_ACCUM|GLUT_ALPHA);
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Basic Application");

    initializeGlutCallbacks();

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) 
    {
		cerr<<"Error: "<<glewGetErrorString(res)<<"\n";
		return 1;
    }

    glClearColor(1.0f, 0.0f, 0.0f, 0.0f); 

    glutMainLoop();
    
    return 0;
}