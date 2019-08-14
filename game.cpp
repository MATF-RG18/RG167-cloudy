#include<iostream>
#include<cmath>
#include<math.h>
#include<GL/glut.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
using namespace std;

void drawScene();

//Main funkcija
int main(int argc, char** argv) {
//Inicijalizacija GLUT-a
glutInit(&argc, argv);
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

int w = glutGet(GLUT_SCREEN_WIDTH);
int h = glutGet(GLUT_SCREEN_HEIGHT);
int windowWidth = 800;
int windowHeight = 800;
glutInitWindowSize(windowWidth, windowHeight);
glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);
glutCreateWindow("Cloudy"); 


glutDisplayFunc(drawScene);
glEnable(GL_DEPTH);
glEnable(GL_DEPTH_TEST);        
glEnable(GL_COLOR_MATERIAL);    
glClearColor(0.5, 0.5, 1.0, 0.0);
glutMainLoop();
return 0;
}


void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    glutSwapBuffers();
}