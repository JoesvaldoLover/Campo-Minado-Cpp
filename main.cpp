#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <queue>

#define par(x, y) (x * 100 + y)
#define lin(casa) (casa / 100)
#define col(casa) (casa % 100)

const unsigned short TAM {9};
char campo[TAM][TAM];
// Quantidade mínima e máxima de bombas que podem aparecer no campo
const unsigned int MAX_BOMBAS {12};
const unsigned int MIN_BOMBAS {8};

// Funções executadas pelo jogador
bool varrerCasa(unsigned short, unsigned short);
void marcarCasa(unsigned short, unsigned short);

// Funções executadas pelo sistema
void inicializar();
void mostrarCampoReal();
void mostrarCampo();
bool verificarVitoria();
int contarBombas(unsigned short, unsigned short);

// Caracteres usados no campo
const char HIDDEN = '?';
const char VAZIO = ' ';
const char BOMBA = 'B';
const char MARK_V = 'X';
const char MARK_B = 'M';

int main(){
	
	short x, y;
	bool loop;
	
	srand(time(NULL));
	
	inicializar();
	
	std::cout << " -----------------------------------\n";
	printf("%24s"," CAMPO MINADO\n");
	std::cout << " -----------------------------------\n";
	std::cout << " - Revele todas as casas do campo" << std::endl <<
				 " - Selecione uma casa digitando:" << std::endl <<
				 " > Linha Coluna (Ex: 2 3)" << std::endl <<
				 " - Escolha marcar ou revelar a casa" << std::endl <<
				 " > s: (Des)Marcar - n: Revelar" << std::endl <<
				 " - Revele uma bomba e o jogo acaba" << std::endl <<
				 " > Ha entre " << MIN_BOMBAS << " a " << MAX_BOMBAS << " bombas no campo" << std::endl << std::endl;
	system("pause");
	
	/*
	* Loop repetido até o fim do jogo
	* - Imprimi o campo no terminal
	* - Usuário informa as coordenadas de uma casa
	*/
	do{
		system("cls");
		
		mostrarCampo();
		
		std::cout << "\nEscolha uma casa [Linha Coluna]: ";
		std::cin >> x;
		std::cin >> y;
		x--;
		y--;

		if((x < 0 || x >= TAM) || (y < 0 || y >= TAM))
			continue;
		
		// Aceita apenas "s" ou "n" como input
		do{
			std::cout << "(Des)Marcar uma casa? (s/n): ";
			char op;
			std::cin >> op;
			
			if(op == 's' || op ==  'S'){
				marcarCasa(x, y);
				break;
			}else if(op == 'n' || op == 'N'){
				loop = varrerCasa(x, y);
				break;
			}
			
		}while(true);

	}while(loop);
	
	system("pause");
	return 0;
}

// Função usada para varrer as casas adjacente a uma casa específica
bool varrerCasa(unsigned short x, unsigned short y){
	
	// Não varre a casa caso esteja marcada
	if(campo[x][y] == MARK_B || campo[x][y] == MARK_V)
		return true;
	
	/* 
	* Se a casa escolhida for uma bomba, acaba o jogo
	* - Revela os valores em todas casas do campo
	*/
	if(campo[x][y] == BOMBA){
		system("cls");
		mostrarCampoReal();
		std::cout << "\n-------------------";
		std::cout << "\nBomba! Voce perdeu!\n";
		std::cout << "-------------------\n";
		return false;
	}
	
	/*
	* casasFila será usado para armazenar as próximas casas que serão reveladas
	* A função "par" é usada para armazenar as coordenadas x e y em um índice
	*/
	std::queue<int> casasFila;
	casasFila.push(par(x, y));
	
	// Quando não tiver mais nenhuma casa para varrer, sai do loop
	do{
		int casa = casasFila.front();
		int lin = lin(casa);
		int col = col(casa);
		
		// Define o valor da casa no campo igual a quantidade de bombas ao seu redor
		campo[lin][col] = contarBombas(lin, col);
		
		// Se houver bombas ao redor, não verificar as casas ao redor da casa atual
		if(campo[lin][col] != VAZIO){
			casasFila.pop();
			continue;
		}

		/*
		* Verifica as casas adjacentes a casa atual
		* - Se está tentando acessar uma casa fora do campo, ignora linha/coluna atual
		* - Se a casa não tiver o caractere, pula para a próxima casa
		* - Caso nenhuma das condinções anteriores seja verdade, adiciona a casa para a fila casasFila
		*/	
		for(int i = lin - 1; i <= lin + 1; i++){
			if(i < 0 || i >= TAM)
				continue;
			
			for(int j = col - 1; j <= col + 1; j++){
				if(j < 0 || j >= TAM)
					continue;
				
				if(campo[i][j] != HIDDEN)
					continue;
				
				casasFila.push(par(i, j));
			}
		}
		
		casasFila.pop();
		
	}while(!casasFila.empty());
	


	return verificarVitoria();
}

/*
* Caso não esteja marcada, marca a casa
* Caso contrário, desmarca a casa
*/
void marcarCasa(unsigned short x, unsigned short y){
	
	switch(campo[x][y]){
		case MARK_V:
			campo[x][y] = HIDDEN;
		break;
		case MARK_B:
			campo[x][y] = BOMBA;
		break;
		case HIDDEN:
			campo[x][y] = MARK_V;
		break;
		case BOMBA:
			campo[x][y] = MARK_B;
		break;
	}
}

/*
* Conta todas bombas ao redor da casa
* - Se não houver bombas, retorna o valor da constante VAZIO
* - Se houver bombas, retorna caractere da quantia de bombas em formato decimal
* - Considera bombas marcadas ou não
*/
int contarBombas(unsigned short row, unsigned short col){
	int bombaQuant = 0;
	
	for(int i = row - 1; i <= row + 1; i++){
		if(i < 0 || i >= TAM)
			continue;
		
		for(int j = col - 1; j <= col + 1; j++){
			if(j < 0 || j >= TAM)
				continue;
			
			switch(campo[i][j])
				case BOMBA: case MARK_B: 
				bombaQuant++;
			
		}
	}
	
	return bombaQuant == 0? VAZIO : bombaQuant + 48;
}

// Se todas casas vazias foram varridas, o jogador ganha
bool verificarVitoria(){
	for(int i = 0; i < TAM; i++)
		for(int j = 0; j < TAM; j++)
			if(campo[i][j] == HIDDEN || campo[i][j] == MARK_V)
				return true;
	
	system("cls");
	mostrarCampoReal();
	std::cout << "\n------------";
	std::cout << "\nVoce ganhou!\n";
	std::cout << "------------\n";		
	return false;
}

// Preenche o campo
void inicializar(){
	int x, y;
	/*
	* Caso não haja variação na quantidade de bombas, MAX_BOMBAS e MIN_BOMBAS são iguais
	* Para evitar uma divisão por zero (rand() % 0), MAX_MIN_DIFF é atribuído o valor 1
	*/
	int const MAX_MIN_DIFF = MAX_BOMBAS - MIN_BOMBAS == 0? 1: MAX_BOMBAS - MIN_BOMBAS;
	
	// Preenche todos campos com o caractere em HIDDEN
	for(int i = 0; i < TAM; i++)
		for(int j = 0; j < TAM; j++)
			campo[i][j] = HIDDEN;
	
	/*
	* Defini o valor em casas aleatórias com o caractere em BOMBA
	* - Se a casa tiver um caractere diferente de HIDDEN, ou seja,
	* possui uma bomba, tenta colocar a bomba em outro lugar
	*/
	for(unsigned short i = 0; i < rand() % (MAX_MIN_DIFF) + MIN_BOMBAS; i++){
		x = rand() % TAM;
		y = rand() % TAM;
		
		campo[x][y] != HIDDEN? i-- : campo[x][y] = BOMBA;
	}
	
	return;
}

// Mostra os valores das casas do campo
void mostrarCampoReal(){
	std::cout << "    ";
	
	// Escreve número da coluna, ajustando o espaçamento entre os números se tiver mais de 2 dígitos
	for(int i = 0;; i++){
		std::cout << i + 1;
		
		if(i >= TAM - 1)
			break;
				
		std::cout << (i + 2 < 10 ? "   " : "  ");
	}
	
	std::cout << std::endl;
	
	// Escreve o número das linhas e os valores nas casas
	for(int i = 0; i < TAM; i++){
		std::cout << i + 1 <<
		(i + 1 < 10 ? ": [" : ":[");
		
		for(int j = 0;; j++){
			std::cout << campo[i][j];
			
			if(j >= TAM - 1)
				break;
				
			std::cout << " | ";
		}
		
		std::cout << "]\n";
		
		if( i == TAM - 1)
				break;
		
		std::cout << "   [";
		
		for(int j = 0; j < TAM - 1; j++)
			std::cout << "----";
		
		std::cout << "-]\n";
	}
}
/*
* Mostra os valores das casas do campo,
* mas escreve um resultado diferente dependendo do valor da casa
* - Se casa ter o caractere de BOMBA, escreve o valor de HIDDEN
* - Se casa ter o caractere de MARK_B, escreve o valor de MARK_V
*/
void mostrarCampo(){
	std::cout << "    ";
	
	// Escreve número da coluna, ajustando o espaçamento entre os números se tiver mais de 2 dígitos
	for(int i = 0;; i++){
		std::cout << i + 1;
		
		if(i >= TAM - 1)
			break;
		
		
		std::cout << (i + 2 < 10 ? "   " : "  ");
	}
	
	std::cout << std::endl;
	
	// Escreve o número das linhas e os valores nas casas
	for(int i = 0; i < TAM; i++){
		std::cout << i + 1 <<
		(i + 1 < 10 ? ": [" : ":[");
		
		for(int j = 0;; j++){
			char show;
			
			switch(campo[i][j]){
				case BOMBA: show = HIDDEN;
				break;
				case MARK_V: show = MARK_B;
				break;
				default:
					show = campo[i][j];
				break;
			}
			
			std::cout << show;
			
			if(j >= TAM - 1)
				break;
				
			std::cout << " | ";
		}
		
		std::cout << "]\n";
		
		if( i == (TAM - 1))
				break;
		
		std::cout << "   [";
		
		for(int j = 0; j < TAM - 1; j++)
			std::cout << "----";
		
		std::cout << "-]\n";
	}
}