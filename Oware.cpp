#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <windows.h>
#include <ctime>
using namespace std;

#define RED 4
#define WHITE 15
#define LIGHTBLUE 9
#define GREEN 2
#define BLACK 0
#define DARKGRAY 8

//Declaração de funções
bool testCapturing(vector<int> board, unsigned int house, int player);

/*Limpa o ecrâ da consola*/
void clrscr(void) {
	COORD coordScreen = { 0, 0 }; // upper left corner
	DWORD cCharsWritten;
	DWORD dwConSize;
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hCon, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	// fill with spaces
	FillConsoleOutputCharacter(hCon, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
	GetConsoleScreenBufferInfo(hCon, &csbi);
	FillConsoleOutputAttribute(hCon, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	// cursor to upper left corner
	SetConsoleCursorPosition(hCon, coordScreen);
}

/*Altera a cor do texto*/
void setcolor(unsigned int color) {
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, color);
}
void setcolor(unsigned int color, unsigned int background_color)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	if (background_color == BLACK)
		SetConsoleTextAttribute(hCon, color);
	else
		SetConsoleTextAttribute(hCon, color | BACKGROUND_BLUE | BACKGROUND_GREEN |
			BACKGROUND_RED);
}
void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/*Pergunta se o utilizador quer jogar o modo 1 vs 1 ou 1 vs RP*/
int gameModeInput() {
	int p;
	bool validInput = false;
	while (!validInput) {
		clrscr();
		setcolor(0, 4);
		gotoxy(50, 2); cout << " Oware Game ";
		setcolor(15);
		gotoxy(25, 4); cout << "Select the mode ( 1 - One Player, 2 - Two Players): ";
		validInput = true; // Começa por assumir que o input será válido a cada ciclo
		cin >> p;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(1000, '\n');
			validInput = false;
		}
		else {
			if (cin.peek() != '\n') {
				validInput = false;
			}
			cin.ignore(1000, '\n');
		}
		if (validInput && p != 1 && p != 2) {
			validInput = false;
		}
		if (!validInput) { // Mensagem de erro 
			gotoxy(25, 6);
			cerr << "Please enter the number 1 or 2";
			Sleep(2500);
		}
	}
	return p;
}

/*Pergunta o nome do/dos jogadores*/
void setNames(string& name1, string& name2, int p) {
	clrscr();
	bool validNames;
	do {
		validNames = true; // Começa por assumir que os nomes a introduzir são válidos
		setcolor(0, 4);
		gotoxy(50, 2); cout << " Oware Game ";
		setcolor(15);
		gotoxy(40, 4); cout << "Name of 1st Player: "; getline(cin, name1);
		if (p == 2) { // Apenas permite escolher o 2º nome se for o modo de dois jogadores
			gotoxy(40, 5); cout << "Name of 2nd Player: "; getline(cin, name2);
		}
		else name2 = "Computer";
		// Limitações dos nomes escolhidos
		if (name1.length() >= 15 || name2.length() >= 15 || name1.length() < 1 || name2.length() < 1) {
			gotoxy(40, 6); cout << "Names should have less than 15 characteres and at least 1";
			validNames = false;
			Sleep(2500);
			clrscr();
		}
	} while (!validNames);
}

/*Mostra o tabuleiro de jogo*/
void displayBoard(vector<int> board, unsigned int seedsPlayer1, unsigned int seedsPlayer2, int turn, string name1, string name2) {
	clrscr();
	setcolor(0, 4);
	gotoxy(50, 2); cout << " Oware Game ";
	setcolor(15); 
	gotoxy(40, 4); cout << "Turn:" << setw(4) << turn + 1 << endl << endl;
	gotoxy(40, 5); cout << "+-----------------------------+";
	gotoxy(40, 7); cout << "+-----------------------------+";
	gotoxy(40, 9); cout << "+-----------------------------+";
	for (int i = 0; i < 7;i++) {
		gotoxy(40 + 5 * i, 6); cout << "|";
	}
	for (int i = 0; i < 7;i++) {
		gotoxy(40 + 5 * i, 8); cout << "|";
	}

	setcolor(2);
	for (int i = 11;i >= 6;i--) { // Casas do jogador 2
		gotoxy((-i+11)*5+42, 6); cout << setw(2) << board[i];
	}	
	setcolor(9);
	for (int i = 0;i < 6;i++) { // Casas do jogador 1
		gotoxy(i * 5 + 42, 8); cout << setw(2) << board[i];
	}
	setcolor(15);
	setcolor(9); gotoxy(10, 8); cout << name1 << "'s Seeds:" << setw(4) << seedsPlayer1 << endl;
	setcolor(2); gotoxy(75, 6); cout << name2 << "'s Seeds:" << setw(4) << seedsPlayer2 << endl;
	setcolor(15);
}

/*Quando um jogador pretende terminar o jogo abruptamente, as sementes no tabuleiro são distribuídas*/
void bruteEnd(vector<int>& board, unsigned int &seedsPlayer1, unsigned int &seedsPlayer2) {
	for (int i = 0;i < 6;i++) { // Sementes em casas do jogador 1 são colocadas 
		seedsPlayer1 += board[i]; // na pontução do jogador 1
		board[i] = 0;
	}
	for (int i = 6;i < 12;i++) { // Sementes em casas do jogador 1 são colocadas
		seedsPlayer2 += board[i];  // na pontução do jogador 2
		board[i] = 0;
	}
}

/*Efetua e verifica a validada do "input" do Random Player, semelhante à segunada metade da função input()*/
void rpinput(bool& validInput, vector<int> board, int player, unsigned int& house, int& k) {
	if (k == 0) srand(time(NULL));
	house = rand() % 6 + 1;
	
	house--;
	if (player % 2 == 1) house += 6;  // Processa o input

	int sum = 0; // Soma as sementes em casas adversárias
	for (int i = 6 * ((player + 1) % 2);i < 6 + 6 * ((player + 1) % 2);i++) {
		sum += board[i];
	}

	if (sum != 0) { // Se o adversário ainda tiver sementes nas suas casas ->
		if (board[house] == 0) validInput = false; // Verifica se a casa escolhida está vazia 
		else if (testCapturing(board, house, player)) validInput = false; // Verifica se pode fazer a captura
	}
	else {
		if (board[house] + (house % 6) < 6) validInput = false; // Verifica se a sua jogada coloca sementes do lado adversário, caso não tenha
	}
}

/*Efetua e verifica a validade do input*/
void input(bool &forcedEnd, bool &validInput, vector<int> board, int player, unsigned int &house, unsigned int &seedsPlayer1, unsigned int &seedsPlayer2, string name1, string name2, int &k, int p) {
	validInput = true;
	if (player % 2 == 0 || p == 2) {  // Se não for a jogada do RP ->
		gotoxy(40, 11 + k); // Indica de quem é a jogada
		if (player % 2 == 0) cout << name1 << "'s turn: ";
		else cout << name2 << "'s turn: ";
		cin >> house;
		gotoxy(40, 12 + k);
		if (cin.fail()) {
			if (cin.eof()) {																// Verifica se algum jogador interrompeu o jogo
				bruteEnd(board, seedsPlayer1, seedsPlayer2);
				forcedEnd = true;
			}
			else {
				cerr << "Please enter a number between 1 and 6";
				cin.clear();
				cin.ignore(1000, '\n');
				validInput = false;
			}
		}
		else {
			if (cin.peek() != '\n') {
				cerr << "Please enter a number between 1 and 6";
				validInput = false;
			}
			cin.ignore(1000, '\n');
		}

		if (validInput && !forcedEnd) { // Se o input até à data não for inválido nem tiver sido interrompido ->
			if (house < 1 || house > 6) {												// Verificar se a casa escolhida está
				cerr << "Please enter a number between 1 and 6";  // dentro dos limites
				validInput = false;
			}
			else {
				house--;
				if (player % 2 == 1) house += 6;  // Processa o input

				int sum = 0; // Soma as sementes em casa adversárias
				for (int i = 6 * ((player + 1) % 2);i < 6 + 6 * ((player + 1) % 2);i++) {
					sum += board[i];
				}

				if (sum != 0) {
					if (board[house] == 0) { // Verifica se a casa escolhida está vazia 
						cerr << "This house is empty, choose another one";
						validInput = false;
					}
					else if (testCapturing(board, house, player)) { // Verifica se pode fazer a captura
						cerr << "Don't capture all other player's seeds";
						validInput = false;
					}
				}
				else {
					if (board[house] + (house % 6) < 6) { // Verifica se a sua jogada coloca sementes do lado adversário, caso não tenha
						cerr << "You can give some seeds to the other player, choose another one";
						validInput = false;
					}
				}
			}
		}
	}
	else {  // Se for a jogada do RP
		rpinput(validInput, board, player, house, k);
	}
	
	k += 2; // Atualiza coordenadas
}

/*Distribui as sementes da casa escolhida*/
void sowing(vector<int> &board, unsigned int house, int &i) {
	i = 0;
	while (board[house] != 0) {
		if (house != (house + i) % 12) {
			board[(house + i) % 12] += 1;
			board[house]--;
		}
		i++;
	}
} 

/*Captura as sementes adversárias caso seja possível*/
void capturing(vector<int> &board, unsigned int house, int player, int i, unsigned int &seedsPlayer1, unsigned int &seedsPlayer2) {
	unsigned int auxI = (house + i - 1) % 12;
	if (player % 2 == 0) { // Captura do jogador 1 
		while (auxI >= 6 && (board[auxI] == 2 || board[auxI] == 3)) {
			seedsPlayer1 += board[auxI];
			board[auxI] = 0;
			auxI--;
		}
	}
	else { // Captura do jogador 2
		while (auxI < 6 && auxI >= 0 && (board[auxI] == 2 || board[auxI] == 3)) {
			seedsPlayer2 += board[auxI];
			board[auxI] = 0;
			auxI--;
		}
	}
}

/*Testa se ao capturar as sementes o adversário fica sem sementes*/
bool testCapturing(vector<int> board, unsigned int house, int player) {
	int i = 0;
	// Distribuição de sementes da casa "house"
	while (board[house] != 0) {
		if (house != (house + i) % 12) {
			board[(house + i) % 12] += 1;
			board[house]--;
		}
		i++;
	}
	// Pseudo-captura (não atualiza as variáveis seedsPlayer1/2)
	unsigned int auxI = (house + i - 1) % 12;
	if (player % 2 == 0) {
		while (auxI >= 6 && (board[auxI] == 2 || board[auxI] == 3)) {
			board[auxI] = 0;
			auxI--;
		}
	}
	else {
		while (auxI < 6 && auxI >= 0 && (board[auxI] == 2 || board[auxI] == 3)) {
			board[auxI] = 0;
			auxI--;
		}
	}
	int sum1 = 0, sum2 = 0;
	for (int i = 6 * ((player+1) % 2);i < 6 + 6 * ((player+1) % 2);i++) { // Soma de sementes adversarias
		sum1 += board[i];																										// depois de possível captura
	}
	for (int i = 6 * (player % 2);i < 6 + 6 * (player % 2);i++) { // Soma das próprias sementes
		sum2 += board[i];																						// depois de possível captura
	}
	if (sum1 == 0) {	
		if (sum2 == 0) return false;	// É possível realizar a captura
		else return true;							// Não é possível realizar a captura
	}
	else return false;							// É possível realizar a captura
}

/*Verifica se o jogo acabou*/
void end(bool& endOfGame, unsigned int seedsPlayer1, unsigned int seedsPlayer2, int player, vector<int>& board, string name1, string name2) {
	if (seedsPlayer1 >= 25 || seedsPlayer2 >= 25 || (seedsPlayer1 == 24 && seedsPlayer2 == 24) || player >= 199) {
		setcolor(4); gotoxy(52, 12); cout << "End Game" << endl; // Indica o final do jogo
		Sleep(2000);
		setcolor(15);
		displayBoard(board, seedsPlayer1, seedsPlayer2, player, name1, name2); // Mostra o tabuleiro de jogo
		gotoxy(40, 12);
		// Verifica a razão pela qual terminou o jogo e anuncia o vencedor
		if (seedsPlayer1 >= 25) {
			setcolor(9); cout << name1 << " won" << endl;
		}
		else if (seedsPlayer2 >= 25) {
			setcolor(2); cout << name2 << " won" << endl;
		}
		else if (seedsPlayer1 == 24 && seedsPlayer2 == 24) cout << "Draw" << endl;
		else if (player >= 199) {
			if (seedsPlayer1 > seedsPlayer2) {
				setcolor(9); cout << name1 << " won" << endl;
			}
			else if (seedsPlayer1 < seedsPlayer2) {
				setcolor(2); gotoxy(40, 13); cout << name2 << " won" << endl;
			}
			else cout << "Draw" << endl;
		}
		endOfGame = true;
		setcolor(15);
	}
}

/*Verifica se a jogada seguinte é possívevl*/
void lastMove(vector<int>& board, int player, unsigned int& seedsPlayer1, unsigned int& seedsPlayer2) {
	bool continueGame = false; // Parte do pressuposto que o jogo vai terminar
	int sum = 0; // Soma as sementes do adversario do proximo jogador a jogar (jogador atual)
	for (int i = 6*(player%2);i < 6 + 6 * (player % 2);i++) {
		sum += board[i];
	}
	if (sum == 0) { // Quando o adversário não tem sementes, verifica se há alguma joagda válida na ronda seguinte
		for (int n = 6*(player+1)%2;n < 6+6*((player+1)%2);n++) {
			if (board[n] + (n%6) >= 6) {
				continueGame = true;
				break;
			}
		}
		if (!continueGame) {								// Se não for possível continuar o jogo
			if (player % 2 == 0) {						// As sementes nas casas do jogador 2 são atribuidas ao próprio
				for (int i = 6; i < 12;i++) {
					seedsPlayer2 += board[i];
					board[i] = 0;
				}
			}
			else {														// As sementes nas casas do jogador 1 são atribuidas ao próprio
				for (int i = 0; i < 6;i++) {
					seedsPlayer1 += board[i];
					board[i] = 0;
				}
			}
		}
	}
	else { // Verifica se existe alguma distribuiçao e captura válida na jogada seguinte
		for (int n = 6 * (player + 1) % 2;n < 6 + 6 * ((player + 1) % 2);n++) {
			if (!testCapturing(board, n, player)) {
				continueGame = true;
				break;
			}
		}
	}
}

int main() {
	unsigned int house=0, seedsPlayer1 = 0, seedsPlayer2 = 0;
	vector<int> board(12,4); // 0-5 -> 1st player; 6-11 -> 2nd player
	bool endOfGame = false, forcedEnd = false, validInput;
	int player = 0, i=0, k=0, p;	// player -> Conta os turnos e alterna o jogador;
	string name1, name2;
	
	p = gameModeInput();
	setNames(name1, name2, p);
	displayBoard(board, seedsPlayer1, seedsPlayer2, player - 1, name1, name2); // Mostra o tabuleiro de jogo
	while (!endOfGame) {
		// Pergunta ao jogador que casa quer escolher até ser válida, ou até terminar o jogo abruptamente
		k = 0;
		do {
			input(forcedEnd, validInput, board, player, house, seedsPlayer1, seedsPlayer2, name1, name2, k, p);
		} while (!validInput && !forcedEnd);

		//Mostra ao jogador a jogada do Random Player, caso seja o caso
		if (player % 2 == 1 && p == 1) {
			gotoxy(40, 11); cout << name2 << "'s turn: " << house-5;
			Sleep(2000);
		}

		if (!forcedEnd) { // Caso o jogo não tenha sido terminado abruptamente
			sowing(board, house, i); // Distribui as sementes
			capturing(board, house, player, i, seedsPlayer1, seedsPlayer2); // Captura as sementes
			displayBoard(board, seedsPlayer1, seedsPlayer2, player, name1, name2); // Mostra o tabuleiro de jogo
			lastMove(board, player, seedsPlayer1, seedsPlayer2); // Verifica se a jogada seguinte é possível
		}
		end(endOfGame, seedsPlayer1, seedsPlayer2, player, board, name1, name2); // Verirfica se o jogo terminou
		player++; // Alterna o jogador e conta os turnos
	}
}