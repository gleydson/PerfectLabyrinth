#include "libs/ed_base.h"
#include "libs/ed_mat.h"

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stack>

using namespace std;

int nlinhas = 20;
int ncolunas = 35;

//MAPA DE CORES
//r red
//g green
//b blue
//m magenta
//c cyan -
//y yellow
//w white
//k black

void queimar(matchar &mat, Par par){
    //PONTOS DE PARADA
    if(!mat.is_inside(par))//se estiver fora da matriz
        return;
    if(mat.get(par) != 'g')//se nao for arvore
        return;

    //ACAO
    mat.get(par) = 'r';

    //DESENHO
    mat_draw(mat);//desenha a matriz
    mat_focus(par, 'c');//faz uma bolinha em par
    ed_show();//mostra na tela

    //RECURSAO
    queimar(mat, Par(par.l + 1, par.c    ));//down
    queimar(mat, Par(par.l - 1, par.c    ));//up
    queimar(mat, Par(par.l    , par.c + 1));//right
    queimar(mat, Par(par.l    , par.c - 1));//left

    //ACAO APOS A RECURSAO
    mat.get(par) = 'k';

    //DESENHO
    mat_draw(mat);//desenha a matriz
    mat_focus(par, 'c');//faz uma bolinha em par
    ed_show();//mostra na tela
}

vector<Par> pegar_vizinhos(Par par){
    vector<Par> vizinhos;
    vizinhos.push_back(Par(par.l, par.c - 1));
    vizinhos.push_back(Par(par.l, par.c + 1));
    vizinhos.push_back(Par(par.l - 1, par.c));
    vizinhos.push_back(Par(par.l + 1, par.c));
    return vizinhos;
}
vector<Par> shuffle(vector<Par> &vet){
    for(int i = 0; i < (int) vet.size(); i++){
        std::swap(vet[i], vet[rand() % (int) vet.size()]);
    }
    return vet;
}

int qtd_lados_verdes(matchar &mat, Par par){
    auto vet = pegar_vizinhos(par);
    int count = 0;
    for(auto ele : vet)
        if(mat.is_inside(ele))
            if(mat.get(ele) == 'g'){
                count++;
            }
    return count;
}

void criar_lab(matchar &mat, Par par){
    if(!mat.is_inside(par))
        return;
    if(mat.get(par) != 'w')//se nao for arvore
        return;
    if(qtd_lados_verdes(mat, par) > 1)
        return;
    mat_draw(mat);//desenha a matriz
    mat_focus(par, 'c');//faz uma bolinha em par
    ed_show();//mostra na tela

    mat.get(par) = 'g';

    mat_draw(mat);//desenha a matriz
    mat_focus(par, 'c');//faz uma bolinha em par
    ed_show();//mostra na tela

    auto aux = pegar_vizinhos(par);
    shuffle(aux);
    for(auto ele : aux)
        criar_lab(mat, ele);

    mat_draw(mat);//desenha a matriz
    mat_focus(par, 'c');//faz uma bolinha em par
    ed_show();//mostra na tela/
}

void caminho(matchar &mat, Par par, Par ponto){
    if(!mat.is_inside(par))
        return;
    if(mat.get(par) != 'g')//se nao for arvore
        return;
    if(par == ponto){
        mat.get(par) = 'r';
        return;
    }

    mat_draw(mat);//desenha a matriz
    mat_focus(par, 'c');//faz uma bolinha em par
    ed_show();//mostra na tela/

    mat.get(par) = 'y';

    mat_draw(mat);//desenha a matriz
    mat_focus(par, 'c');//faz uma bolinha em par
    ed_show();//mostra na tela/

    auto aux = pegar_vizinhos(par);
    shuffle(aux);
    for(auto ele : aux)
        if(mat.get(ponto) != 'r')
            caminho(mat, ele, ponto);

    if(mat.get(ponto) == 'r')
        mat.get(par) = 'k';

    mat_draw(mat);//desenha a matriz
    mat_focus(par, 'c');//faz uma bolinha em par
    ed_show();//mostra na tela/
}


int main(){

    srand(time(NULL));

    //cria uma matriz de caracteres de 15 linhas por 20 colunas, e preenche todos os elementos
    //com 'y'
    matchar mat(15, 20, 'w');

    //chama o metodo de desenho livre onde a cor primeira default eh branca
    mat_paint_brush(mat, "wg");

    //Par par = mat_get_click(mat, "escolha uma arvore para queimar");
    Par par(0, 0);
    //queimar(mat, par);//chama a função recursiva
    criar_lab(mat, par);

    Par par1 = mat_get_click(mat, "escolha o pt 1");
    Par par2 = mat_get_click(mat, "escolha o pt 2");

    caminho(mat, par1, par2);

    ed_lock();//impede que termine abruptamente

    return 0;
}


