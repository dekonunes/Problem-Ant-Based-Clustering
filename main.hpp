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
#define TEMPO 800000
#define resolucao 640
using namespace std;

struct formiga {
	int item; //0 não contem item, 1 contem item
	int i; //local da formiga em i
	int j; //local da formiga em j
	int raio;
	float tamVisao;
};

void povoarFormigasTeste(vector<formiga> *,int);
void povoarItensMatrizTeste(int , int , int );

void povoarItensMatriz(int, int, int);
void povoarFormigas(vector<formiga> *, int, int, int, int);
void mostrarMatriz(int, int);
void montarMatriz(int, int);
void movimentoDasFormigas(formiga *, int, int, int);
float visibilidadeItem(vector<vector<int> >, formiga, int, int);
void probabilidadeItens(formiga, int);
void mostrarMatrizLimpa(int, int);
void movimentoFinal(formiga *, int, int, int);

#endif /* MAIN_H_ */
