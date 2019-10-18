#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int nVertices, ehPonderado;
    int **pesos, **arestas;
    int *grau;
} Grafo;

Grafo * criaGrafo(int vertices, int ehPonderado);
void inserirAresta(Grafo **gr, int origem, int destino, int peso, int ehDigrafo);

int main(){
    Grafo *grafo;

    grafo = criaGrafo(3, 1);
    inserirAresta(&grafo, 1, 2, 4, 0);
    inserirAresta(&grafo, 2, 3, 4, 0);
    inserirAresta(&grafo, 3, 1, 4, 0);

    printf("Arestas\n");
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            printf("%d ", grafo->arestas[j][i]);
        }
        printf("\n");
    }

    printf("Pesos\n");
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            printf("%d ", grafo->pesos[j][i]);
        }
        printf("\n");
    }
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
