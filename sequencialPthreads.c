#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NR_VERTICES 1000

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int min = 0;
int minBruto = 0;
int cor = 1;

int g[NR_VERTICES][NR_VERTICES];
int cores[NR_VERTICES];

// FUNÇÕES AUXILIARES
void preenche(){
    // g[0][1] = 1;
    // g[1][0] = 1;
    for (int i = 0; i < NR_VERTICES; i++){
        for (int j = 0; j < NR_VERTICES; j++){
            if(i != j)
                g[i][j] = 1;//rand() % 2;
            else
                g[i][j] = 0;
        }
    }
}

// FUNÇÕES AUXILIARES
void zera_cores(){
    for(int i = 0; i < NR_VERTICES; i++)
        cores[i] = 0;
    //cor = 0;
}

// FUNÇÕES AUXILIARES
void imprimeMatriz(){
    //IMPRIME MATRIZ
    printf("\t");
    for(int i = 0; i < NR_VERTICES; i++)
        printf("|%i", i);
    printf("|\n\n");

    for(int i = 0; i < NR_VERTICES; i++){
        printf("|%i|\t", i);
        for(int j = 0; j < NR_VERTICES; j++){
            if(g[i][j] == 0)
                printf("|*");
            else
                printf("|%i",g[i][j]);
        }
        printf("|\n");
    }
}

/*FUNÇÃO "heuristica"
* - FUNÇÃO BASEADA NA COLOÇÃO DE GRAFOS SEQUENCIAL"
*/
void *heuristica(void *arg){
    int *a_p = (int *)(arg);
    int a = *a_p;
    //printf("a:%d\n", a);
    int cor = 1;
    for(int i = 0; i < NR_VERTICES; i++){

        for(int j = 0; j < NR_VERTICES; j++){
        }
    }
}

/*FUNÇÃO "ForcaBruta*/
void *ForcaBruta(void *corMax){

    int * cor = (int *)(corMax);

    for(int i = 0; i < NR_VERTICES; i++)for(int j = i + 1; j < NR_VERTICES; j++)
    {
        //SE O VERTICE ATUAL NAO POSSUI COR, ENTAO ATRIBUI O 1
        if(cores[i] == 0)cores[i] = 1;

        //SE HOUVER LIGAÇÕES ENTRE O VERTICE i e j && O VERTICE ATUAL NAO TIVER COR ATRIBUIDA
        if(g[i][j] == 1 && cores[j] == 0)
        {
            if(cores[i] != 1)cores[j] = 1;
            else cores[j] = 2;

            //VERIFICA SE HA INCOMPATIBILIDADE NA COR ESCOLHIDA PARA O VETICE j
            for(int k = 0; k < NR_VERTICES; k++)if(g[j][k] == 1 && cores[j] == cores[k])
            {
                //SE FOR ENCONTRADOO ALGUM ERRO, RECOMECA A VERIFICAÇÃO COM A DOR DO VERTICE SOMADA A 1
                k = 0;
                cores[j]++;
            }
        }
        //SE A COR PASSAR DO LIMITE, ENTAO JA PARA A FUNÇÃO
        if(&cores[j] > cor)
        {
            i = j = NR_VERTICES;
            zera_cores();
        }
    }
}

void *teste(void *arg){
    pthread_mutex_lock(&mutex);
    printf("%d\n", *(int *)arg);
    for(int j = 0; j < NR_VERTICES; j++){

    }
    pthread_mutex_unlock(&mutex);
}
int main(int argc, char *argv[]){
    int a;
    time_t ti, tf;
    pthread_t t1;
    preenche();

    for(int i = 0; i < NR_VERTICES; i++){
        pthread_create(&t1, NULL, teste, (void *)(&i));
        pthread_join(t1, NULL);
    }

    printf("\n\nnumero de cores: %d\ntempo total: %f", min, (double)(tf-ti)/(CLOCKS_PER_SEC));

    return 0;
}
