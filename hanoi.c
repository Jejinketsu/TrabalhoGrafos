#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DISCOS 4
#define PINOS 3

#define WHITE 0
#define GRAY 1
#define BLACK 2

typedef struct {
    int nVertices, ehPonderado;
    int **pesos, **arestas;
    int *grau, *cor, *dist, *anterior;
} Grafo;

typedef struct Fila{
    int vertice;
    struct Fila *prox; 
} Fila;


void printEstado(int *atual);
int contem(int *lista, int tam, int num);
Grafo * criaGrafo(int vertices, int ehPonderado);
void inserirAresta(Grafo **gr, int origem, int destino, int peso, int ehDigrafo);
void buscaLargura(Grafo **gr, int origem);

// Funções para montar o grafo
void montaHanoi(Grafo **gr, int **estados, int tamanho);
int comparaEstado(int *estado1, int *estado2, int tamanho);
int discoEmBaixo(int *estado, int posDisco, int tamanho);
int discoEmCima(int *estado, int posDisco, int tamanho);
int posDiff(int *vetor1, int *vetor2, int tamanho);
int contEquals(int *vetor1, int *vetor2, int tamanho);

// Funções para registrar rota
int length(int *lista, int tam);
void copyList(int **copia, int *original, int tam);
Fila *enqueue(Fila *fila, int vertice);
Fila *dequeue(Fila **fila);
void showFila(Fila *fila);

int main(){

    int possibilidades = pow(PINOS, DISCOS);

    Grafo *grafo = criaGrafo(possibilidades, 0);

    int **estados = malloc(sizeof(int *)*possibilidades);
    int cont = 0;

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            for(int k = 0; k < 3; k++){
                for(int l = 0; l < 3; l++){
                    estados[cont] = malloc(sizeof(int)*3);
                    estados[cont][0] = i+1;
                    estados[cont][1] = j+1;
                    estados[cont][2] = k+1;
                    estados[cont][3] = l+1;
                    cont++;
                }
            }
        }
    }

    montaHanoi(&grafo, estados, possibilidades);
    buscaLargura(&grafo, 1);
    
    return 0;
}

Grafo * criaGrafo(int vertices, int ehPonderado){
    Grafo *gr;
    gr = (Grafo *) malloc(sizeof(Grafo));

    if(gr != NULL){
        gr->nVertices = vertices;
        gr->ehPonderado = (ehPonderado != 0)? 1:0;

        gr->grau = (int*) calloc(vertices, sizeof(int));
        gr->cor = (int*) calloc(vertices, sizeof(int)); 
        gr->dist = (int*) calloc(vertices, sizeof(int));
        gr->anterior = (int*) calloc(vertices, sizeof(int));

        gr->arestas = (int**) malloc(vertices*sizeof(int*));
        for(int i = 0; i < vertices; i++)
            gr->arestas[i] = (int*) calloc(vertices, sizeof(int));

        if(gr->ehPonderado){
            gr->pesos = (int**) malloc(vertices * sizeof(int*));
            for(int i = 0; i < vertices; i++)
                gr->pesos[i] = (int*) calloc(vertices, sizeof(int));
        }

    }

    return gr;
}

void inserirAresta(Grafo **gr, int origem, int destino, int peso, int ehDigrafo){
    if(gr != NULL){ 
        if(origem > 0 && origem <= (**gr).nVertices){
            if(destino > 0 && destino <= (**gr).nVertices){
                (**gr).arestas[origem-1][(**gr).grau[origem-1]] = destino;

                if((**gr).ehPonderado)
                    (**gr).pesos[origem-1][(**gr).grau[origem-1]] = peso;

                (**gr).grau[origem-1]++;

                if(!ehDigrafo){
                    inserirAresta(gr, destino, origem, peso, 1);
                }
            }
        }
    }
}

void buscaLargura(Grafo **gr, int origem){
    (*gr)->cor[origem-1] = GRAY;
    (*gr)->dist[origem-1] = 0;
    (*gr)->anterior[origem-1] = -1;
    Fila *agendados = NULL;

    agendados = enqueue(agendados, origem);
    while(agendados != NULL){
        Fila *aux = dequeue(&agendados);
        for(int i = 0; i < (*gr)->grau[aux->vertice-1]; i++){
            if((*gr)->cor[((*gr)->arestas[aux->vertice-1][i])-1] == WHITE){
                (*gr)->cor[((*gr)->arestas[aux->vertice-1][i])-1] = GRAY;
                (*gr)->dist[((*gr)->arestas[aux->vertice-1][i])-1] = ((*gr)->dist[aux->vertice-1])+1;
                (*gr)->anterior[((*gr)->arestas[aux->vertice-1][i])-1] = aux->vertice;
                agendados = enqueue(agendados, (*gr)->arestas[aux->vertice-1][i]);
            }
        }
        (*gr)->cor[aux->vertice-1] = BLACK;
    }
}

void printEstado(int *atual){
    printf("(%d,%d,%d,%d)", atual[0], atual[1], atual[2], atual[3]);
}

void montaHanoi(Grafo **gr, int **estados, int tamanho){
    for(int i = 0; i < tamanho; i++){
        for(int j = 0; j < tamanho; j++){
            if(comparaEstado(estados[j], estados[i], DISCOS)){
                inserirAresta(gr, i+1, j+1, 0, 1);
            }
        }
    }
}

int comparaEstado(int *estado1, int *estado2, int tamanho){
    int temAresta = 0;

    if(contEquals(estado1, estado2, tamanho) == DISCOS-1){
        int posicao = posDiff(estado1, estado2, tamanho);
        if(posicao != -1){
            if(!discoEmCima(estado1, posicao, tamanho)){
                if(!discoEmBaixo(estado2, posicao, tamanho)){
                    temAresta = 1;
                }
            }
        }
    }

    return temAresta;
}

int discoEmBaixo(int *estado, int posDisco, int tamanho){
    int discoAbaixo = 0;
    for(int i = posDisco+1; i < tamanho; i++) 
        if(estado[i] == estado[posDisco]) discoAbaixo = 1;
    return discoAbaixo;
}

int discoEmCima(int *estado, int posDisco, int tamanho){
    int discoAcima = 0;
    for(int i = posDisco+1; i < tamanho; i++) 
        if(estado[i] == estado[posDisco]) discoAcima = 1;
    return discoAcima;
}

int posDiff(int *vetor1, int *vetor2, int tamanho){
    int pos = -1;
    for(int i = 0; i < tamanho; i++) if(vetor1[i] != vetor2[i]) pos = i;
    return pos;
}

int contEquals(int *vetor1, int *vetor2, int tamanho){
    int cont = 0;
    for(int i = 0; i < tamanho; i++) if(vetor1[i] == vetor2[i]) cont++;
    return cont;
}

int contem(int *lista, int tam, int num){
    int contem = 0;

    for(int i = 0; i < tam; i++)
        if(lista[i] == num) contem = 1;
    
    return contem;
}

int length(int *lista, int tam){
    int cont = 0;
    for(int i = 1; i < tam && lista[i] != 0; i++, cont++);
    return cont;
}

void copyList(int **copia, int *original, int tam){
    for(int i = 0; i < tam && original[i] != 0; i++, copia[0][i] = original[i]);
}

Fila *enqueue(Fila *fila, int vertice){
    Fila *elemento = malloc(sizeof(Fila));
    elemento->vertice = vertice;
    elemento->prox = NULL;

    Fila *aux;
    if(fila != NULL){
        for(aux = fila; aux != NULL && aux->prox != NULL; aux = aux->prox);

        if(aux == NULL) fila = elemento;
        else aux->prox = elemento;
    }
    else fila = elemento;
    
    return fila;
}

Fila *dequeue(Fila **fila){
    Fila *aux = (*fila);
    (*fila) = (*fila)->prox;
    return aux;
}

void showFila(Fila *fila){
    if(fila != NULL){
        printf("%d ", fila->vertice);
        showFila(fila->prox);
    }
}