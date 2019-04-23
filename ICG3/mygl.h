#ifndef _MYGL_H_
#define _MYGL_H_

#include <stdlib.h>
#include <iostream>
#include "definitions.h"

//*****************************************************************************
// Defina aqui as suas funções gráficas
//*****************************************************************************

struct Posicao{
    int x;
    int y;
};


void putPixel(Posicao p, int *cor){

    for(unsigned int i = 0; i < 4; i++){
        FBptr[p.x*4+i + p.y*4*IMAGE_WIDTH] = cor[i];
    }
}


void Interpolation(int i, int *cor){
    cor[0] = i;
    cor[1] = 255-i;
}


void DrawLine(Posicao pInicial, Posicao pFinal, int *cor){
    int xi = pInicial.x;
    int yi = pInicial.y;
    int xf = pFinal.x;
    int yf = pFinal.y;
    int dx = xf - xi;
    int dy = yf - yi;
    int x = xi;
    int y = yi;
    int d; // Variável de decisão
    int xAux = 1;
    int yAux = 1;
    Posicao aux;

    if(dx < 0){
        xAux = -xAux;
    }
    if(dy < 0){
        yAux = -yAux;
    }

    //Se d < 0, então o pixel (x+1, y) é selecionado. Senão, o pixel (x+1, y+1) é selecionado
    if(abs(dx) > abs(dy)){
        d = (abs(dy) * 2) - abs(dx);
        for(int i = 0; i < abs(dx); i++){
            aux.x = x;
            aux.y = y;
            putPixel(aux, cor);

            if(d < 0){
                d += abs(dy) * 2;
            }
            else{
                d += (abs(dy) - abs(dx)) * 2;
                y += yAux;
            }
            x += xAux;
        }
    }
    //Caso dx seja menor que dy 
    else{
        d = (abs(dx) * 2) - abs(dy);
        for(int i = 0; i < abs(dy); i++){

            aux.x = x;
            aux.y = y;
            putPixel(aux, cor);

            if(d < 0){
                d += abs(dx) * 2;
            }
            else{
                d += (abs(dx) - abs(dy)) * 2;
                x += xAux;
            }
            y += yAux;
        }
    }
}

void drawTriangles(Posicao p1, Posicao p2, Posicao p3, int *cor){
    DrawLine(p1, p2, cor);
    DrawLine(p1, p3, cor);
    DrawLine(p2, p3, cor);
}


#endif // _MYGL_H_