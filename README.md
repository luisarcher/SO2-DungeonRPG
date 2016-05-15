# SO2-DungeonRPG
Projecto Final - Sistemas Operativos 2 - Engenharia Informática
________________________
Registar os clientes num array de estruturas do tipo Jogador [FEITO]

Estruturas de comunicação de cliente e servidor

Servidor a receber comandos do cliente através da estrutura criada

Cliente recebe o seu line of sight processado pelo servidor

Quando receber os comandos do cliente, verificar se pode mover (parede á volta)
  - Criar uma func chamada CheckPlayerSurroundings() -> Jogador.c

Verificar se o user pode apanhar as pedras no chão, só pode carregar 15

O MoverJogador não pode por a posição anterior vazia, pode ter sido atacado por um monstro, então a posição onde estava seria um monstro.

Como representar um player e monstro na mesma posição?
-ideia actual para o hash: (numMonstro*100) + num player
-exemplo: monstro(57) + player(2) = 57*100+2 = 5702
-4 digitos, 2 para cada identificador

StoneAutohit on/off

O broadcast é só para os jogadores que têm HP > 0, definir a HP = 0, quando um jogador sai ou morre, assim, é facil saber quem ainda está em "linha".

As threads que alteram dados do mapa precisam de MutEx

Se o jogador já tiver uma mensagem privada por entregar e houver uma mensagem de broadcast (tipo, o jogo começou), concatena esta ultima.

TimingMatrix to work with ActualizaClientes Thread

fechar a thread respectiva ao cliente quando ele se desliga