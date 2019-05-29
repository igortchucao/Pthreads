#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int **mArestas;
int *cores;
int nVertices;

// FUN��ES AUXILIARES
void criagrafo(int vertices){
    //grafo *g;

    //if((g = (grafo*)malloc(sizeof(grafo)))){

    nVertices = vertices;
    cores = (int*)malloc(vertices*sizeof(int));
    mArestas = (int**)malloc(vertices*sizeof(int*));
    for (int i = 0; i < vertices; i++){
        mArestas[i] = (int*)malloc((vertices)*sizeof(int));
    }

    for (int i = 0; i < vertices; i++){
        cores[i] = 0;
        for (int j = 0; j < vertices; j++)
            mArestas[i][j] = 0;
    }

}

// FUN��ES AUXILIARES
void liberagrafo(){
    for (int i = 0; i < nVertices; i++){
        free(mArestas[i]);
    }
    free(mArestas);
    free(cores);
    //free(g);
}

// FUN��ES AUXILIARES
void zera_cores(){
    for(int i = 0; i < nVertices; i++)
        cores[i] = 0;
    //cor = 0;
}

// FUN��ES AUXILIARES
void preenche_matriz(int v){
    if(v == 0){
        // 0 = 1 ARESTA. NA PEN�LTIMA POSI��O
        mArestas[0][1] = 1;
    	mArestas[1][0] = 1;
    }else{
        // TODAS AS ARESTAS POSS�VEIS. EXCLUINDO SELF-LOOPS
    	for(int i = 0; i < nVertices; i++){
	        for(int j = 0; j < nVertices; j++){
	            if(i != j){
                    mArestas[i][j] = 1;
                    mArestas[j][i] = 1;
                }
	        }
	    }
    }
}

//FORCA BRUTA
void coloracao1(int cor){

    for(int i = 0; i < nVertices; i++)for(int j = i + 1; j < nVertices; j++)
    {
        //SE O VERTICE ATUAL NAO POSSUI COR, ENTAO ATRIBUI O 1
        if(cores[i] == 0)cores[i] = 1;

        //SE HOUVER LIGA��ES ENTRE O VERTICE i e j && O VERTICE ATUAL NAO TIVER COR ATRIBUIDA
        if(mArestas[i][j] == 1 && cores[j] == 0)
        {
            if(cores[i] != 1)cores[j] = 1;
            else cores[j] = 2;

            //VERIFICA SE HA INCOMPATIBILIDADE NA COR ESCOLHIDA PARA O VETICE j
            for(int k = 0; k < nVertices; k++)if(mArestas[j][k] == 1 && cores[j] == cores[k])
            {
                //SE FOR ENCONTRADOO ALGUM ERRO, RECOMECA A VERIFICA��O COM A DOR DO VERTICE SOMADA A 1
                k = 0;
                cores[j]++;
            }
        }
        //SE A COR PASSAR DO LIMITE, ENTAO JA PARA A FUN��O
        if(cores[j] > cor)
        {
            i = j = nVertices;
            zera_cores();
            cor++;
            coloracao1(cor);
        }
    }
}


int main(){
	criagrafo(2000);
	preenche_matriz(0);

	coloracao1(0);

	int qtdCores = -1;
    for(int i = 0; i < nVertices; i++)
        if(qtdCores < cores[i])
            qtdCores = cores[i];

    printf("QUANTIDADES DE CORES ALGORITMO FORCA BRUTA: %i", qtdCores);

	liberagrafo();
	return 0;
}
