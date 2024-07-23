#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <array>

//definindo constantes
    const int ALTURA_PALCO {25};
    const int LARGURA_PALCO {100};
    const char ESPACO_VAZIO = ' ';
    const char PLAYER1 {'A'};
    const char PAREDE {'#'};
    const char BOMBA {'Q'};
    const char PLAYER2 {'B'};

int numero_aleatorio(int inicio, int fim) {
    int numero = (rand() % (fim - inicio + 1)) + inicio;
    return numero;
}

void inicializar_palco(char palco[ALTURA_PALCO][LARGURA_PALCO]) {
    std::cout << std::size(palco[0]); 
    for (int a = 0; a < ALTURA_PALCO; a++) {
        for (int b = 0; b < LARGURA_PALCO; b++) {
            if (a == 0 || a == ALTURA_PALCO - 1 || b == 0 || b == LARGURA_PALCO - 1) {
                palco[a][b] = PAREDE;
            } else {
                palco[a][b] = ESPACO_VAZIO;
            }
        }
    }
}

void copiar_array(char dest[ALTURA_PALCO][LARGURA_PALCO], const char src[ALTURA_PALCO][LARGURA_PALCO]) {
    for (int i = 0; i < ALTURA_PALCO; i++) {
        for (int j = 0; j < LARGURA_PALCO; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

void desenhar_palco(const char palco[ALTURA_PALCO][LARGURA_PALCO], const int pos_player1[2], const std::vector<std::array<int, 2>> &pos_bombas) {
    std::string buffer_saida = "\n\n\n\n\n\n\n\n\n";
    for (int a = 0; a < ALTURA_PALCO; a++) {
        for (int b = 0; b < LARGURA_PALCO; b++) {
            if (a == pos_player1[1] && b == pos_player1[0]) {
                buffer_saida += PLAYER1;
            } else {
                bool is_bomba = false;
                for (const auto &bomba : pos_bombas) {
                    if (a == bomba.at(1) && b == bomba.at(0)) {
                        buffer_saida += BOMBA;
                        is_bomba = true;
                        break;
                    }
                }
                if (!is_bomba) {
                    buffer_saida += palco[a][b];
                }
            }
        }
        buffer_saida += "\n";
        }
    std::system("cls");
    std::cout << buffer_saida;
}



int main() {
    char palco[ALTURA_PALCO][LARGURA_PALCO];
    inicializar_palco(palco);

    //criando instâcia de mapa apartir do palco
    char mapa[ALTURA_PALCO][LARGURA_PALCO];
    copiar_array(mapa, palco);
    
    //array das bombas
    std::vector<std::array<int, 2>> pos_bombas;

    //definindo a posição dos jogadores
    int pos_player1[2] {5, 5};
    int direcao[2] {0, 0};

    //variáveis do fluxo do game
    bool atualizar = true;
    char tecla = ' ';
    int numero_chance {};
    srand(time(0));
    
    //LOOP PRINCIPAL DO JOGO
    bool em_jogo = true;
    while (em_jogo == true) {
        //lidando com as mecânicas de spawn de coisas
        numero_chance = numero_aleatorio(0, 100);
        if (numero_chance <= 20) {
            while (true) {
                std::array<int, 2> pos_nova = {numero_aleatorio(0, LARGURA_PALCO), numero_aleatorio(0, ALTURA_PALCO)};
                char escolhido_pos_nova = mapa[pos_nova.at(1)][pos_nova.at(0)];
                if (escolhido_pos_nova == ESPACO_VAZIO) {
                    pos_bombas.push_back(pos_nova);
                    break;
                }
            }
        }


        //Recebendo teclas do jogador e estabelecendo intenções
        //coletando entradas do usuário
        if (_kbhit()) {
            tecla = _getch();
            if (tecla == char (27)){
                break;
            } else if (tecla == 'w' or tecla == 'W') {
                direcao[0] = 0;
                direcao[1] = -1;
            } else if (tecla == 'a' or tecla == 'A') {
                direcao[0] = -1;
                direcao[1] = 0;
            } else if (tecla == 's' or tecla == 'S') {
                direcao[0] = 0;
                direcao[1] = 1;
            } else if (tecla == 'd' or tecla == 'D') {
                direcao[0] = 1;
                direcao[1] = 0;
            }
        } else {
            direcao[0] = 0;
            direcao[1] = 0;
        }

        //aplicando alteração dos objetos
        if (direcao[0] != 0 or direcao[1] != 0) {
            int nova_pos_player1[2] = {pos_player1[0] + direcao[0], pos_player1[1] + direcao[1]};
            if (mapa[nova_pos_player1[1]][nova_pos_player1[0]] == ESPACO_VAZIO) {
                pos_player1[0] = nova_pos_player1[0];
                pos_player1[1] = nova_pos_player1[1];
                atualizar = true;
            }
        }
        
        //PRINTANDO TUDO
        if (atualizar == true) {
            desenhar_palco(mapa, pos_player1, pos_bombas);
            atualizar = false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return 0;
}