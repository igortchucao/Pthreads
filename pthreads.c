#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NR_VERTICES 5000

pthread_mutex_t lock;

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
                g[i][j] = rand() % 2;
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
    pthread_mutex_lock(&lock);
    int j;
    cor = 1;
    int * a = (int *)(arg);
    //printf("%i\n", *a);
    for(int i = 0; i < NR_VERTICES; i++)
        for(j = 0; j < NR_VERTICES; j++){
            if(cores[i] == 0)cores[i] = cor;
            if(g[i][j] == 1){
                if(cores[j] == cores[i]){
                    cor++;
                    cores[i] = cor;
                    j = 0;
                }
            }
        }
    pthread_mutex_unlock(&lock);
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

int main(int argc, char *argv[]){
    srand(time(NULL));
    time_t ti, tf;
    pthread_t td;
    pthread_t t1[NR_VERTICES];
    preenche();

    pthread_mutex_init(&lock, NULL);

    ti = time(NULL);
    for(int i = 0; i < NR_VERTICES; i++) pthread_create(&(t1[i]), NULL, ForcaBruta, (void *)(&i));
    for(int i = 0; i < NR_VERTICES; i++) pthread_join(t1[i], NULL);
    tf = time(NULL);
    for (int i = 0; i < NR_VERTICES; i++)if(cores[i] > minBruto) minBruto = cores[i];
    printf("\nnumero de cores(bruto): %d\ntempo total: %f", minBruto, (double)(tf-ti));

    zera_cores();

    ti = time(NULL);
    pthread_create(&td, NULL, heuristica, (void *)1);
    pthread_join(td, NULL);
    tf = time(NULL);
    for (int i = 0; i < NR_VERTICES; i++)if(cores[i] > min) min = cores[i];
    printf("\n\nnumero de cores: %d\ntempo total: %f", min, (double)(tf-ti));

    pthread_mutex_destroy(&lock);

    return 0;
}
