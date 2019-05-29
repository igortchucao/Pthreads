#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct dado{
    int **mArestas;
    int *cores;
    int nVertices;
} grafo;

// RESOLVENDO PROBLEMA DE FUNÇÃO NÃO DEFINIDA
int coloracao1(grafo *g, int cor);
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

void preenche_matriz(grafo *g, int v){
    if(v == 0){
        // 0 = 1 ARESTA. NA PENÚLTIMA POSIÇÃO
        g->mArestas[0][1] = 1;
    	g->mArestas[1][0] = 1;
    }else{
        // TODAS AS ARESTAS POSSÍVEIS. EXCLUINDO SELF-LOOPS
    	for(int i = 0; i < g->nVertices; i++){
	        for(int j = 0; j < g->nVertices; j++){
	            if(i != j){
                    g->mArestas[i][j] = 1;
                    g->mArestas[j][i] = 1;
                }
	        }
	    }
    }
}

int coloracao1(grafo *g, int cor){
    int corA = 1;

    for(int i = 0; i < g->nVertices; i++)for(int j = i + 1; j < g->nVertices; j++)
    {
        //SE O VERTICE ATUAL NAO POSSUI COR, ENTAO ATRIBUI O 1
        if(g->cores[i] == 0)g->cores[i] = 1;

        //SE HOUVER LIGAÇÕES ENTRE O VERTICE i e j && O VERTICE ATUAL NAO TIVER COR ATRIBUIDA
        if(g->mArestas[i][j] == 1 && g->cores[j] == 0)
        {
            if(g->cores[i] != 1)g->cores[j] = 1;
            else g->cores[j] = 2;

            //VERIFICA SE HA INCOMPATIBILIDADE NA COR ESCOLHIDA PARA O VETICE j
            for(int k = 0; k < g->nVertices; k++)if(g->mArestas[j][k] == 1 && g->cores[j] == g->cores[k])
            {
                //SE FOR ENCONTRADOO ALGUM ERRO, RECOMECA A VERIFICAÇÃO COM A DOR DO VERTICE SOMADA A 1
                k = 0;
                g->cores[j]++;
            }
        }
        //SE A COR PASSAR DO LIMITE, ENTAO JA PARA A FUNÇÃO
        if(g->cores[j] > cor)
        {
            i = j = g->nVertices;
            zera_cores(g);
            cor++;
            coloracao1(g, cor);
        }
    }

    int max = -1;
    for(int i = 0; i < g->nVertices; i++)
    	if(g->cores[i] > max)
    		max = g->cores[i];
    printf("quantidade de cor max é %d\n", max);
    return max;
}

int main(){
	grafo *g = criagrafo(10);
	preenche_matriz(g, 1);
	printf("vertices %i\n", g->nVertices);
	for(int i = 0; i < g->nVertices; i++){
            printf("\n");
            for(int j = 0; j < g->nVertices; j++)printf("%i ", g->mArestas[i][j]);
	}
	printf("\n");
	for(int i = 0; i < g->nVertices; i++)printf("%i\n", g->cores[i]);
	coloracao1(g, 1);
	for(int i = 0; i < g->nVertices; i++)printf("%i\n", g->cores[i]);
	liberagrafo(g);
	return 0;
}
