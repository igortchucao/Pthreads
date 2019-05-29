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
int coloracao2(grafo *g);
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

grafo *dadosParaGrafo(char *argv){
    FILE *arq;
    grafo *g;
    int v1, v2, nr;

    if((arq = fopen(argv, "r"))){
        fscanf(arq, "%d", &nr);
        g = criagrafo(nr);
        while(!feof(arq)){
            fscanf(arq, "%d %d", &v1, &v2);
            insereAresta(g, v1-1, v2-1);
        }
    }else
        printf("Arquivo nao encontrado\n");

    return g;
}

// DES/ALOCAÇÃO DE MEMÓRIA
grafo *criagrafo(int vertices){
    grafo *g;

    if((g = (grafo*)malloc(sizeof(grafo)))){

        g->nVertices = vertices;
        g->mArestas = (int**)malloc(vertices*sizeof(int*));
        g->cores = (int*)malloc(vertices*sizeof(int));
        for (int i = 0; i < vertices; i++){
            g->mArestas[i] = (int*)malloc((vertices+2)*sizeof(int));
        }

        for (int i = 0; i < vertices; i++){
            for (int j = 0; j < vertices+2; j++)
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
    if(v == 1){
    	for(int i = 0; i < g->nVertices; i++){
	        for(int j = i; j < g->nVertices; j++){
	            if(i == j){
	                g->mArestas[i][j] = 1;
	            }
	        }
	    }
    }else{
    	for(int i = 0; i < g->nVertices; i++){
	        for(int j = i; j < g->nVertices; j++){

	            if(i == g->nVertices-1 && j == g->nVertices){
	                g->mArestas[j][i] = 1;
	                g->mArestas[i][j] = 1;
	            }else
	                g->mArestas[i][j] = 0;
	        }
	    }
    }
}

void imprimeMatriz(grafo *g){
    //IMPRIME MATRIZ
    printf("\t");
    for(int i = 0; i < g->nVertices; i++)
        printf("|%i", i);
    printf("|\n\n");

    for(int i = 0; i < g->nVertices; i++){
        printf("|%i|\t", i);
        for(int j = 0; j < g->nVertices+2; j++){
            if(g->mArestas[i][j] == 0)
                printf("|*");
            else
                printf("|%i",g->mArestas[i][j]);
        }
        printf("|\n");
    }
}

// HEURÍSTICAS
int coloracao1(grafo *g, int cor){

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
    int qtdCores = -1;
    for(int i = 0; i < g->nVertices; i++)
        if(qtdCores < g->cores[i])
            qtdCores = g->cores[i];
            // printf("%d ", g->cores[i]);

    return qtdCores;
}

int coloracao2(grafo *g){
    /*
        coloração sequencial. Testa, vértice por vértice, se daterminada
        cor é permitida.
        Inicialmente todas começam com a cor 1
    */
    zera_cores(g);

    int min = 0, cor = 1;
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

    return min;
}

// PTHREAD
void *funcaoThreads(void *arg_ptr){
    int *a_ptr = (int *)arg_ptr;
    int algoritmo = *a_ptr;
    clock_t ti, tf;
    int qtCores;
    float tempo;

    //DEFINIÇÃO DO TAMANHO INICIAL DO GRAFO
    int tamanho = 1000;

    do{
        grafo *g = criagrafo(tamanho);
        preenche_matriz(g, 1);

        switch(algoritmo){
            case 1:
                //executa força bruta
                ti = clock();
                qtCores = coloracao1(g, 1);
                tf = clock();

                tempo = (double)(tf-ti)/(CLOCKS_PER_SEC);
                printf("forca bruta: %d - tempo: %f s\n",qtCores, tempo);
            break;

            case 2:
                //executa heurística
                ti = clock();
                qtCores = coloracao2(g);
                tf = clock();

                tempo = (double)(tf-ti)/(CLOCKS_PER_SEC);
                printf("heuristica: %d - tempo: %f s\n",qtCores, tempo);
            break;
            default:
                break;
        }

        liberagrafo(g);
        tamanho *= 2;
    }while(tamanho != 4000/*tamanho final do grafo*/);
    pthread_exit(0);
}

int main(int argc, char *argv[]){
    char *entrada = argv[1];
    pthread_t td1, td2;

    int algoritmo1 = 1, algoritmo2 = 2;

    pthread_create(&td1, NULL, (void *)funcaoThreads, &algoritmo1);
    pthread_join(td1, NULL);

    pthread_create(&td2, NULL, (void *)funcaoThreads, &algoritmo2);
    pthread_join(td2, NULL);

    return argc;
}
