#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int NUM_VERTS;

#define WHITE 0
#define GRAY 1
#define BLACK 2
#define TESTES 1

typedef struct {
    int nVertices, ehPonderado;
    int **pesos, **arestas;
    int *grau, *cor, *dist, *anterior, *dinheiro;
} Grafo;

typedef struct Fila{
    int vertice;
    struct Fila *prox; 
} Fila;

void montaGrafo(Grafo **gr);
Grafo * criaGrafo(int vertices, int ehPonderado);
void inserirAresta(Grafo **gr, int origem, int destino, int peso, int ehDigrafo);
int buscaProfundidade(Grafo **gr, int origem, int *visitados, int **rotaFinal, int passos, int dinheiro);
int buscaLargura(Grafo **gr, int origem, int **rotaFinal, int dinheiro);

// Funções para registrar rota
void montaRota(Grafo **gr, int origem, int **rotaFinal, int final);
int contem(int *lista, int tam, int num);
int length(int *lista);
void append(int **copia, int valor, int local);
void copyList(int **copia, int *original, int tam, int local);
Fila *enqueue(Fila *fila, int vertice);
Fila *dequeue(Fila **fila);
void showFila(Fila *fila);

//Funções para fazer testes
long getMicrotime();

int main(){
    Grafo *grafo;
    int *visitados = calloc(NUM_VERTS, sizeof(int));
    int *tempoP = calloc(TESTES, sizeof(int));
    int *tempoL = calloc(TESTES, sizeof(int));
    int **rotaFinal = malloc(sizeof(int*));
    rotaFinal[0] = calloc(NUM_VERTS, sizeof(int));
    rotaFinal[1] = calloc(NUM_VERTS, sizeof(int));

    int dinheiroL = 0;
    int dinheiroP = 0;

    /*  ENTRADA
        A primeira linha da entrada recebe dois inteiros V e A representando o numero
        de vertices e o numero de arestas, respectivamente.
        As A linhas seguintes são compostas por três inteiros O, D, P representando a
        origem, destino e peso, respectivamente. EX:
        6 8
        1 2 3
        1 5 4
        1 6 6
        4 6 2
        4 2 5
        4 3 7
        2 3 2
        5 6 3
    */
    montaGrafo(&grafo);
    for(int j = 0; j < TESTES; j++){
        long inicio, fim, tempo;

        inicio = getMicrotime();
        dinheiroP = buscaProfundidade(&grafo, 1, visitados, rotaFinal, 0, 40);
        fim = getMicrotime();

        tempo = fim-inicio;
        //printf("%ld\n", tempo);
        tempoP[j] = tempo;
        inicio = getMicrotime();
        dinheiroL = buscaLargura(&grafo, 1, rotaFinal, 40);
        fim = getMicrotime();

        tempo = fim-inicio;
        //printf("%ld\n", tempo);
        tempoL[j] = tempo;
    }
    /*
    printf("Melhor rota com profundidade: ");
    for(int i = 1; i < NUM_VERTS && rotaFinal[0][i] != 0; i++) printf("%d ", rotaFinal[0][i]);
    printf("\n");

    printf("Melhor rota com largura: ");
    for(int i = 1; i < NUM_VERTS && rotaFinal[1][i] != 0; i++) if(rotaFinal[1][i] <= NUM_VERTS)printf("%d ", rotaFinal[1][i]);
    printf("\n");

    printf("Profundidade: %d\n", dinheiroP);
    printf("Largura: %d\n", dinheiroL);
    */
    
    printf("\nTempo Profundidade:\n");
    for(int i = 0; i < TESTES; i++) printf("%d\n", tempoP[i]);
    printf("\nTempo Largura:\n");
    for(int i = 0; i < TESTES; i++) printf("%d\n", tempoL[i]);
    

    return 0;
}

void montaGrafo(Grafo **gr){

    int origem, destino, peso, arestas;

    scanf("%d %d", &NUM_VERTS, &arestas);
    *gr = criaGrafo(NUM_VERTS, 1);
    
    for(int i = 0; i < arestas; i++){
        scanf("%d %d %d", &origem, &destino, &peso);
        inserirAresta(gr, origem, destino, peso, 0);
    }

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
        gr->dinheiro = (int*) calloc(vertices, sizeof(int));

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

int buscaProfundidade(Grafo **gr, int origem, int *visitados, int **rotaFinal, int passos, int dinheiro){
    int troco = 40;
    if(gr != NULL) {
        visitados[passos] = origem;

        if(length(*rotaFinal) < length(visitados)){
            copyList(rotaFinal, visitados, NUM_VERTS, 0);
            for(int i = 1; i < NUM_VERTS && rotaFinal[0][i] != 0; i++) printf("%d ", rotaFinal[0][i]);
            printf("\n");
            troco = dinheiro;
        }

        for(int i = 0; i < (*gr)->grau[origem-1]; i++){
            if(!contem(visitados, passos, (*gr)->arestas[origem-1][i]) && dinheiro >= (*gr)->pesos[origem-1][i]){
                troco = buscaProfundidade(gr, (*gr)->arestas[origem-1][i], visitados, rotaFinal, passos+1, dinheiro-(*gr)->pesos[origem-1][i]);
                if(troco > dinheiro) troco = dinheiro;
                visitados[passos+1] = 0;
            }
        }
    }
    return troco;
}

int buscaLargura(Grafo **gr, int origem, int **rotaFinal, int dinheiro){
    (*gr)->cor[origem-1] = GRAY;
    (*gr)->dist[origem-1] = 0;
    (*gr)->anterior[origem-1] = -1;
    (*gr)->dinheiro[origem-1] = dinheiro;
    Fila *agendados = NULL;
    int troco = dinheiro, final = 0;;

    agendados = enqueue(agendados, origem);
    while(agendados != NULL){
        Fila *aux = dequeue(&agendados);
        for(int i = 0; i < (*gr)->grau[aux->vertice-1]; i++){
            if((*gr)->cor[((*gr)->arestas[aux->vertice-1][i])-1] == WHITE && (*gr)->pesos[aux->vertice-1][i] <= (*gr)->dinheiro[aux->vertice-1]){
                (*gr)->cor[((*gr)->arestas[aux->vertice-1][i])-1] = GRAY;
                (*gr)->dist[((*gr)->arestas[aux->vertice-1][i])-1] = ((*gr)->dist[aux->vertice-1])+1;
                (*gr)->anterior[((*gr)->arestas[aux->vertice-1][i])-1] = aux->vertice;
                (*gr)->dinheiro[((*gr)->arestas[aux->vertice-1][i])-1] = ((*gr)->dinheiro[aux->vertice-1])-(*gr)->pesos[aux->vertice-1][i];
                agendados = enqueue(agendados, (*gr)->arestas[aux->vertice-1][i]);
            }
        }
        (*gr)->cor[aux->vertice-1] = BLACK;
        final = aux->vertice;
        if(troco > (*gr)->dinheiro[aux->vertice-1]) troco = (*gr)->dinheiro[aux->vertice-1];
        //printf("dinheiro %d: %d\n", aux->vertice, (*gr)->dinheiro[aux->vertice-1]);
    }
    
    montaRota(gr, origem, rotaFinal, final);
    return troco;
}

void montaRota(Grafo **gr, int origem, int **rotaFinal, int final){
    if(gr != NULL) {
        for(int i = 0; final != origem; i++){
            append(rotaFinal, final, 1);
            printf("final: %d\n", final);
            final = (*gr)->anterior[final-1];
        }   
    }
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

void append(int **copia, int valor, int local){
    int i;
    for(i = 0; i < NUM_VERTS && copia[local][i] != 0; i++);
    if(i < NUM_VERTS) copia[local][i] = valor;
}

void copyList(int **copia, int *original, int tam, int local){
    int i = 0;
    for(i = 0; i < tam && original[i] != 0; i++, copia[local][i] = original[i]);
    for(i; i < tam; i++) copia[local][i] = 0;
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

long getMicrotime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}