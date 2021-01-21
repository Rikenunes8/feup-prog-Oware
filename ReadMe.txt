Todos os objetivos foram atingidos.

Foi ainda implementado:
	- uma função de leitura do nome dos jogadores
	- um comando (^Z (ctrl+z) em Windows) de desistência, que faz com que as sementes sejam distribuídas de acordo com o lado do tabuleiro em que se encontrem
	- opção de jogar com 2 jogadores ou 1 jogador + 1 "Random Player" (RP)

Tutorial:
	Escolhe-se o modo de jogo, introduzindo 1 para o modo 1 vs RP e 2 para o modo 1 vs 1.
	Escolhe-se o nome do jogador 1 e do jogador 2 caso seja o modo 2. Caso seja o modo 1, apenas se escolhe o nome do jogador 1.
	Tabuleiro de jogo:
		- as casas da linha superior pertencem ao jogador dois e são ordenadas da direita para a esquerda
		- as casas da linha inferior pertencem ao jogador um e são ordenadas da esquerda para a direita
	O jogo inicia com o jogador 1, que deve escolher uma casa sua, introduzindo um número entre 1 e 6, inclusive.
	A vez de jogar vai alternando entre os dois jogadores até terminar o jogo.
	Caso seja a vez do RP jogar, no modo 1, o jogador 1 deve aguardar a sua vez.