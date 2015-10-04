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

// Definindo as cores
#define RED 1.0, 0.0, 0.0
#define BLUE 0.0, 0.0, 1.0
#define BLACK 0.0, 0.0, 0.0

const double green[] = {0.0, 1.0, 0.0};

// Variáveis referentes ao tamanho da tela
float DISPLAY_WIDTH = 0;
float DISPLAY_HEIGHT = 0;

AppSettings* appSettings = new AppSettings();

// Vértices
float globalX = 0.0, globalY = 0.0;


using namespace std;
using namespace tinyxml2;

double* color;

void selectColor (string color) {
	if (color == "green") {
		color[0] = 0.0;
		color[1] = 1.0;
		color[2] = 0.0;
	} else if (color == "white") {
		color[0] = 1.0;
		color[1] = 1.0;
		color[2] = 1.0;
	}
}

void desenharArena() {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
		glVertex3f(appSettings->getArena()->getX(),
					appSettings->getArena()->getY(),
					0.0);
		glVertex3f(appSettings->getArena()->getX() + appSettings->getArena()->getWidth(),
					appSettings->getArena()->getY(),
					0.0);
		glVertex3f(appSettings->getArena()->getX()  + appSettings->getArena()->getWidth(),
					appSettings->getArena()->getY() + appSettings->getArena()->getHeight(),
					0.0);
		glVertex3f(appSettings->getArena()->getX(),
					appSettings->getArena()->getY() + appSettings->getArena()->getHeight(),
					0.0);
	glEnd();
}

void desenharPostoAbastecimento() {
	glColor3f(0.75, 0.75, 0.75);
	glBegin(GL_POLYGON);
		glVertex3f(appSettings->getPostoAbastecimento()->getX(),
					appSettings->getPostoAbastecimento()->getY(),
					0.0);
		glVertex3f(appSettings->getPostoAbastecimento()->getX() + appSettings->getPostoAbastecimento()->getWidth(),
					appSettings->getPostoAbastecimento()->getY(),
					0.0);
		glVertex3f(appSettings->getPostoAbastecimento()->getX()  + appSettings->getPostoAbastecimento()->getWidth(),
					appSettings->getPostoAbastecimento()->getY() + appSettings->getPostoAbastecimento()->getHeight(),
					0.0);
		glVertex3f(appSettings->getPostoAbastecimento()->getX(),
					appSettings->getPostoAbastecimento()->getY() + appSettings->getPostoAbastecimento()->getHeight(),
					0.0);
	glEnd();
}

void desenharJogador() {
	int i = 0;
	int num_segments = 100;
	float radius = appSettings->getJogador()->getR();
	float twicePi = 2*3.142;

	glColor3f(0.0, 1.0, 0.0);

	// Desenhar a parte interna do círculo
	glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
	glVertex3f(appSettings->getJogador()->getCx(), appSettings->getJogador()->getCy(), 0.0); // center of circle
	   for (i = 0; i <= 20; i++)   {
	        glVertex3f (
	            (appSettings->getJogador()->getCx() + (radius * cosf(i * twicePi / 20))),
	            (appSettings->getJogador()->getCy() + (radius * sin(i * twicePi / 20))),
	            0.0);
	    }
	glEnd();

	// Desenhar a parte da superfície do círculo
	glBegin(GL_LINE_LOOP);
		for (i = 0; i < num_segments; i++)   {
	        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);//get the current angle
	        float x = appSettings->getJogador()->getR() * cosf(theta);//calculate the x component
	        float y = appSettings->getJogador()->getR() * sinf(theta);//calculate the y component
	        glVertex3f(x + appSettings->getJogador()->getCx(),
	        			y + appSettings->getJogador()->getCy(),
	        			0.0);//output vertex
	    }
	glEnd();
}

void desenharInimigos() {
	int i = 0;
	int num_segments = 100;
	float radius = 0.0;
	float twicePi = 2*3.142;

	vector<Circle>::iterator it;

	for (it = appSettings->getInimigos().begin(); it != appSettings->getInimigos().end(); it++) {
		radius = it->getR();
		glColor3f(1.0, 0.0, 0.0);
		// Desenhar a parte interna do círculo
		glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
		glVertex3f(it->getCx(), it->getCy(), 0.0); // center of circle
		   for (i = 0; i <= 20; i++)   {
				glVertex3f (
					(it->getCx() + (radius * cosf(i * twicePi / 20))),
					(it->getCy() + (radius * sin(i * twicePi / 20))),
					0.0);
			}
		glEnd();

		// Desenhar a parte da superfície do círculo
		glBegin(GL_LINE_LOOP);
			for (i = 0; i < num_segments; i++)   {
				float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);//get the current angle
				float x = it->getR() * cosf(theta);//calculate the x component
				float y = it->getR() * sinf(theta);//calculate the y component
				glVertex3f(x + it->getCx(),
							y + it->getCy(),
							0.0);//output vertex
			}
		glEnd();
	}
}

void desenharObjetosResgate() {
	int i = 0;
	int num_segments = 100;
	float radius = 0.0;
	float twicePi = 2*3.142;

	vector<Circle>::iterator it;

	for (it = appSettings->getObjetoResgate().begin(); it != appSettings->getObjetoResgate().end(); it++) {
		radius = it->getR();
		glColor3f(0.0, 0.0, 1.0);
		// Desenhar a parte interna do círculo
		glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
		glVertex3f(it->getCx(), it->getCy(), 0.0); // center of circle
		   for (i = 0; i <= 20; i++)   {
				glVertex3f (
					(it->getCx() + (radius * cosf(i * twicePi / 20))),
					(it->getCy() + (radius * sin(i * twicePi / 20))),
					0.0);
			}
		glEnd();

		// Desenhar a parte da superfície do círculo
		glBegin(GL_LINE_LOOP);
			for (i = 0; i < num_segments; i++)   {
				float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);//get the current angle
				float x = it->getR() * cosf(theta);//calculate the x component
				float y = it->getR() * sinf(theta);//calculate the y component
				glVertex3f(x + it->getCx(),
							y + it->getCy(),
							0.0);//output vertex
			}
		glEnd();
	}

}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	desenharArena();
	desenharPostoAbastecimento();
	desenharJogador();
	desenharInimigos();
	desenharObjetosResgate();
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

void detectarArena (float x, float y) {
	if (x > appSettings->getArena()->getX()
		&& x < (appSettings->getArena()->getX() + appSettings->getArena()->getWidth())) {

		if (y > appSettings->getArena()->getY()
			&& y < (appSettings->getArena()->getY() + appSettings->getArena()->getHeight())) {

			cout << "id=" <<  appSettings->getArena()->getId() << "\n";
		}
	}
}

void detectarPostoDeAbastecimento (float x, float y) {
	if (x > appSettings->getPostoAbastecimento()->getX()
		&& x < (appSettings->getPostoAbastecimento()->getX() + appSettings->getPostoAbastecimento()->getWidth())) {
		if (y > appSettings->getPostoAbastecimento()->getY()
			&& y < (appSettings->getPostoAbastecimento()->getY() + appSettings->getPostoAbastecimento()->getHeight())) {

			cout << "id=" <<  appSettings->getPostoAbastecimento()->getId() << "\n";
		}
	}
}

bool internoCircunferencia(float x, float y, Circle* circle) {

	float distX = pow ((circle->getCx() - x), 2.0);
	float distY = pow ((circle->getCy() - y), 2.0);
	float rPow2 = pow (circle->getR(), 2.0);
	float soma = distX + distY;

	if (soma <= rPow2) {
		return true;
	}

	return false;
}

void detectarJogador (float x, float y) {
	if (internoCircunferencia(x, y, appSettings->getJogador())) {
		cout << "id=" <<  appSettings->getJogador()->getId() << "\n";
	}
}

void detectarInimigo (float x, float y) {
	vector<Circle>::iterator it;
	Circle* aux = new Circle();

	for (it = appSettings->getInimigos().begin(); it != appSettings->getInimigos().end(); it++) {
		aux->setCx(it->getCx());
		aux->setCy(it->getCy());
		aux->setFill(it->getFill());
		aux->setId(it->getId());
		aux->setR(it->getR());

		if (internoCircunferencia(x, y, aux)) {
			cout << "id=" <<  aux->getId() << "\n";
		}
	}
}

void detectarObjetoResgate (float x, float y) {
	vector<Circle>::iterator it;
	Circle* aux = new Circle();

	for (it = appSettings->getObjetoResgate().begin(); it != appSettings->getObjetoResgate().end(); it++) {
		aux->setCx(it->getCx());
		aux->setCy(it->getCy());
		aux->setFill(it->getFill());
		aux->setId(it->getId());
		aux->setR(it->getR());

		if (internoCircunferencia(x, y, aux)) {
			cout << "id=" << aux->getId() << "\n";
		}
	}
}


void mouse(int button, int state, int x, int y) {

	if (state == GLUT_DOWN) {
		globalX = (float) x;
		globalY = (float) y;
		system("clear");
		detectarPostoDeAbastecimento(globalX, globalY);
		detectarJogador(globalX, globalY);
		detectarInimigo(x, y);
		detectarObjetoResgate(x, y);
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

	glutCreateWindow("Trabalho 2");

	// Inicializa a janela
	init();
	glutDisplayFunc(display);
	glutIdleFunc(idle);

	glutMouseFunc(mouse);
//	glutMotionFunc(mouseMove);

	glutMainLoop();

	return 0;
}
