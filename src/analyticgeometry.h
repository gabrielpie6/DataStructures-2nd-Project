#ifndef ANALYTIC_GEOMETRY_H
#define ANALYTIC_GEOMETRY_H

#include <stdbool.h>

#define PI 3.14159265358979323846



/*
    Módulo que provê funções relacionadas à geometria analítica úteis apra verificações de colisão,
    medidas de distância, cálculo de áreas, definição do truncamento da constante pi e uma função
    de setorização angular do plano cartesiano.
*/




/*
    Calcula a distância entre dois pontos no plano cartesiano.
    Pré-condição: x1, y1, x2 e y2 são números reais.
    Pós-condição: retorna a distância entre os pontos (x1, y1) e (x2, y2).
*/
double distance(double x1, double y1, double x2, double y2);

/*
    Calcula a área de um círculo de raio r.
    Pré-condição: radius é um número real.
    Pós-condição: retorna a área do círculo de raio radius.
*/
double CircleArea(double radius);

/*
    Calcula a área de um retângulo de lados width e height.
    Pré-condição: width e height são números reais.
    Pós-condição: retorna a área do retângulo de lados width e height.
*/
double RectangleArea(double width, double height);


/*
    SegFunction é uma função de setorização angular do plano cartesiano. SegFunction, uma função de R² em |R,
    segmenta o plano cartesiano em n setores angulares, então, daddo uma coordenada (x, y),
    SegFunction retorna o número inteiro a qual pertence o ponto (x, y) na segmentação do plano.
    Pré-condição: x e y são números reais e n é um número inteiro, n >= 1.
*/
int SegFunction(double x, double y, int n);

/*
    Dado um ponto (x1, y1) e um retângulo de vértices (Rxi, Ryi) e (Rxf, Ryf), a função
    isPointInsideRectangle verifica se o ponto está dentro do retângulo.
    Pré-condição: x1, y1, Rxi, Ryi, Rxf e Ryf são números reais.
    Pós-condição: retorna true se o ponto (x1, y1) está dentro do retângulo de vértices (Rxi, Ryi) e (Rxf, Ryf).
*/
bool isPointInsideRectangle(double x1, double y1, double Rxi, double Ryi, double Rxf, double Ryf);


#endif