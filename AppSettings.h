/*
 * AppSettings.h
 *
 *  Created on: 13/09/2015
 *      Author: rodrigo
 */

#ifndef APPSETTINGS_H_
#define APPSETTINGS_H_

#include <string>
#include <vector>

#include "Rectangle.h"
#include "Circle.h"

using namespace std;

class AppSettings {
protected:
	string pathArena;
	Rectangle* arena;
	Rectangle* postoAbastecimento;
	Circle* jogador;
	vector<Circle> inimigos;
	int quantidadeInimigos;

	vector<Circle> objetoResgate;
	int quantidadeObjetosResgate;

private:
	void detectarLimitesArena(float x, float y, float width, float height);
	void checkCircles (Circle* circle);
public:
	AppSettings();
	virtual ~AppSettings();
	void loadConfigXML (char** path);
	void loadSvgFile ();
	void showValues ();

	// Getters and Setters
	string getPathArena();
	void setPathArena(string pathArena);
	Rectangle* getArena();
	void setArena(Rectangle* arena);
	Rectangle* getPostoAbastecimento();
	void setPostoAbastecimento(Rectangle* postoAbastecimento);
	Circle* getJogador();
	void setJogador (Circle* jogador);
	vector<Circle> getInimigos();
	void setInimigos(vector<Circle> inimigos);
	int getQuantidadeInimigos();
	void setQuantidadeInimigos(int quantidadeInimigos);

	vector<Circle> getObjetoResgate ();
	void setObjetoResgate (vector<Circle> objetoResgate);
	int getQuantidadeObjetosResgate();
	void setQuantidadeObjetosResgate (int quantidadeObjetoResgate);
};

#endif /* APPSETTINGS_H_ */
