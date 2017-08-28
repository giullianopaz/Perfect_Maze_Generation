/*
    Autor: Giulliano Lyra Paz
    E-mail: giulliano94@gmail.com

*/

//Classe para armazenar posicoes das listas de adjacencias
class Adjacency{

public:
    // Coordenadas do nodo adjacente
    int node_row;
    int node_column;
    // Coordenadas da parede adjacente
    int wall_row;
    int wall_column;
    Adjacency* next;
    bool visited;
    Adjacency* last;
};

// Classe para armazenar informacoes das paredes do labirinto
class Wall{
public:
    int row;
    int column;
    char data; // pode ser '#' ou ' '
    Wall* next;
    Wall* last;
    int size;
};

// Classe para definir nodo
class Node{

public:
    //posicoes
    char data;
    int row;
    int column;
    Node* next; // Ponteiro para o próximo
    Adjacency* adj_list; // Lista de adjacencias
    int size_adj_list;
    Node* last; // Ponteiro para o ultimo nodo
    bool visited; // Variável booleana para verificar se o nodo já foi visitado
    int size;
    Node();
    ~Node();
    
};

Node::Node(){}
