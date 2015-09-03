#include "main.hpp"

int matriz[40][40];

int main(int argc, char *argv[]) {
	if (argc < 6) {
		cout
				<< "Digite os parametros: 1º Linhas Matriz 2º Colunas Matriz 3º Qtd Formigas 4º Qtd Itens 5º Qtd iterações 6º Variação do raio de cada formiga"
				<< endl << "Exemplo: ./ant 40 40 100 250 1000000 1" << endl;
		exit(0);
	}
	//int tamMatrizI = 40, tamMatrizJ = 40, qtdFormigas = 100, qtdItens = 250, qtdLoop = 1000000, raio = 2;
	int qtdLoop, qtdFormigas, qtdItens, tamMatrizI;
	int tamMatrizJ = atoi(argv[2]), raio = atoi(argv[6]);
	tamMatrizI = atoi(argv[1]);
	qtdLoop = atoi(argv[5]);
	qtdFormigas = atoi(argv[3]);
	qtdItens = atoi(argv[4]);
	vector<formiga> formigas;
	if ((tamMatrizI < (raio * 2 + 1)) || (tamMatrizJ < (raio * 2 + 1))) {
		cout << "o tamanho do raio é maior que o tamanho da matriz" << endl;
		exit(0);
	}

	//pthread_t *threads = NULL;
	//int numThreads = atoi(argv[7]);

	//threads = (pthread_t *) malloc(sizeof(pthread_t) * numThreads);

	if (qtdFormigas >= qtdItens) {
		cout
				<< "Amigo ele vai entrar em loop, porfavor coloque mais itens que formigas"
				<< endl;
		exit(0);
	}
	//0 em branco, 1 item, 2 formiga sem item em baixo, 3 formiga e item (solto), 4 formiga e item (segurando) nada em baixo, 5 formiga e item (segurando) item em baixo
	srand(time(NULL));
	montarMatriz(tamMatrizI, tamMatrizJ);
	povoarItensMatriz(qtdItens, tamMatrizI, tamMatrizJ);
	povoarFormigas(&formigas, qtdFormigas, raio, tamMatrizI, tamMatrizJ);
	//povoarItensMatrizTeste(qtdItens, tamMatrizI, tamMatrizJ);
	//povoarFormigasTeste(&formigas, raio);
	mostrarMatriz(tamMatrizI, tamMatrizJ);
	movimentoDasFormigas(&formigas.at(0), qtdFormigas, tamMatrizI, tamMatrizJ);
	for (int var = 0; var < qtdLoop; ++var) {
		for (int var2 = 0; var2 < qtdFormigas; ++var2) {
			movimentoDasFormigas(&formigas.at(var2), qtdFormigas, tamMatrizI,
					tamMatrizJ);
			//usleep(TEMPO);
			//if (system("CLS")) system("clear");
			//cout << "-------------------------------" << endl;
			//mostrarMatriz(tamMatrizI, tamMatrizJ);
		}
	}

	for (int i = 0; i < qtdFormigas; i++) {
		while (formigas.at(i).item == 1) {
			movimentoFinal(&formigas.at(i), qtdFormigas, tamMatrizI,
					tamMatrizJ);
		}
	}
	//mostrarMatriz(&matriz, tamMatrizI, tamMatrizJ);
	cout << "-------------------------------" << endl;
	mostrarMatrizLimpa(tamMatrizI, tamMatrizJ);
	//usleep(80000000);
	return 0;
}

void movimentoFinal(formiga *formigas, int qtdFormigas, int tamMatrizI,
		int tamMatrizJ) {
	movimentoDasFormigas(formigas, qtdFormigas, tamMatrizI, tamMatrizJ);
}

void probabilidadeItens(formiga *formiga, int probabilidade) {
	int random = rand() % 100;
	if (formiga->item == 1) {
		if (((random < probabilidade) && (matriz[formiga->i][formiga->j] == 4))) {
			matriz[formiga->i][formiga->j] = 3;
			formiga->item = 0;
		}
	} else if (formiga->item == 0) {
		if ((random > probabilidade) && (matriz[formiga->i][formiga->j] == 3)) {
			matriz[formiga->i][formiga->j] = 4;
			formiga->item = 1;
		}
	}
}

float visibilidadeItem(formiga *formiga, int tamMatrizI, int tamMatrizJ) {
	int iAux = 0, jAux = 0, linhasMatriz = 3, aux1 = 0, aux2 = 0, aux3 = 0,
			aux4 = 0, aux5 = 0;
	float qtdItensRedor = 0;
	formiga->tamVisao = 0; //quantidade de celular analizadas
	iAux = formiga->i - 1;
	jAux = formiga->j - 1;
	tamMatrizI--;
	tamMatrizJ--;
	for (int var2 = 1; var2 < formiga->raio + 1; var2++) {
		linhasMatriz = (var2 * 2 + 1);
		//iAux = formiga->i - 1;
		//jAux = formiga->j - 1;
		for (int var3 = 0; var3 < linhasMatriz; var3++) {
			if (iAux < 0) {
				iAux = (tamMatrizI - aux1);
				aux1++;
			}
			if (jAux < 0)
				jAux = (tamMatrizJ - (var2));
			if (jAux > tamMatrizJ)
				jAux = 0;
			if (matriz[iAux][jAux] == 1)
				qtdItensRedor++;
			formiga->tamVisao++;
			//cout << iAux << "====" << jAux << endl;
			jAux++;
		}
		//jAux--; //só para arrumar, se não ele passa
		iAux = formiga->i - var2;
		jAux = formiga->j + var2;
		iAux++;
		for (int var4 = 0; var4 < linhasMatriz - 1; var4++) {
			if (iAux < 0) {
				iAux = (tamMatrizI - (aux2));
				aux2++;
			}
			if (iAux > tamMatrizI)
				iAux = 0;
			if (jAux > tamMatrizJ) {
				jAux = 0 + aux3;
				aux3++;
			}
			if (matriz[iAux][jAux] == 1)
				qtdItensRedor++;
			formiga->tamVisao++;
			//cout << iAux << "====" << jAux << endl;
			iAux++;
		}
		iAux = formiga->i + var2;
		jAux = formiga->j + (var2 - 1);
		//cout << iAux << "====" << jAux << endl;
		for (int var5 = 0; var5 < linhasMatriz - 1; var5++) {
			if (iAux > tamMatrizI) {
				iAux = 0 + aux4;
				aux4++;
			}
			if (jAux < 0)
				jAux = tamMatrizJ;
			if (jAux > tamMatrizJ)
				jAux = 0;
			if (matriz[iAux][jAux] == 1)
				qtdItensRedor++;
			formiga->tamVisao++;
			//cout << iAux << "====" << jAux << endl;
			jAux--;
		}
		iAux = formiga->i + (var2 - 1);
		jAux = formiga->j - var2;
		//cout << iAux << "====" << jAux << endl;
		for (int var6 = 0; var6 < linhasMatriz - 2; var6++) {
			if (iAux > tamMatrizI)
				iAux = 0;
			if (iAux < 0)
				iAux = tamMatrizI;
			if (jAux < 0) {
				jAux = tamMatrizJ - aux5;
				aux5++;
			}
			if (matriz[iAux][jAux] == 1)
				qtdItensRedor++;
			formiga->tamVisao++;
			//cout << iAux << "====" << jAux << endl;
			iAux--;
		}
		jAux = formiga->j - 2;
		iAux = formiga->i - 1;
		iAux--;
	}
	return ((qtdItensRedor / (qtdItensRedor + formiga->tamVisao))
			* (qtdItensRedor / (qtdItensRedor + formiga->tamVisao)));
}

void povoarFormigasTeste(vector<formiga> *formigas, int raio) {
	formiga formigaAux2;
	int i = 0, j = 0;
	/*
	 formigaAux1.i = 0;
	 formigaAux1.j = 0;
	 formigaAux1.item = 0;
	 if (matriz->at(i).at(j) == 0) {
	 matriz->at(i).at(j) = 2;
	 formigas->push_back(formigaAux1);
	 } else if (matriz->at(i).at(j) == 1) {
	 matriz->at(i).at(j) = 3;
	 formigas->push_back(formigaAux1);
	 } */
	i = 3;
	j = 0;
	formigaAux2.i = i;
	formigaAux2.j = j;
	formigaAux2.item = 0;
	formigaAux2.raio = raio;
	if (matriz[i][j] == 0) {
		matriz[i][j] = 2;
		formigas->push_back(formigaAux2);
	} else if (matriz[i][j] == 1) {
		matriz[i][j] = 3;
		formigas->push_back(formigaAux2);
	}

}

void povoarItensMatrizTeste(int qtdItens, int tamMatrizI, int tamMatrizJ) {
	int i, j;
	for (int var = 0; var < qtdItens - 1; ++var) {
		i = 1;
		j = 4;
		if (matriz[i][j] == 0) {
			matriz[i][j] = 1;
		} else if (matriz[i][j] == 1) {
			var--;
		}
	}
}

void montarMatriz(int tamI, int tamJ) {
	matriz[40][40] = {0};
}

void mostrarMatriz(int tamMatrizI, int tamMatrizJ) {
	for (int i = 0; i < tamMatrizI; ++i) {
		for (int j = 0; j < tamMatrizJ; ++j) {
			if (matriz[i][j] == 1) {
				cout << "\033[1;34m" << matriz[i][j] << "\033[0m";
			} else if (matriz[i][j] == 2) {
				cout << "\033[1;31m" << matriz[i][j] << "\033[0m";
			} else if (matriz[i][j] == 3) {
				cout << "\033[1;32m" << matriz[i][j] << "\033[0m";
			} else if (matriz[i][j] == 4) {
				cout << "\033[1;33m" << matriz[i][j] << "\033[0m";
			} else if (matriz[i][j] == 5) {
				cout << "\033[1;35m" << matriz[i][j] << "\033[0m";
			} else {
				cout << matriz[i][j];
				//cout << " ";
			}
		}
		cout << endl;
	}
}

void mostrarMatrizLimpa(int tamMatrizI, int tamMatrizJ) {
	for (int i = 0; i < tamMatrizI; ++i) {
		for (int j = 0; j < tamMatrizJ; ++j) {
			if (matriz[i][j] == 1) {
				cout << "\033[1;34m" << matriz[i][j] << "\033[0m";
			} else if (matriz[i][j] == 4) {
				cout << "\033[1;34m" << 1 << "\033[0m";
			} else if (matriz[i][j] == 5) {
				cout << "\033[1;34m" << 1 << "\033[0m";
			} else {
				//cout << matriz->at(i).at(j);
				cout << " ";
			}
		}
		cout << endl;
	}
}

void povoarFormigas(vector<formiga> *formigas, int qtdFormigas, int raio,
		int tamMatrizI, int tamMatrizJ) {
	formiga formigaAux;
	for (int var = 0; var < qtdFormigas; ++var) {
		formigaAux.i = rand() % tamMatrizI;
		formigaAux.j = rand() % tamMatrizJ;
		formigaAux.item = 0;
		//formigaAux.raio = (rand() % raio) + 1;
		formigaAux.raio = raio;
		if (matriz[formigaAux.i][formigaAux.j] == 0) {
			matriz[formigaAux.i][formigaAux.j] = 2;
			formigas->push_back(formigaAux);
		} else if (matriz[formigaAux.i][formigaAux.j] == 1) {
			matriz[formigaAux.i][formigaAux.j] = 3;
			formigas->push_back(formigaAux);
		} else if ((matriz[formigaAux.i][formigaAux.j] == 2)
				|| (matriz[formigaAux.i][formigaAux.j] == 3)) {
			var--;
		}
	}
}

void povoarItensMatriz(int qtdItens, int tamMatrizI, int tamMatrizJ) {
	int i, j;
	for (int var = 0; var < qtdItens; ++var) {
		i = rand() % tamMatrizI;
		j = rand() % tamMatrizJ;
		if (matriz[i][j] == 0) {
			matriz[i][j] = 1;
		} else if (matriz[i][j] == 1) {
			var--;
		}
	}
}

void movimentoDasFormigas(formiga *formigas, int qtdFormigas, int tamMatrizI,
		int tamMatrizJ) {
	int iAux = formigas->i, jAux = formigas->j;
	int direcao, probInteiro;
	float probFloat;
	probFloat = visibilidadeItem(formigas, tamMatrizI, tamMatrizJ) * 100;
	probInteiro = ceil(probFloat);
	probabilidadeItens(formigas, probInteiro);
	tamMatrizI--;
	tamMatrizJ--;
	direcao = rand() % 8;
	switch (direcao) {
	case 0:
		//movimento para superior esquerdo
		iAux--;
		jAux--;
		if (formigas->i == 0)
			iAux = tamMatrizI;
		if (formigas->j == 0)
			jAux = tamMatrizJ;
		if (((matriz[iAux][jAux] == 2)) || ((matriz[iAux][jAux] == 3))
				|| ((matriz[iAux][jAux] == 4)) || ((matriz[iAux][jAux] == 5)))
			break;
		if ((matriz[iAux][jAux] == 1) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e item em baixo
			matriz[iAux][jAux] = 5;
		} else if ((matriz[iAux][jAux] == 1) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e item em baixo
			matriz[iAux][jAux] = 3;
		} else if ((matriz[iAux][jAux] == 0) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e SEM item em baixo
			matriz[iAux][jAux] = 4;
		} else if ((matriz[iAux][jAux] == 0) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e SEM item em baixo
			matriz[iAux][jAux] = 2;
		}
		if ((matriz[formigas->i][formigas->j] == 5)
				|| (matriz[formigas->i][formigas->j] == 3)) {
			matriz[formigas->i][formigas->j] = 1; //atualiza o lugar para sem formiga com 1 item
		} else if ((matriz[formigas->i][formigas->j] == 2)
				|| (matriz[formigas->i][formigas->j] == 4)) {
			matriz[formigas->i][formigas->j] = 0; //atualiza o lugar para sem formiga e sem item
		}
		formigas->i = iAux;
		formigas->j = jAux;
		break;
	case 1:
		//movimento para cima
		iAux--;
		if (formigas->i == 0)
			iAux = tamMatrizI;
		if (((matriz[iAux][jAux] == 2)) || ((matriz[iAux][jAux] == 3))
				|| ((matriz[iAux][jAux] == 4)) || ((matriz[iAux][jAux] == 5)))
			break;
		if ((matriz[iAux][jAux] == 1) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e item em baixo
			matriz[iAux][jAux] = 5;
		} else if ((matriz[iAux][jAux] == 1) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e item em baixo
			matriz[iAux][jAux] = 3;
		} else if ((matriz[iAux][jAux] == 0) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e SEM item em baixo
			matriz[iAux][jAux] = 4;
		} else if ((matriz[iAux][jAux] == 0) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e SEM item em baixo
			matriz[iAux][jAux] = 2;
		}
		if ((matriz[formigas->i][formigas->j] == 5)
				|| (matriz[formigas->i][formigas->j] == 3)) {
			matriz[formigas->i][formigas->j] = 1; //atualiza o lugar para sem formiga com 1 item
		} else if ((matriz[formigas->i][formigas->j] == 2)
				|| (matriz[formigas->i][formigas->j] == 4)) {
			matriz[formigas->i][formigas->j] = 0; //atualiza o lugar para sem formiga e sem item
		}
		formigas->i = iAux;
		break;
	case 2:
		//movimento para superior direito
		iAux--;
		jAux++;
		if (formigas->i == 0)
			iAux = tamMatrizI;
		if (formigas->j == tamMatrizJ)
			jAux = 0;
		if (((matriz[iAux][jAux] == 2)) || ((matriz[iAux][jAux] == 3))
				|| ((matriz[iAux][jAux] == 4)) || ((matriz[iAux][jAux] == 5)))
			break;
		if ((matriz[iAux][jAux] == 1) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e item em baixo
			matriz[iAux][jAux] = 5;
		} else if ((matriz[iAux][jAux] == 1) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e item em baixo
			matriz[iAux][jAux] = 3;
		} else if ((matriz[iAux][jAux] == 0) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e SEM item em baixo
			matriz[iAux][jAux] = 4;
		} else if ((matriz[iAux][jAux] == 0) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e SEM item em baixo
			matriz[iAux][jAux] = 2;
		}
		if ((matriz[formigas->i][formigas->j] == 5)
				|| (matriz[formigas->i][formigas->j] == 3)) {
			matriz[formigas->i][formigas->j] = 1; //atualiza o lugar para sem formiga com 1 item
		} else if ((matriz[formigas->i][formigas->j] == 2)
				|| (matriz[formigas->i][formigas->j] == 4)) {
			matriz[formigas->i][formigas->j] = 0; //atualiza o lugar para sem formiga e sem item
		}
		formigas->i = iAux;
		formigas->j = jAux;
		break;
	case 3:
		//movimento para esquerda
		jAux--;
		if (formigas->j == 0)
			jAux = tamMatrizJ;
		if (((matriz[iAux][jAux] == 2)) || ((matriz[iAux][jAux] == 3))
				|| ((matriz[iAux][jAux] == 4)) || ((matriz[iAux][jAux] == 5)))
			break;
		if ((matriz[iAux][jAux] == 1) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e item em baixo
			matriz[iAux][jAux] = 5;
		} else if ((matriz[iAux][jAux] == 1) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e item em baixo
			matriz[iAux][jAux] = 3;
		} else if ((matriz[iAux][jAux] == 0) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e SEM item em baixo
			matriz[iAux][jAux] = 4;
		} else if ((matriz[iAux][jAux] == 0) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e SEM item em baixo
			matriz[iAux][jAux] = 2;
		}
		if ((matriz[formigas->i][formigas->j] == 5)
				|| (matriz[formigas->i][formigas->j] == 3)) {
			matriz[formigas->i][formigas->j] = 1; //atualiza o lugar para sem formiga com 1 item
		} else if ((matriz[formigas->i][formigas->j] == 2)
				|| (matriz[formigas->i][formigas->j] == 4)) {
			matriz[formigas->i][formigas->j] = 0; //atualiza o lugar para sem formiga e sem item
		}
		formigas->j = jAux;
		break;
	case 4:
		//movimento para direita
		jAux++;
		if (formigas->j == tamMatrizJ)
			jAux = 0;
		if (((matriz[iAux][jAux] == 2)) || ((matriz[iAux][jAux] == 3))
				|| ((matriz[iAux][jAux] == 4)) || ((matriz[iAux][jAux] == 5)))
			break;
		if ((matriz[iAux][jAux] == 1) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e item em baixo
			matriz[iAux][jAux] = 5;
		} else if ((matriz[iAux][jAux] == 1) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e item em baixo
			matriz[iAux][jAux] = 3;
		} else if ((matriz[iAux][jAux] == 0) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e SEM item em baixo
			matriz[iAux][jAux] = 4;
		} else if ((matriz[iAux][jAux] == 0) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e SEM item em baixo
			matriz[iAux][jAux] = 2;
		}
		if ((matriz[formigas->i][formigas->j] == 5)
				|| (matriz[formigas->i][formigas->j] == 3)) {
			matriz[formigas->i][formigas->j] = 1; //atualiza o lugar para sem formiga com 1 item
		} else if ((matriz[formigas->i][formigas->j] == 2)
				|| (matriz[formigas->i][formigas->j] == 4)) {
			matriz[formigas->i][formigas->j] = 0; //atualiza o lugar para sem formiga e sem item
		}
		formigas->j = jAux;
		break;
	case 5:
		//movimento para inferior esquerdo
		iAux++;
		jAux--;
		if (formigas->j == 0)
			jAux = tamMatrizJ;
		if (formigas->i == tamMatrizI)
			iAux = 0;
		if (((matriz[iAux][jAux] == 2)) || ((matriz[iAux][jAux] == 3))
				|| ((matriz[iAux][jAux] == 4)) || ((matriz[iAux][jAux] == 5)))
			break;
		if ((matriz[iAux][jAux] == 1) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e item em baixo
			matriz[iAux][jAux] = 5;
		} else if ((matriz[iAux][jAux] == 1) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e item em baixo
			matriz[iAux][jAux] = 3;
		} else if ((matriz[iAux][jAux] == 0) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e SEM item em baixo
			matriz[iAux][jAux] = 4;
		} else if ((matriz[iAux][jAux] == 0) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e SEM item em baixo
			matriz[iAux][jAux] = 2;
		}
		if ((matriz[formigas->i][formigas->j] == 5)
				|| (matriz[formigas->i][formigas->j] == 3)) {
			matriz[formigas->i][formigas->j] = 1; //atualiza o lugar para sem formiga com 1 item
		} else if ((matriz[formigas->i][formigas->j] == 2)
				|| (matriz[formigas->i][formigas->j] == 4)) {
			matriz[formigas->i][formigas->j] = 0; //atualiza o lugar para sem formiga e sem item
		}
		formigas->i = iAux;
		formigas->j = jAux;
		break;
	case 6:
		//movimento para baixo
		iAux++;
		if (formigas->i == tamMatrizI)
			iAux = 0;
		if (((matriz[iAux][jAux] == 2)) || ((matriz[iAux][jAux] == 3))
				|| ((matriz[iAux][jAux] == 4)) || ((matriz[iAux][jAux] == 5)))
			break;
		if ((matriz[iAux][jAux] == 1) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e item em baixo
			matriz[iAux][jAux] = 5;
		} else if ((matriz[iAux][jAux] == 1) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e item em baixo
			matriz[iAux][jAux] = 3;
		} else if ((matriz[iAux][jAux] == 0) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e SEM item em baixo
			matriz[iAux][jAux] = 4;
		} else if ((matriz[iAux][jAux] == 0) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e SEM item em baixo
			matriz[iAux][jAux] = 2;
		}
		if ((matriz[formigas->i][formigas->j] == 5)
				|| (matriz[formigas->i][formigas->j] == 3)) {
			matriz[formigas->i][formigas->j] = 1; //atualiza o lugar para sem formiga com 1 item
		} else if ((matriz[formigas->i][formigas->j] == 2)
				|| (matriz[formigas->i][formigas->j] == 4)) {
			matriz[formigas->i][formigas->j] = 0; //atualiza o lugar para sem formiga e sem item
		}
		formigas->i = iAux;
		break;
	case 7:
		//movimento para inferior direito
		iAux++;
		jAux++;
		if (formigas->i == tamMatrizI)
			iAux = 0;
		if (formigas->j == tamMatrizJ)
			jAux = 0;
		if (((matriz[iAux][jAux] == 2)) || ((matriz[iAux][jAux] == 3))
				|| ((matriz[iAux][jAux] == 4)) || ((matriz[iAux][jAux] == 5)))
			break;
		if ((matriz[iAux][jAux] == 1) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e item em baixo
			matriz[iAux][jAux] = 5;
		} else if ((matriz[iAux][jAux] == 1) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e item em baixo
			matriz[iAux][jAux] = 3;
		} else if ((matriz[iAux][jAux] == 0) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e SEM item em baixo
			matriz[iAux][jAux] = 4;
		} else if ((matriz[iAux][jAux] == 0) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e SEM item em baixo
			matriz[iAux][jAux] = 2;
		}
		if ((matriz[formigas->i][formigas->j] == 5)
				|| (matriz[formigas->i][formigas->j] == 3)) {
			matriz[formigas->i][formigas->j] = 1; //atualiza o lugar para sem formiga com 1 item
		} else if ((matriz[formigas->i][formigas->j] == 2)
				|| (matriz[formigas->i][formigas->j] == 4)) {
			matriz[formigas->i][formigas->j] = 0; //atualiza o lugar para sem formiga e sem item
		}
		formigas->i = iAux;
		formigas->j = jAux;
		break;
	default:
		break;
	}

}

