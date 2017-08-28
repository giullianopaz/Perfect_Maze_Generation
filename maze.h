/*
    Autor: Giulliano Lyra Paz
    E-mail: giulliano94@gmail.com

*/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "node.h"

using namespace std;

// Arquivo de saida do labirinto perfeito
ofstream file("perfect_maze.txt");

// Classe para manipular a lista de adjacencias
class Maze{
public:
    Maze();
    ~Maze();
    Wall* createWalls(int n_row, int n_column);
    Node* createNodes(int n_row, int n_column);
    Adjacency* insertInAdjacencyList(Adjacency* p, int wall_row, int wall_column,
                                                   int node_row, int node_column);
    void makeMaze(Node* p, Wall* w, int n_row, int n_column);
    Wall* searchWall(Wall* w, int row, int column);
    Node* searchNode(Node* p, int row, int column);
    bool isAllVisited(Adjacency* p);
    int randomIndex(int len);
    Adjacency* randomAdjacencyNode(Adjacency* a, int size_adj_list);
    void depthFirstSearch(Node* p, Wall* wall, int row, int column);
    void deleteWalls(Wall* w);
    void deleteNodes(Node* p);
    void deleteAdjacencyNode(Adjacency* a);
};

Maze::Maze(){
    srand((int)time(0));
}

Maze::~Maze(){
    file.close();
}

// Cria paredes do labirinto
Wall* Maze::createWalls(int n_row, int n_column){

    Wall* wall;
    wall = NULL;

    // Cria paredes
    /*Qualquer posicao que tiver algum dos indices da coordenadas impares, eh parede*/
    for(int i = 1; i <= n_row; i += 1){
        for(int j = 1; j <= n_column; j += 1){

            // Se nao tiver indices impar, nao eh parede
            if( i % 2 == 0 and j % 2 == 0){
                continue; // Pula iteracao
            }

            if(wall == NULL){

                wall = new Wall;
                wall->row = i;
                wall->column = j;
                wall->data = '#';
                wall->next = NULL;
                wall->size = 1;

                wall->last = wall;
            }
            else{

                Wall* new_wall = new Wall;

                new_wall = new Wall;
                new_wall->row = i;
                new_wall->column = j;
                new_wall->data = '#';
                new_wall->next = NULL;
                wall->size++;

                wall->last->next = new_wall;
                wall->last = new_wall;
            }
        }
    }

    return wall;
}

// cria posicoes do labirinto
Node* Maze::createNodes(int n_row, int n_column){

    Node* start;

    // Ponteiro para o primeiro elemento
    start = NULL;
    /*Qualquer posicao que tiver os dois indices da coordenada pares eh nodo*/
    for(int i = 2; i <= n_row; i += 2){
        for(int j = 2; j <= n_column; j += 2){

            // Se start for NULL, é a primeira iteração
            // canto superior esquerdo
            if(start == NULL){

                // Aloca memoria
                start = new Node;

                // Posicoe do novo nodo no labirinto
                start->data = ' ';
                start->row = i;
                start->column = j;

                start->next = NULL;
                start->visited = false;
                start->size_adj_list = 0;
                start->size = 1;

                // Insere adjacencias na lista

                // Para o Sul
                // Lista de adjacencia recebe ponteiro para a lista de adjacencia, coordenadas da parede adjacente e do nodo adjacente para o SUL
                start->adj_list = insertInAdjacencyList(start->adj_list, i+1, j, i+2, j);
                start->size_adj_list++;

                // Para o Leste
                // Lista de adjacencia recebe ponteiro para a lista de adjacencia, coordenadas da parede adjacente e do nodo adjacente para o Leste
                start->adj_list = insertInAdjacencyList(start->adj_list, i, j+1, i, j+2);
                start->size_adj_list++;

                // Por enquando start tbm eh o ultimo
                start->last = start;

                // Pula iteracao
                continue;
            }

            // Aloca memoria novo nodo
            Node* new_node = new Node;

            // Posicoes do novo nodo
            new_node->data = ' ';
            new_node->row = i;
            new_node->column = j;

            new_node->next = NULL;
            new_node->visited = false;
            start->size++;

            // Se NAO estiver na ultima linha
            if(i != n_row-1){
                // Lista de adjacencia recebe ponteiro para a lista de adjacencia, coordenadas da parede adjacente e do nodo adjacente para o SUL
                new_node->adj_list = insertInAdjacencyList(new_node->adj_list, i+1, j, i+2, j);
                new_node->size_adj_list++;
            }

            // Se NAO estiver na ultima coluna
            if(j != n_column-1){
                // Lista de adjacencia recebe ponteiro para a lista de adjacencia, coordenadas da parede adjacente e do nodo adjacente para o Leste
                new_node->adj_list = insertInAdjacencyList(new_node->adj_list,  i, j+1, i, j+2);
                new_node->size_adj_list++;
            }

             // Se NAO estiver na primeira coluna
            if(j != 2){
                // Lista de adjacencia recebe ponteiro para a lista de adjacencia, coordenadas da parede adjacente e do nodo adjacente para o Oeste
                new_node->adj_list = insertInAdjacencyList(new_node->adj_list, i, j-1, i, j-2);
                new_node->size_adj_list++;
            }

            // Se NAO estiver na primeira linha
            if(i != 2){
                // Lista de adjacencia recebe ponteiro para a lista de adjacencia, coordenadas da parede adjacente e do nodo adjacente para o Norte
                new_node->adj_list = insertInAdjacencyList(new_node->adj_list, i-1, j, i-2, j);
                new_node->size_adj_list++;
            }



            //Insere novo nodo no fim da lista
            start->last->next = new_node;
            // Atualiza ponteiro para o ultimo
            start->last = new_node;
        }
    }

    return start;
}

//Insere na lista de adjacencia
Adjacency* Maze::insertInAdjacencyList(Adjacency* p, int wall_row, int wall_column,
                                                     int node_row, int node_column){

    if(p == NULL){

        // Aloca memoria para o nodo nodo
        p = new Adjacency;

        // Atribui coordenada do nodo adjacente
        p->node_row = node_row;
        p->node_column = node_column;
        // Atribui coordenada da parede adjacente
        p->wall_row = wall_row;
        p->wall_column = wall_column;
        p->visited = false;
        p->next = p; // Lista circular
        p->last = p; // Por enquanto, o ultimo tbm eh o primeiro
    }
    else{

        // Ponteiro para o novo nodo
        Adjacency* new_node = new Adjacency;

        // Atribui coordenada do nodo adjacente
        new_node->node_row = node_row;
        new_node->node_column = node_column;
        // Atribui coordenada da parede adjacente
        new_node->wall_row = wall_row;
        new_node->wall_column = wall_column;
        new_node->visited = false;
        new_node->next = p;// Lista circular

        p->last->next = new_node;// Adiciona do fim da lista
        p->last = new_node;// Atualiza ponteiro para o ultimo

    }

    return p;
}


// Envia labirinto para arquivo
void Maze::makeMaze(Node* p, Wall* w, int n_row, int n_column){

    for(int i = 1; i <= n_row; i++){
        for(int j = 1; j <= n_column; j++){

            // Cria entrada(1,2) e saida(2, n_column) do labirinto
            if((i == 2 and j == n_column) or (i == 1 and j == 2)){
                file << " ";
            }

            // Se indices forem pares, nao eh uma parede
            else if(i % 2 == 0 and j % 2 == 0){
                // Busca nodo e manda para o arquivo
                file << searchNode(p, i, j)->data;
            }
            // Senao, eh parede
            else{
                // Busca parede e manda dado para o arquivo
                file << searchWall(w, i, j)->data;
            }
        }
        file << "\n";
    }
}

// Busca por uma parede pelas suas coordenadas e retorna seu ponteiro
Wall* Maze::searchWall(Wall* w, int row, int column){

    Wall* aux = w;

    while(aux){

        // Se encontrou nodo
        if(aux->row == row and aux->column == column){
            // Retorna Ponteiro
            return aux;
        }

        aux = aux->next;
    }
    // Se não existe nodo, retorna NULL
    return NULL;
}

// Busca por um nodo pelas suas coordenadas e retorna seu ponteiro
Node* Maze::searchNode(Node* p, int row, int column){

    Node* aux = p;

    while(aux){

        // Se encontrou nodo
        if(aux->row == row and aux->column == column){
            // Retorna Ponteiro
            return aux;
        }

        aux = aux->next;
    }

    return NULL;
}

// Testa se todos os nodos da lista de adjacencia foram visitados
bool Maze::isAllVisited(Adjacency* p){

    Adjacency* aux = p;

    do{
        // Testa se algum ainda nao foi visitado
        if(!aux->visited)
            return false;

        aux = aux->next;

    }while(aux != p);

    // Se todos foram visitados, retorna true
    return true;
}

// Gera indice aleatorio entre 1 e len
int Maze::randomIndex(int len){
    // Valor aleatorio entre 1 e len, intervalor fechado
    return 1 + rand() % len;
}

// Escolhe nodo na lista de adjacencias aleatoriamente
Adjacency* Maze::randomAdjacencyNode(Adjacency* a, int size_adj_list){

    // Se todos foram visitados, retorna NULL
    if(isAllVisited(a))
        return NULL;

    // Ponteiro auxiliar
    Adjacency* aux = a;

    // 'anda' na lista de adjacencia ate posicao aleatoria
    for(int i = 0; i < randomIndex(size_adj_list); i++)
        aux = aux->next;

    // Se posicao escolhia ja foi visitada, 'anda' mais ate uma que nao tenha sido visitada
    while(1){
        if(aux->visited == true){
            aux = aux->next;
        }
        else{
            break;
        }
    }

    // Retorna ponteiro para posicao
    return aux;
}

// Busca em profundidade para geracao do labirinto perfeito
void Maze::depthFirstSearch(Node* p, Wall* wall, int row, int column){

    // Procura nodo na lista de nodos do labirinto
    Node* node = searchNode(p, row, column);

    // Se posicao nao existe, retorna
    if(node == NULL)
        return;

    // Ponteiro auxiliar para lista de adjacencias do nodo em questao
    Adjacency* aux_adj_list = node->adj_list;

    // Marca nodo como visitado
    node->visited = true;

    // Percorre lista de adjacencia, chamando recursivamente a DFS
    while(1){

        // Escolhe nodo adjacente aleatoriamente
        Adjacency* randomChoice = randomAdjacencyNode(aux_adj_list, node->size_adj_list);

        // Se nao ha mais nodos adjacentes para explorar, sai do laco
        if(randomChoice == NULL)
            break;

        // Marca nodo adjacente como visitado
        randomChoice->visited = true;

        // Ponteiro auxiliar para nodo adjacente
        Node* temp = searchNode(p, randomChoice->node_row, randomChoice->node_column);

        // Se nodo ainda nao foi visitado
        if(!temp->visited){

            // Busca, na lista de paredes, a parede correspondente a linha e coluna da lista de adjacencia
            // Abre a parede
            searchWall(wall, randomChoice->wall_row, randomChoice->wall_column)->data = ' ';

            // Chama funcao para nodo adjacente
            depthFirstSearch(p, wall, randomChoice->node_row, randomChoice->node_column);
        }
    }
}

// Deleta lista nodos da lista principal de grafos
void Maze::deleteWalls(Wall* w){

    while(w){

        Wall* old = w;
        w = w->next;

        delete old;
    }
}

// Deleta lista nodos da lista principal de grafos
void Maze::deleteNodes(Node* p){

    while(p){

        Node* old = p;
        p = p->next;

        deleteAdjacencyNode(old->adj_list);
        delete old->adj_list;
        // delete old; // Why doesn't work?
    }
}

void Maze::deleteAdjacencyNode(Adjacency* a){
    // Torna a lista nao circular
    a->last->next = NULL;

    while(a){

        Adjacency* old = a;

        a = a->next;

        delete old;
    }
}
