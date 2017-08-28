/*
    Autor: Giulliano Lyra Paz
    Matricula: 121151618
    E-mail: giulliano94@gmail.com

*/
#include "maze.h"

using namespace std;

int main(int argc, char const *argv[]){

    srand((int)time(0));

    int start_row = 0;
    int start_column = 0;
    
    if(argc < 3){
        cout << "\n [ERROR] Dimensoes do tabuleiro devem ser informadas por argumentos!\n\n Exemplo: ~$ ./main 11 19\n" << endl;
        return 0;
    }

    int n_row = atoi(argv[1]), n_column = atoi(argv[2]);

    if(n_row % 2 == 0 or n_column % 2 == 0){
        cout << "\n [ERROR] Dimensoes do tabuleiro devem ser valores impares! \n" << endl;
        return 0;
    }

    if(n_row < 5 or n_column < 5){
        cout << "\n [ERROR] Dimensao minima eh 5 por 5! \n" << endl;
        return 0;
    }

    Maze *maze;
    Node *node = NULL;
    Wall *wall = NULL;

    cout << "\nCriando paredes... ";
    wall = maze->createWalls(n_row, n_column);
    if(wall) cout << " OK\n" << endl;
    // Cria labirinto 
    cout << "Criando Nodos... ";
    node = maze->createNodes(n_row, n_column);  
    if(node) cout << " OK" << endl;  

    start_row = maze->randomIndex(n_row-2);
    if(start_row % 2 != 0) start_row++;

    start_column = maze->randomIndex(n_column-2);
    if(start_column % 2 != 0) start_column++;

    // // Mostra buscas no grafo
    cout <<"\nGerando Labirinto Perfeito com DFS...  OK" <<endl;
    maze->depthFirstSearch(node, wall, start_row ,start_column);
    
    cout <<"\nSalvando Labirinto Perfeito no arquivo de saida...  OK" <<endl;
    maze->makeMaze(node, wall, n_row, n_column);

    cout <<"\nLabirinto Perfeito salvo em 'perfect_maze.txt'!" <<endl;

    cout << "\nLiberando Memoria... OK";
    maze->deleteWalls(wall);
    maze->deleteNodes(node);

    cout << endl;
    return 0;
}