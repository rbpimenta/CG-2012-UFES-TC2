/*
 * AppSettings.cpp
 *
 *  Created on: 13/09/2015
 *      Author: rodrigo
 */

#include "AppSettings.h"
#include "tinyxml2.h"
#include "Rectangle.h"
#include "Circle.h"

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>

using namespace std;
using namespace tinyxml2;

AppSettings::AppSettings() {
	// TODO Auto-generated constructor stub
	this->pathArena = "";
	this->arena = new Rectangle();
	this->postoAbastecimento = new Rectangle();
	this->jogador = new Circle();

	this->quantidadeInimigos = 0;
	this->quantidadeObjetosResgate = 0;
}

AppSettings::~AppSettings() {
	// TODO Auto-generated destructor stub
}

/*
 * irá carregar o nome do arquivo SVG
 */
void AppSettings::loadConfigXML(char** path) {
	// Carregar arquivo XML principal
	XMLDocument* doc = new XMLDocument();

	// Buscando arquivo de configuração
	string strPath = path[1];
	strPath = strPath + "config.xml";

	const char* pFilename = NULL;
	pFilename = strPath.data();

	if (doc->LoadFile(pFilename) == XML_ERROR_FILE_NOT_FOUND) {
		printf("Erro na hora de abrir arquivo! Finalizando programa...\n");
		exit(1);
	}

	XMLElement* aplicacao = doc->FirstChildElement("aplicacao");
	if (aplicacao == NULL) {
		printf(
				"Erro na hora de encontrar Element 'aplicacao'! Finalizando programa...\n");
		exit(1);
	}

	XMLElement* arquivoEntrada = aplicacao->FirstChildElement(
			"arquivosDeEntrada");
	if (arquivoEntrada == NULL) {
		printf(
				"Erro na hora de encontrar Element 'poligono'! Finalizando programa...\n");
		exit(1);
	}

	// Definindo o nome do arquivo da arena
	XMLElement* arquivoDaArena = arquivoEntrada->FirstChildElement(
			"arquivoDaArena");

	const char* nome = "";
	const char* tipo = "";
	const char* caminho = "";

	nome = arquivoDaArena->Attribute("nome");
	tipo = arquivoDaArena->Attribute("tipo");
	caminho = arquivoDaArena->Attribute("caminho");

	string nomeSvg(nome);
	string tipoSvg(tipo);
	string caminhoSvg(caminho);
	string total = caminhoSvg + nomeSvg + "." + tipoSvg;

	this->pathArena = total;
	doc->~XMLDocument();
}

void AppSettings::loadSvgFile() {
	XMLDocument* doc = new XMLDocument();

	doc->LoadFile(this->pathArena.data());

	if (doc == NULL) {
		cout << "Problema ao abrir arquivo .svg\n";
		exit(1);
	}

	XMLElement* svg = doc->FirstChildElement("svg");
	if (svg == NULL) {
		cout
				<< "Erro na hora de encontrar Element 'svg'! Finalizando programa...\n";
		exit(1);
	}

	XMLElement* rectElem = svg->FirstChildElement("rect");

	if (rectElem == NULL) {
		cout
				<< "Erro na hora de encontrar Element 'rect'! Finalizando programa...\n";
		exit(1);
	}

	// atributos do tipo rect
	string id = ""; // Identificador
	const char* aux = "";

	while (rectElem != NULL) {
		// set Id
		aux = (rectElem->Attribute("id"));
		id = aux;

		// Carregar arena
		if (id == "Arena") {
			this->arena->setValues(rectElem);
		}

		// Carregar Posto de abastecimento
		if (id == "PostoAbastecimento") {
			this->postoAbastecimento->setValues(rectElem);
			this->detectarLimitesArena(this->postoAbastecimento->getX(),
					this->postoAbastecimento->getY(),
					this->postoAbastecimento->getWidth(),
					this->postoAbastecimento->getHeight());

		}

		rectElem = rectElem->NextSiblingElement("rect");
	}

	// Setando valores circle
	XMLElement* circleElem = svg->FirstChildElement("circle");
	if (circleElem == NULL) {
		cout
				<< "Erro na hora de encontrar Element 'circle'! Finalizando programa...\n";
		exit(1);
	}

	while (circleElem != NULL) {
		aux = (circleElem->Attribute("id"));
		id = aux;

		if (id == "Jogador") {
			this->jogador->setValues(circleElem);
			this->checkCircles(this->jogador);
		}

		if (id == "Inimigo") {
			Circle* enemyCircle = new Circle();
			enemyCircle->setValues(circleElem);
			this->checkCircles(enemyCircle);
			this->inimigos.push_back(*enemyCircle);
			this->quantidadeInimigos++;
		}

		if (id == "ObjetoResgate") {
			Circle* rescueObjectCircle = new Circle();
			rescueObjectCircle->setValues(circleElem);
			this->checkCircles(rescueObjectCircle);
			this->objetoResgate.push_back(*rescueObjectCircle);
			this->quantidadeObjetosResgate++;
		}

		circleElem = circleElem->NextSiblingElement("circle");
	}

	// Condições de saída do programa
	if (this->quantidadeInimigos < 1 || this->quantidadeObjetosResgate < 1) {
		cout << "Não há a quantidade mínima de inimigos e/ou objetos de resgate! Finalizando programa...\n";
	}

}

void AppSettings::showValues() {
	cout << "Imprimindo AppSettings\n";
	this->arena->showValues();
	this->postoAbastecimento->showValues();
	this->jogador->showValues();

	vector<Circle>::iterator it;
	for (it = this->inimigos.begin(); it != this->inimigos.end(); it++) {
		it->showValues();
	}

	for (it = this->objetoResgate.begin(); it != this->objetoResgate.end();
			it++) {
		it->showValues();
	}
}

void AppSettings::checkCircles (Circle* circle) {
	float x = circle->getCx();
	float y = circle->getCy();
	float raio = circle->getR();

	// Checa centor do círculo
	this->detectarLimitesArena (x, y, 0.0, 0.0);

	// Checa parte inferior do circulo
	this->detectarLimitesArena (x, y + raio, 0.0, 0.0);

	// Checa parte superior do circulo
	this->detectarLimitesArena (x, y - raio, 0.0, 0.0);

	// Checa parte mais à direita do círculo
	this->detectarLimitesArena (x + raio, y, 0.0, 0.0);

	// Checa parte mais à esquerda do círculo
	this->detectarLimitesArena (x - raio, y, 0.0, 0.0);
}

void AppSettings::detectarLimitesArena(float x, float y, float width, float height) {
	Rectangle* arena = this->getArena();
	float limiteSuperior = arena->getY();
	float limiteInferior = arena->getY() + arena->getHeight();
	float limiteEsquerdo = arena->getX();
	float limiteDireito = arena->getX() + arena->getWidth();


	if (y < limiteSuperior || y > limiteInferior
		|| (y + height) > limiteInferior) {
		cout << "Objeto fora dos limites da arena. Finalizando o programa...";
		exit(1);
	}

	if (x < limiteEsquerdo || x > limiteDireito
		|| (x + width) > limiteDireito) {
		cout << "Objeto fora dos limites da arena. Finalizando o programa...";
		exit(1);
	}

}

// Getters and Setters
string AppSettings::getPathArena() {
	return this->pathArena;
}

void AppSettings::setPathArena(string pathArena) {
	this->pathArena = pathArena;
}

Rectangle* AppSettings::getArena() {
	return this->arena;
}

void AppSettings::setArena(Rectangle* arena) {
	this->arena = arena;
}

Rectangle* AppSettings::getPostoAbastecimento() {
	return this->postoAbastecimento;
}

void AppSettings::setPostoAbastecimento(Rectangle* postoAbastecimento) {
	this->postoAbastecimento = postoAbastecimento;
}

Circle* AppSettings::getJogador() {
	return this->jogador;
}

void AppSettings::setJogador(Circle* jogador) {
	this->jogador = jogador;
}

vector<Circle> AppSettings::getInimigos() {
	return this->inimigos;
}

void AppSettings::setInimigos(vector<Circle> inimigos) {
	this->inimigos = inimigos;
}

int AppSettings::getQuantidadeInimigos() {
	return this->quantidadeInimigos;
}

void AppSettings::setQuantidadeInimigos(int quantidadeInimigos) {
	this->quantidadeInimigos = quantidadeInimigos;
}

vector<Circle> AppSettings::getObjetoResgate() {
	return this->objetoResgate;
}

void AppSettings::setObjetoResgate(vector<Circle> objetoResgate) {
	this->objetoResgate = objetoResgate;
}

int AppSettings::getQuantidadeObjetosResgate() {
	return this->quantidadeObjetosResgate;
}

void AppSettings::setQuantidadeObjetosResgate(int quantidadeObjetoResgate) {
	this->quantidadeObjetosResgate = quantidadeObjetosResgate;
}
