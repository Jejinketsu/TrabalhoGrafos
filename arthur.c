#include <stdio.h>
#include <stdlib.h>

#define NUM_VERTS 6

typedef struct {
    int nVertices, ehPonderado;
    int **pesos, **arestas;
    int *grau;
} Grafo;

Grafo * criaGrafo(int vertices, int ehPonderado);
void inserirAresta(Grafo **gr, int origem, int destino, int peso, int ehDigrafo);
int ** buscaProfundidade(Grafo **gr, int origem, int *visitados, int **rotaFinal, int passos, int dinheiro);

int contem(int *lista, int tam, int num);

// Funções para registrar rota
int length(int *lista);
void copyList(int **copia, int *original);

int main(){
    Grafo *grafo;
    int *visitados = calloc(NUM_VERTS, sizeof(int));
    int **rotaFinal = malloc(sizeof(int*));
    rotaFinal[0] = calloc(NUM_VERTS, sizeof(int));

    grafo = criaGrafo(NUM_VERTS, 1);
    inserirAresta(&grafo, 1, 2, 3, 0);
    inserirAresta(&grafo, 1, 5, 4, 0);
    inserirAresta(&grafo, 1, 6, 6, 0);
    inserirAresta(&grafo, 4, 6, 2, 0);
    inserirAresta(&grafo, 4, 2, 5, 0);
    inserirAresta(&grafo, 4, 3, 7, 0);
    inserirAresta(&grafo, 2, 3, 2, 0);
    inserirAresta(&grafo, 5, 6, 3, 0);

    /*
    inserirAresta(&grafo, 1, 2, 4, 0);
    inserirAresta(&grafo, 2, 3, 4, 0);
    inserirAresta(&grafo, 3, 1, 4, 0);
    */

    rotaFinal = buscaProfundidade(&grafo, 1, visitados, rotaFinal, 0, 16);
    for(int i = 1; i < NUM_VERTS; i++) printf("%d ", rotaFinal[0][i]);
    printf("\n");

    return 0;
}

Grafo * criaGrafo(int vertices, int ehPonderado){
    Grafo *gr;
    gr = (Grafo *) malloc(sizeof(Grafo));

    if(gr != NULL){
        gr->nVertices = vertices;
        gr->ehPonderado = (ehPonderado != 0)? 1:0;
        gr->grau = (int*) calloc(vertices, sizeof(int));

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

int ** buscaProfundidade(Grafo **gr, int origem, int *visitados, int **rotaFinal, int passos, int dinheiro){
    if(gr != NULL) {
        visitados[passos] = origem;

        if(length(*rotaFinal) < length(visitados)){
            copyList(rotaFinal, visitados);
        }

        for(int i = 0; i < (*gr)->grau[origem-1]; i++){
            if(!contem(visitados, passos, (*gr)->arestas[origem-1][i]) && dinheiro >= (*gr)->pesos[origem-1][i]){
                int **retorno = buscaProfundidade(gr, (*gr)->arestas[origem-1][i], visitados, rotaFinal, passos+1, dinheiro-(*gr)->pesos[origem-1][i]);
                if(length(*rotaFinal) < length(*retorno)) rotaFinal = retorno;
                visitados[passos+1] = 0;
            }
        }

    }
    return rotaFinal;
}

int contem(int *lista, int tam, int num){
    int contem = 0;

    for(int i = 0; i < tam; i++)
        if(lista[i] == num) contem = 1;
    
    return contem;
}

int length(int *lista){
    int cont = 0;
    for(int i = 1; i < NUM_VERTS && lista[i] != 0; i++, cont++);
    return cont;
}

void copyList(int **copia, int *original){
    for(int i = 0; i < NUM_VERTS && original[i] != 0; i++, copia[0][i] = original[i]);
}