/*
 * main.cpp
 *
 *  Created on: 13/09/2015
 *      Author: rodrigo
 */

#include "tinyxml2.h"
#include "AppSettings.h"
#include "Rectangle.h"
#include "Circle.h"

#include <cstdlib>
#include <stdlib.h>
#include <cstring>
#include <ctime>
#include <math.h>

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>


// Variáveis referentes ao tamanho da tela
float DISPLAY_WIDTH = 0;
float DISPLAY_HEIGHT = 0;

AppSettings* appSettings = new AppSettings();

// Vértices
float globalX = 0.0, globalY = 0.0;


using namespace std;
using namespace tinyxml2;

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	appSettings->desenharObjetos();
	glutSwapBuffers();
}


void idle(void) {

}

void init(void) {
	// Selecionar cor de fundo (preto)
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Inicializar sistema de viz
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0.0, 0.0, 1.0);
}

void mouse(int button, int state, int x, int y) {

	if (state == GLUT_DOWN) {
		globalX = (float) x;
		globalY = (float) y;
		appSettings->detectarObjetos(x, y);
	}
}

int main(int argc, char** argv) {
	appSettings->loadConfigXML(argv);
	appSettings->loadSvgFile();
	//appSettings->showValues();

	// Iniciando tela e demais variáveis
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// Setando variáveis da tela
	DISPLAY_WIDTH = appSettings->getArena()->getWidth();
	DISPLAY_HEIGHT = appSettings->getArena()->getHeight();

	glutInitWindowSize(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("Trabalho 3");

	// Inicializa a janela
	init();
	glutDisplayFunc(display);
	glutIdleFunc(idle);

	glutMouseFunc(mouse);
//	glutMotionFunc(mouseMove);

	glutMainLoop();

	return 0;
}
