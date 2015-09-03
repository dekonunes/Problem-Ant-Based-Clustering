/*
 * main.h
 *
 *  Created on: 20 de ago de 2015
 *      Author: udesc
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <iostream>
#include <vector>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <math.h>
#include <SFML/Graphics.hpp>
#define TEMPO 80000
#define resolucao 640
using namespace std;

struct formiga {
	int item; //0 não contem item, 1 contem item
	int i; //local da formiga em i
	int j; //local da formiga em j
	int raio;
	float tamVisao;
};

enum Color {
	NONE = 0, BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE
};

void povoarItensMatriz(vector<vector<int> > *, int, int, int);
void povoarFormigas(vector<vector<int> > *, vector<formiga> *, int, int, int, int);
void mostrarMatriz(vector<vector<int> > *, int, int);
void montarMatriz(vector<vector<int> > *, int, int);
void movimentoDasFormigas(vector<vector<int> > *, formiga *, int, int, int);
float visibilidadeItem(vector<vector<int> >, formiga, int, int);
void probabilidadeItens(vector<vector<int> > *, formiga, int);
void mostrarMatrizLimpa(vector<vector<int> > *, int, int);
void movimentoFinal(vector<vector<int> > *,formiga *, int, int, int);
void interfaceGrafica(vector<vector<int> > *, vector<formiga> *, int, int, int, int);
void update(vector<vector<int> > *, vector<formiga> *, int, int, int, sf::RenderWindow *, std::vector<std::vector<sf::RectangleShape> > *);
void updateFinal(vector<vector<int> > *, formiga *, int, int, int, sf::RenderWindow *, std::vector<std::vector<sf::RectangleShape> > *);
void updateColor(vector<vector<int> > *, formiga *, int, int, int, std::vector<std::vector<sf::RectangleShape> > *, int);

#endif /* MAIN_H_ */
