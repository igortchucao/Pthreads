void forca_bruta(int cor)
{
    int corA = 1;

    for(int i = 0; i < g.nVertices; i++)for(int j = i + 1; j < g.nVertices; j++)
    {
        //SE O VERTICE ATUAL NAO POSSUI COR, ENTAO ATRIBUI O 1
        if(g->cores[i] == 0)g->cores[i] = 1;

        //SE HOUVER LIGAÇÕES ENTRE O VERTICE i e j && O VERTICE ATUAL NAO TIVER COR ATRIBUIDA
        if(g->mArestas[i][j] == 1 && g->cores[j] == 0)
        {
            if(g->cores[i] != 1)g->cores[j] = 1;
            else g->cores[j] = 2;

            //VERIFICA SE HA INCOMPATIBILIDADE NA COR ESCOLHIDA PARA O VETICE j
            for(int k = 0; k < g.nVertices; k++)if(g->mArestas[j][k] == 1 && g->cores[j] == g->cores[k])
            {
                //SE FOR ENCONTRADOO ALGUM ERRO, RECOMECA A VERIFICAÇÃO COM A DOR DO VERTICE SOMADA A 1
                k = 0;
                g->cores[j]++;
            }
        }
        //SE A COR PASSAR DO LIMITE, ENTAO JA PARA A FUNÇÃO
        if(g->cores[j] > cor)
        {
            i = j = g.nVertices;
            zera_cores();
            cor++;
            forca_bruta(cor);
        }
    }
}
