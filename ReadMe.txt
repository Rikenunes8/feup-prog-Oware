Todos os objetivos foram atingidos.

Foi ainda implementado:
	- uma fun��o de leitura do nome dos jogadores
	- um comando (^Z (ctrl+z) em Windows) de desist�ncia, que faz com que as sementes sejam distribu�das de acordo com o lado do tabuleiro em que se encontrem
	- op��o de jogar com 2 jogadores ou 1 jogador + 1 "Random Player" (RP)

Tutorial:
	Escolhe-se o modo de jogo, introduzindo 1 para o modo 1 vs RP e 2 para o modo 1 vs 1.
	Escolhe-se o nome do jogador 1 e do jogador 2 caso seja o modo 2. Caso seja o modo 1, apenas se escolhe o nome do jogador 1.
	Tabuleiro de jogo:
		- as casas da linha superior pertencem ao jogador dois e s�o ordenadas da direita para a esquerda
		- as casas da linha inferior pertencem ao jogador um e s�o ordenadas da esquerda para a direita
	O jogo inicia com o jogador 1, que deve escolher uma casa sua, introduzindo um n�mero entre 1 e 6, inclusive.
	A vez de jogar vai alternando entre os dois jogadores at� terminar o jogo.
	Caso seja a vez do RP jogar, no modo 1, o jogador 1 deve aguardar a sua vez.