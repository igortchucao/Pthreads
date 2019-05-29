#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define TOTAL_THREADS 4

int min = 0;

typedef struct dado{
    int **mArestas;
    int *cores;
    int nVertices;
} grafo;

// RESOLVENDO PROBLEMA DE FUNÇÃO NÃO DEFINIDA
int coloracao1(grafo *g);
grafo *criagrafo(int vertices);

// LEITURA DO ARQUIVO
int insereAresta(grafo *graph, int v1, int v2){
    // Verificando validade do grafo e dos vértices
    if(graph == NULL) return 0;
    if(v1 < 0 || v1 >= graph->nVertices) return 0;
    if(v2 < 0 || v2 >= graph->nVertices) return 0;

    graph->mArestas[v1][v2] = 1;
    graph->mArestas[v2][v1] = 1;
    
    return 1;
}

grafo *criagrafo(int vertices){
    grafo *g;

    if((g = (grafo*)malloc(sizeof(grafo)))){

        g->nVertices = vertices;
        g->cores = (int*)malloc(vertices*sizeof(int));
        g->mArestas = (int**)malloc(vertices*sizeof(int*));
        for (int i = 0; i < vertices; i++){
            g->mArestas[i] = (int*)malloc((vertices)*sizeof(int));
        }

        for (int i = 0; i < vertices; i++){
            for (int j = 0; j < vertices; j++)
                g->mArestas[i][j] = 0;
            g->cores[i] = 0;
        }

    }
    return g;
}

void liberagrafo(grafo *g){
    for (int i = 0; i < g->nVertices; i++){
        free(g->mArestas[i]);
    }
    free(g->mArestas);
    free(g->cores);
    free(g);
}

// FUNÇÕES AUXILIARES
void zera_cores(grafo *g){
    for(int i = 0; i < g->nVertices; i++)
        g->cores[i] = 0;
}

void imprimeMatriz(grafo *g){
    //IMPRIME MATRIZ
    printf("\t");
    for(int i = 0; i < g->nVertices; i++)
        printf("|%i", i);
    printf("|\n\n");    

    for(int i = 0; i < g->nVertices; i++){
        printf("|%i|\t", i);
        for(int j = 0; j < g->nVertices; j++){
            if(g->mArestas[i][j] == 0)
                printf("|*");
            else
                printf("|%i",g->mArestas[i][j]);
        }
        printf("|\n");
    }
}

void fillGraph(grafo *g, int v){
    if(v == 0){
        // 0 = 1 ARESTA. NA PENÚLTIMA POSIÇÃO  
        g->mArestas[0][1] = 1;
    	g->mArestas[1][0] = 1;
    }else{
        // TODAS AS ARESTAS POSSÍVEIS. EXCLUINDO SELF-LOOPS
    	for(int i = 0; i < g->nVertices; i++){
	        for(int j = i; j < g->nVertices; j++){
	            if(i != j){
                    g->mArestas[i][j] = 1;
                    g->mArestas[j][i] = 1;
                }
	        }
	    }
    }
}

int coloracao1(grafo *g){
    zera_cores(g);

    int cor = 1;
    for(int i = 0; i < g->nVertices; i++){
        for(int j = 0; j < g->nVertices; j++){
        	g->cores[i] = cor;
            if(g->mArestas[i][j] == 1){
                if(g->cores[j] == g->cores[i]){
                    cor++;
                    g->cores[i] = cor;
                    if(cor > min)
                        min = cor;
                }
            }
        }
        cor = 1;
    }

    for(int i = 0; i < g->nVertices; i++)
    	// printf("%d ", g->cores[i]);

    return min;
}
void *coloring(void *arg){
    int min = 0, cor = 1, *a_ptr = (int *)arg;
    int a = *a_ptr;
	grafo *g = criagrafo(1000);
	fillGraph(g, 1);

    for(int i = 0; i < g->nVertices; i++){
        for(int j = 0; j < g->nVertices; j++){
        	g->cores[i] = cor;
            if(g->mArestas[i][j] == 1){
                if(g->cores[j] == g->cores[i]){
                    cor++;
                    g->cores[i] = cor;
                    if(cor > min)
                        min = cor;
                }
            }
        }
        cor = 1;
    }
    printf("%d\n", min);

	liberagrafo(g);
}

int main(int argc, char *argv[]){
	// int *a_p = (int *)argv[1];
	// int a = *a_p - 1397489712;
	pthread_t t1;

	for (int i = 0; i < TOTAL_THREADS; ++i){
		// printf("thread %d em uso...:\n", i);
		pthread_create(&t1, NULL, (void *)coloring, (void*)&i);
		pthread_join(t1, NULL);
	}
	return 0;
}