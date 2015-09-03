#include "main.hpp"

int main(int argc, char *argv[]) {
	if (argc < 6) {
		cout
				<< "Digite os parametros: 1º Linhas Matriz 2º Colunas Matriz 3º Qtd Formigas 4º Qtd Itens 5º Qtd iterações 6º Variação do raio de cada formiga"
				<< endl << "Exemplo: ./ant 40 40 100 250 500000 1" << endl;
		exit(0);
	}
	//int tamMatrizI = 40, tamMatrizJ = 40, qtdFormigas = 100, qtdItens = 250, qtdLoop = 1000000, raio = 1;
	int qtdLoop;
	int tamMatrizI = atoi(argv[1]), tamMatrizJ = atoi(argv[2]), qtdFormigas = atoi(argv[3]), qtdItens = atoi(argv[4]),
		raio = atoi(argv[6]);
	qtdLoop = atoi(argv[5]);
	if (qtdFormigas >= qtdItens) {
	 cout << "Amigo ele vai entrar em loop, porfavor coloque mais itens que formigas" << endl;
	 exit(0);
	 }
	//0 em branco, 1 item, 2 formiga sem item em baixo, 3 formiga e item (solto), 4 formiga e item (segurando) nada em baixo, 5 formiga e item (segurando) item em baixo
	srand(time(NULL));
	vector<vector<int> > matriz;
	vector<formiga> formigas;
	montarMatriz(&matriz, tamMatrizI, tamMatrizJ);
	povoarItensMatriz(&matriz, qtdItens, tamMatrizI, tamMatrizJ);
	povoarFormigas(&matriz, &formigas, qtdFormigas, raio, tamMatrizI, tamMatrizJ);
	interfaceGrafica(&matriz, &formigas, qtdFormigas, tamMatrizI, tamMatrizJ, qtdLoop);
	usleep(80000000);
	return 0;
}

void interfaceGrafica(vector<vector<int> > *matriz, vector<formiga> *formigas, int qtdFormigas, int tamMatrizI,int tamMatrizJ, int qtdLoop) {
	bool flag;
	sf::RenderWindow *window;
	std::vector<std::vector<sf::RectangleShape> > grid;
	window = new sf::RenderWindow(sf::VideoMode(resolucao, resolucao), "Ant-Clustering!");
	window->setPosition(sf::Vector2i(700 / 2, 50));

	for (int i = 0; i < tamMatrizI; ++i) {
		vector<sf::RectangleShape> row;
		for (int j = 0; j < tamMatrizJ; ++j) {
			row.push_back(sf::RectangleShape(sf::Vector2f((resolucao / tamMatrizI), (resolucao / tamMatrizI))));
			row[j].setOutlineThickness(2);
			row[j].setOutlineColor(sf::Color(0, 0, 0));
			row[j].setPosition(i * (resolucao / tamMatrizI), j * (resolucao / tamMatrizI));

			flag = false;
			if (!flag) {
				if (matriz->at(i).at(j) == 0)
					row[j].setFillColor(sf::Color(sf::Color::White)); // espaço livre
				else
					row[j].setFillColor(sf::Color(sf::Color(128, 128, 128))); // item
			}
		}
		grid.push_back(row);
	}

	for (int i = 0; i < qtdLoop; ++i){

		update(matriz, formigas, qtdFormigas, tamMatrizI, tamMatrizJ, window, &grid);
		//if (i == 1)
			//usleep(10000000);
	}
	for (int i = 0; i < qtdFormigas; i++) {
		if (matriz->at(formigas->at(i).i).at(formigas->at(i).j) == 2)
			matriz->at(formigas->at(i).i).at(formigas->at(i).j) = 0;
		if (matriz->at(formigas->at(i).i).at(formigas->at(i).j) == 3)
			matriz->at(formigas->at(i).i).at(formigas->at(i).j) = 1;
	}
	for (int i = 0; i < qtdFormigas; i++) {
		while (formigas->at(i).item == 1) {
			updateFinal(matriz, &formigas->at(i), qtdFormigas, tamMatrizI, tamMatrizJ, window, &grid); //para não deixar nenhuma formiga carregando quando termina
		}
		if (matriz->at(formigas->at(i).i).at(formigas->at(i).j) == 2)
			matriz->at(formigas->at(i).i).at(formigas->at(i).j) = 0;
		if (matriz->at(formigas->at(i).i).at(formigas->at(i).j) == 3)
			matriz->at(formigas->at(i).i).at(formigas->at(i).j) = 1;
	}
}

void updateFinal(vector<vector<int> > *matriz, formiga *formigas, int qtdFormigas, int tamMatrizI,int tamMatrizJ, sf::RenderWindow *window, std::vector<std::vector<sf::RectangleShape> > *grid) {
	movimentoFinal(matriz, formigas, qtdFormigas, tamMatrizI, tamMatrizJ);
	updateColor(matriz, formigas, qtdFormigas, tamMatrizI, tamMatrizJ, grid, 1);
	for (int i = 0; i < tamMatrizI; i++)
		for (int j = 0; j < tamMatrizJ; j++)
			window->draw(grid->at(i).at(j));
	window->display();
}

void update(vector<vector<int> > *matriz, vector<formiga> *formigas, int qtdFormigas, int tamMatrizI, int tamMatrizJ,sf::RenderWindow *window, std::vector<std::vector<sf::RectangleShape> > *grid) {
	for (int i = 0; i < qtdFormigas; i++) {
		movimentoDasFormigas(matriz, &formigas->at(i), qtdFormigas, tamMatrizI, tamMatrizJ);
		updateColor(matriz, &formigas->at(i), qtdFormigas, tamMatrizI, tamMatrizJ, grid, 0);
	}

	for (int i = 0; i < tamMatrizI; i++)
		for (int j = 0; j < tamMatrizJ; j++)
			window->draw(grid->at(i).at(j));
	window->display();
}

void updateColor(vector<vector<int> > *matriz, formiga *formigas, int qtdFormigas, int tamMatrizI,int tamMatrizJ, std::vector<std::vector<sf::RectangleShape> > *grid, int cor) {
	bool flag;
	for (int i = 0; i < tamMatrizI; i++) {
		for (int j = 0; j < tamMatrizJ; j++) {
			flag = false;
			if ((matriz->at(i).at(j) == 2) ) {
				flag = true;
					grid->at(i).at(j).setFillColor(sf::Color(sf::Color::Red)); // formiga sem item
			} else if ((matriz->at(i).at(j) == 4)) {
					flag = true;
					grid->at(i).at(j).setFillColor(sf::Color(sf::Color::Black)); // formiga com item
				} else if ((matriz->at(i).at(j) == 3)) {
					flag = true;
					grid->at(i).at(j).setFillColor(sf::Color(sf::Color::Green));
				} else if ((matriz->at(i).at(j) == 5)) {
					flag = true;
					grid->at(i).at(j).setFillColor(sf::Color(sf::Color::Yellow));
				}
			if (!flag) {
				if (matriz->at(i).at(j) == 0)
					grid->at(i).at(j).setFillColor(sf::Color(sf::Color::White));
				else
					grid->at(i).at(j).setFillColor(sf::Color(sf::Color::Blue)); //item
			}
		}
	}
}

void movimentoFinal(vector<vector<int> > *matriz, formiga *formigas, int qtdFormigas, int tamMatrizI,
		int tamMatrizJ) {
			movimentoDasFormigas(matriz, formigas, qtdFormigas, tamMatrizI, tamMatrizJ);
}

void probabilidadeItens(vector<vector<int> > *matriz, formiga *formiga, int probabilidade) {
	int random = rand() % 100;
	if (formiga->item == 1) {
		if (((random < probabilidade) && (matriz->at(formiga->i).at(formiga->j) == 4))) {
			matriz->at(formiga->i).at(formiga->j) = 3;
			formiga->item = 0;
		}
	} else if (formiga->item == 0) {
		if ((random > probabilidade) && (matriz->at(formiga->i).at(formiga->j) == 3)) {
			matriz->at(formiga->i).at(formiga->j) = 4;
			formiga->item = 1;
		}
	}
}

float visibilidadeItem(vector<vector<int> > *matriz, formiga *formiga, int tamMatrizI, int tamMatrizJ) {
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
		for (int var3 = 0; var3 < linhasMatriz; var3++) {
			if (iAux < 0) {
				iAux = (tamMatrizI - aux1);
				aux1++;
			}
			if (jAux < 0)
				jAux = (tamMatrizJ - (var2));
			if (jAux > tamMatrizJ)
				jAux = 0;
			if (matriz->at(iAux).at(jAux) == 1)
				qtdItensRedor++;
			formiga->tamVisao++;
			jAux++;
		}
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
			if (matriz->at(iAux).at(jAux) == 1)
				qtdItensRedor++;
			formiga->tamVisao++;
			iAux++;
		}
		iAux = formiga->i + var2;
		jAux = formiga->j + (var2 - 1);
		for (int var5 = 0; var5 < linhasMatriz - 1; var5++) {
			if (iAux > tamMatrizI) {
				iAux = 0 + aux4;
				aux4++;
			}
			if (jAux < 0)
				jAux = tamMatrizJ;
			if (jAux > tamMatrizJ)
				jAux = 0;
			if (matriz->at(iAux).at(jAux) == 1)
				qtdItensRedor++;
			formiga->tamVisao++;
			jAux--;
		}
		iAux = formiga->i + (var2 - 1);
		jAux = formiga->j - var2;
		for (int var6 = 0; var6 < linhasMatriz - 2; var6++) {
			if (iAux > tamMatrizI)
				iAux = 0;
			if (iAux < 0)
				iAux = tamMatrizI;
			if (jAux < 0) {
				jAux = tamMatrizJ - aux5;
				aux5++;
			}
			if (matriz->at(iAux).at(jAux) == 1)
				qtdItensRedor++;
			formiga->tamVisao++;
			iAux--;
		}
		jAux = formiga->j - 2;
		iAux = formiga->i - 1;
		iAux--;
	}
	return ((qtdItensRedor / (qtdItensRedor + formiga->tamVisao))
			* (qtdItensRedor / (qtdItensRedor + formiga->tamVisao)));
}

void montarMatriz(vector<vector<int> > *matriz, int tamI, int tamJ) {
	for (int i = 0; i < tamI; ++i) {
		vector<int> row;
		for (int j = 0; j < tamJ; ++j) {
			row.push_back(0);
		}
		matriz->push_back(row);
	}
}

void mostrarMatriz(vector<vector<int> > *matriz, int tamMatrizI, int tamMatrizJ) {
	for (int i = 0; i < tamMatrizI; ++i) {
		for (int j = 0; j < tamMatrizJ; ++j) {
			if (matriz->at(i).at(j) == 1) {
				cout << "\033[1;34m" << matriz->at(i).at(j) << "\033[0m";
			} else if (matriz->at(i).at(j) == 2) {
				cout << "\033[1;31m" << matriz->at(i).at(j) << "\033[0m";
			} else if (matriz->at(i).at(j) == 3) {
				cout << "\033[1;32m" << matriz->at(i).at(j) << "\033[0m";
			} else if (matriz->at(i).at(j) == 4) {
				cout << "\033[1;33m" << matriz->at(i).at(j) << "\033[0m";
			} else if (matriz->at(i).at(j) == 5) {
				cout << "\033[1;35m" << matriz->at(i).at(j) << "\033[0m";
			} else {
				//cout << matriz->at(i).at(j);
				cout << " ";
			}
		}
		cout << endl;
	}
}

void mostrarMatrizLimpa(vector<vector<int> > *matriz, int tamMatrizI, int tamMatrizJ) {
	for (int i = 0; i < tamMatrizI; ++i) {
		for (int j = 0; j < tamMatrizJ; ++j) {
			if (matriz->at(i).at(j) == 1) {
				cout << "\033[1;34m" << matriz->at(i).at(j) << "\033[0m";
			} else if (matriz->at(i).at(j) == 4) {
				cout << "\033[1;34m" << 1 << "\033[0m";
			} else if (matriz->at(i).at(j) == 5) {
				cout << "\033[1;34m" << 1 << "\033[0m";
			} else {
				//cout << matriz->at(i).at(j);
				cout << " ";
			}
		}
		cout << endl;
	}
}

void povoarFormigas(vector<vector<int> > *matriz, vector<formiga> *formigas, int qtdFormigas, int raio, int tamMatrizI,int tamMatrizJ) {
	formiga formigaAux;
	for (int var = 0; var < qtdFormigas; ++var) {
		formigaAux.i = rand() % tamMatrizI;
		formigaAux.j = rand() % tamMatrizJ;
		formigaAux.item = 0;
		//formigaAux.raio = (rand() % raio) + 1;
		formigaAux.raio = raio;
		if (matriz->at(formigaAux.i).at(formigaAux.j) == 0) {
			matriz->at(formigaAux.i).at(formigaAux.j) = 2;
			formigas->push_back(formigaAux);
		} else if (matriz->at(formigaAux.i).at(formigaAux.j) == 1) {
			matriz->at(formigaAux.i).at(formigaAux.j) = 3;
			formigas->push_back(formigaAux);
		} else if ((matriz->at(formigaAux.i).at(formigaAux.j) == 2)
				|| (matriz->at(formigaAux.i).at(formigaAux.j) == 3)) {
			var--;
		}
	}
}

void povoarItensMatriz(vector<vector<int> > *matriz, int qtdItens, int tamMatrizI, int tamMatrizJ) {
	int i, j;
	for (int var = 0; var < qtdItens; ++var) {
		i = rand() % tamMatrizI;
		j = rand() % tamMatrizJ;
		if (matriz->at(i).at(j) == 0) {
			matriz->at(i).at(j) = 1;
		} else if (matriz->at(i).at(j) == 1) {
			var--;
		}
	}
}

void movimentoDasFormigas(vector<vector<int> > *matriz, formiga *formigas, int qtdFormigas, int tamMatrizI,	int tamMatrizJ) {
	int iAux = formigas->i, jAux = formigas->j;
	int direcao, probInteiro;
	float probFloat;
	probFloat = visibilidadeItem(matriz,formigas, tamMatrizI, tamMatrizJ) * 100;
	probInteiro = ceil(probFloat);
	probabilidadeItens(matriz, formigas, probInteiro);
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
		if (((matriz->at(iAux).at(jAux) == 2)) || ((matriz->at(iAux).at(jAux) == 3))
				|| ((matriz->at(iAux).at(jAux) == 4)) || ((matriz->at(iAux).at(jAux) == 5)))
			break;
		if ((matriz->at(iAux).at(jAux) == 1) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e item em baixo
			matriz->at(iAux).at(jAux) = 5;
		} else if ((matriz->at(iAux).at(jAux) == 1) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e item em baixo
			matriz->at(iAux).at(jAux) = 3;
		} else if ((matriz->at(iAux).at(jAux) == 0) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e SEM item em baixo
			matriz->at(iAux).at(jAux) = 4;
		} else if ((matriz->at(iAux).at(jAux) == 0) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e SEM item em baixo
			matriz->at(iAux).at(jAux) = 2;
		}
		if ((matriz->at(formigas->i).at(formigas->j) == 5)
				|| (matriz->at(formigas->i).at(formigas->j) == 3)) {
			matriz->at(formigas->i).at(formigas->j) = 1; //atualiza o lugar para sem formiga com 1 item
		} else if ((matriz->at(formigas->i).at(formigas->j) == 2)
				|| (matriz->at(formigas->i).at(formigas->j) == 4)) {
			matriz->at(formigas->i).at(formigas->j) = 0; //atualiza o lugar para sem formiga e sem item
		}
		formigas->i = iAux;
		formigas->j = jAux;
		break;
	case 1:
		//movimento para cima
		iAux--;
		if (formigas->i == 0)
			iAux = tamMatrizI;
		if (((matriz->at(iAux).at(jAux) == 2)) || ((matriz->at(iAux).at(jAux) == 3))
				|| ((matriz->at(iAux).at(jAux) == 4)) || ((matriz->at(iAux).at(jAux) == 5)))
			break;
		if ((matriz->at(iAux).at(jAux) == 1) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e item em baixo
			matriz->at(iAux).at(jAux) = 5;
		} else if ((matriz->at(iAux).at(jAux) == 1) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e item em baixo
			matriz->at(iAux).at(jAux) = 3;
		} else if ((matriz->at(iAux).at(jAux) == 0) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e SEM item em baixo
			matriz->at(iAux).at(jAux) = 4;
		} else if ((matriz->at(iAux).at(jAux) == 0) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e SEM item em baixo
			matriz->at(iAux).at(jAux) = 2;
		}
		if ((matriz->at(formigas->i).at(formigas->j) == 5)
				|| (matriz->at(formigas->i).at(formigas->j) == 3)) {
			matriz->at(formigas->i).at(formigas->j) = 1; //atualiza o lugar para sem formiga com 1 item
		} else if ((matriz->at(formigas->i).at(formigas->j) == 2)
				|| (matriz->at(formigas->i).at(formigas->j) == 4)) {
			matriz->at(formigas->i).at(formigas->j) = 0; //atualiza o lugar para sem formiga e sem item
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
		if (((matriz->at(iAux).at(jAux) == 2)) || ((matriz->at(iAux).at(jAux) == 3))
				|| ((matriz->at(iAux).at(jAux) == 4)) || ((matriz->at(iAux).at(jAux) == 5)))
			break;
		if ((matriz->at(iAux).at(jAux) == 1) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e item em baixo
			matriz->at(iAux).at(jAux) = 5;
		} else if ((matriz->at(iAux).at(jAux) == 1) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e item em baixo
			matriz->at(iAux).at(jAux) = 3;
		} else if ((matriz->at(iAux).at(jAux) == 0) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e SEM item em baixo
			matriz->at(iAux).at(jAux) = 4;
		} else if ((matriz->at(iAux).at(jAux) == 0) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e SEM item em baixo
			matriz->at(iAux).at(jAux) = 2;
		}
		if ((matriz->at(formigas->i).at(formigas->j) == 5)
				|| (matriz->at(formigas->i).at(formigas->j) == 3)) {
			matriz->at(formigas->i).at(formigas->j) = 1; //atualiza o lugar para sem formiga com 1 item
		} else if ((matriz->at(formigas->i).at(formigas->j) == 2)
				|| (matriz->at(formigas->i).at(formigas->j) == 4)) {
			matriz->at(formigas->i).at(formigas->j) = 0; //atualiza o lugar para sem formiga e sem item
		}
		formigas->i = iAux;
		formigas->j = jAux;
		break;
	case 3:
		//movimento para esquerda
		jAux--;
		if (formigas->j == 0)
			jAux = tamMatrizJ;
		if (((matriz->at(iAux).at(jAux) == 2)) || ((matriz->at(iAux).at(jAux) == 3))
				|| ((matriz->at(iAux).at(jAux) == 4)) || ((matriz->at(iAux).at(jAux) == 5)))
			break;
		if ((matriz->at(iAux).at(jAux) == 1) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e item em baixo
			matriz->at(iAux).at(jAux) = 5;
		} else if ((matriz->at(iAux).at(jAux) == 1) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e item em baixo
			matriz->at(iAux).at(jAux) = 3;
		} else if ((matriz->at(iAux).at(jAux) == 0) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e SEM item em baixo
			matriz->at(iAux).at(jAux) = 4;
		} else if ((matriz->at(iAux).at(jAux) == 0) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e SEM item em baixo
			matriz->at(iAux).at(jAux) = 2;
		}
		if ((matriz->at(formigas->i).at(formigas->j) == 5)
				|| (matriz->at(formigas->i).at(formigas->j) == 3)) {
			matriz->at(formigas->i).at(formigas->j) = 1; //atualiza o lugar para sem formiga com 1 item
		} else if ((matriz->at(formigas->i).at(formigas->j) == 2)
				|| (matriz->at(formigas->i).at(formigas->j) == 4)) {
			matriz->at(formigas->i).at(formigas->j) = 0; //atualiza o lugar para sem formiga e sem item
		}
		formigas->j = jAux;
		break;
	case 4:
		//movimento para direita
		jAux++;
		if (formigas->j == tamMatrizJ)
			jAux = 0;
		if (((matriz->at(iAux).at(jAux) == 2)) || ((matriz->at(iAux).at(jAux) == 3))
				|| ((matriz->at(iAux).at(jAux) == 4)) || ((matriz->at(iAux).at(jAux) == 5)))
			break;
		if ((matriz->at(iAux).at(jAux) == 1) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e item em baixo
			matriz->at(iAux).at(jAux) = 5;
		} else if ((matriz->at(iAux).at(jAux) == 1) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e item em baixo
			matriz->at(iAux).at(jAux) = 3;
		} else if ((matriz->at(iAux).at(jAux) == 0) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e SEM item em baixo
			matriz->at(iAux).at(jAux) = 4;
		} else if ((matriz->at(iAux).at(jAux) == 0) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e SEM item em baixo
			matriz->at(iAux).at(jAux) = 2;
		}
		if ((matriz->at(formigas->i).at(formigas->j) == 5)
				|| (matriz->at(formigas->i).at(formigas->j) == 3)) {
			matriz->at(formigas->i).at(formigas->j) = 1; //atualiza o lugar para sem formiga com 1 item
		} else if ((matriz->at(formigas->i).at(formigas->j) == 2)
				|| (matriz->at(formigas->i).at(formigas->j) == 4)) {
			matriz->at(formigas->i).at(formigas->j) = 0; //atualiza o lugar para sem formiga e sem item
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
		if (((matriz->at(iAux).at(jAux) == 2)) || ((matriz->at(iAux).at(jAux) == 3))
				|| ((matriz->at(iAux).at(jAux) == 4)) || ((matriz->at(iAux).at(jAux) == 5)))
			break;
		if ((matriz->at(iAux).at(jAux) == 1) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e item em baixo
			matriz->at(iAux).at(jAux) = 5;
		} else if ((matriz->at(iAux).at(jAux) == 1) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e item em baixo
			matriz->at(iAux).at(jAux) = 3;
		} else if ((matriz->at(iAux).at(jAux) == 0) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e SEM item em baixo
			matriz->at(iAux).at(jAux) = 4;
		} else if ((matriz->at(iAux).at(jAux) == 0) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e SEM item em baixo
			matriz->at(iAux).at(jAux) = 2;
		}
		if ((matriz->at(formigas->i).at(formigas->j) == 5)
				|| (matriz->at(formigas->i).at(formigas->j) == 3)) {
			matriz->at(formigas->i).at(formigas->j) = 1; //atualiza o lugar para sem formiga com 1 item
		} else if ((matriz->at(formigas->i).at(formigas->j) == 2)
				|| (matriz->at(formigas->i).at(formigas->j) == 4)) {
			matriz->at(formigas->i).at(formigas->j) = 0; //atualiza o lugar para sem formiga e sem item
		}
		formigas->i = iAux;
		formigas->j = jAux;
		break;
	case 6:
		//movimento para baixo
		iAux++;
		if (formigas->i == tamMatrizI)
			iAux = 0;
		if (((matriz->at(iAux).at(jAux) == 2)) || ((matriz->at(iAux).at(jAux) == 3))
				|| ((matriz->at(iAux).at(jAux) == 4)) || ((matriz->at(iAux).at(jAux) == 5)))
			break;
		if ((matriz->at(iAux).at(jAux) == 1) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e item em baixo
			matriz->at(iAux).at(jAux) = 5;
		} else if ((matriz->at(iAux).at(jAux) == 1) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e item em baixo
			matriz->at(iAux).at(jAux) = 3;
		} else if ((matriz->at(iAux).at(jAux) == 0) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e SEM item em baixo
			matriz->at(iAux).at(jAux) = 4;
		} else if ((matriz->at(iAux).at(jAux) == 0) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e SEM item em baixo
			matriz->at(iAux).at(jAux) = 2;
		}
		if ((matriz->at(formigas->i).at(formigas->j) == 5)
				|| (matriz->at(formigas->i).at(formigas->j) == 3)) {
			matriz->at(formigas->i).at(formigas->j) = 1; //atualiza o lugar para sem formiga com 1 item
		} else if ((matriz->at(formigas->i).at(formigas->j) == 2)
				|| (matriz->at(formigas->i).at(formigas->j) == 4)) {
			matriz->at(formigas->i).at(formigas->j) = 0; //atualiza o lugar para sem formiga e sem item
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
		if (((matriz->at(iAux).at(jAux) == 2)) || ((matriz->at(iAux).at(jAux) == 3))
				|| ((matriz->at(iAux).at(jAux) == 4)) || ((matriz->at(iAux).at(jAux) == 5)))
			break;
		if ((matriz->at(iAux).at(jAux) == 1) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e item em baixo
			matriz->at(iAux).at(jAux) = 5;
		} else if ((matriz->at(iAux).at(jAux) == 1) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e item em baixo
			matriz->at(iAux).at(jAux) = 3;
		} else if ((matriz->at(iAux).at(jAux) == 0) && (formigas->item == 1)) {
			//atualizar o movimento da formiga segurando item e SEM item em baixo
			matriz->at(iAux).at(jAux) = 4;
		} else if ((matriz->at(iAux).at(jAux) == 0) && (formigas->item == 0)) {
			//atualizar o movimento da formiga SEM item e SEM item em baixo
			matriz->at(iAux).at(jAux) = 2;
		}
		if ((matriz->at(formigas->i).at(formigas->j) == 5)
				|| (matriz->at(formigas->i).at(formigas->j) == 3)) {
			matriz->at(formigas->i).at(formigas->j) = 1; //atualiza o lugar para sem formiga com 1 item
		} else if ((matriz->at(formigas->i).at(formigas->j) == 2)
				|| (matriz->at(formigas->i).at(formigas->j) == 4)) {
			matriz->at(formigas->i).at(formigas->j) = 0; //atualiza o lugar para sem formiga e sem item
		}
		formigas->i = iAux;
		formigas->j = jAux;
		break;
	default:
		break;
	}
}

