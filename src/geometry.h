#ifndef GEOMETRY_H
#define GEOMETRY_H

/*
    Módulo que provê a definição de tipos abstratos de dados para quatro formas geométricas básicas
    e suas funções de processamento:
    Obs.: Todas as formas possuem coordenadas, angulo de rotação corrente e cores de borda
          e preenchimento (exceto para linha).
        - Círculo: é uma uma forma que possui circunfêrência, um centro e um raio.
        - Retângulo: é uma forma que possui quatro arestas, sendo dois pares de dois segmentos paralelos,
            um centro, uma largura e altura.
        - Linha: é uma forma que possui dois pontos de âncoras que se conectam por um segmento.
        - Texto: é uma forma que possui um ponto de âncora de onde o texto passará a ser escrito,
            um Style.
    -> Style é um tipo abstrato de dados que armazena as propriedades de um texto, como fonte,
       tamanho e peso.

    CONVENÇÃO:
        O tipo de uma forma geométrica (getGeoClass) é dado da seguinte maneira:
        - Círculo:   'c'
        - Retângulo: 'r'
        - Linha:     'l'
        - Texto:     't'
*/


typedef void * Geometry;
typedef void * Style;

#define COLOR_STRING_SIZE 100

// Processing functions
/*
    Desloca a forma geométrica fornecida em dx e dy unidades.
    Pré-condição: a forma geométrica deve ser válida.
*/
void      Dislocate_Geo      (Geometry geo, double dx, double dy);

/*
    Rotaciona a forma geométrica fornecida em um ângulo fornecido.
    Pré-condição: a forma geométrica deve ser válida.
*/
void      Rotate_Geo         (Geometry geo, double angle);

/*
    Remove a memória total alocada pela forma geométrica fornecida.
    Pré-condição: a forma geométrica deve ser válida.
*/
void      removeGeo          (Geometry geo);


// Create functions
/*
    Cria uma forma geométrica do tipo círculo.
    Pré-condição: id deve ser um valor inteiro
                  x e y devem ser valores reais.
                  radius deve ser um valor real não negativo.
                  colorBorder e colorFill devem ser strings válidas.
    Pós-condição: retorna uma forma geométrica do tipo círculo.
*/
Geometry  createCircle       (int id, double x, double y, double radius, char * colorBorder, char * colorFill);

/*
    Cria uma forma geométrica do tipo retângulo.
    Pré-condição: id deve ser um valor inteiro
                  x e y devem ser valores reais.
                  width e height devem ser valores reais não negativos.
                  colorBorder e colorFill devem ser strings válidas.
    Pós-condição: retorna uma forma geométrica do tipo retângulo.
*/
Geometry  createRectangle    (int id, double x, double y, double width, double height, char * colorBorder, char * colorFill);

/*
    Cria uma forma geométrica do tipo linha.
    Pré-condição: id deve ser um valor inteiro
                  x1, y1, x2 e y2 devem ser valores reais.
                  colorFill deve ser uma string válida.
    Pós-condição: retorna uma forma geométrica do tipo linha.
*/
Geometry  createLine         (int id, double x1, double y1, double x2, double y2, char * colorFill);

/*
    Cria uma forma geométrica do tipo texto.
    Pré-condição: id deve ser um valor inteiro
                  x e y devem ser valores reais.
                  colorBorder e colorFill devem ser strings válidas.
                  anchor deve ser um caractere de controle que representa a posição da âncora.
                  text deve ser uma string válida.
    Pós-condição: retorna uma forma geométrica do tipo texto.
*/
Geometry  createText         (int id, double x, double y, char * colorBorder, char * colorFill, char anchor, char * text);

/*
    Cria um estilo de texto.
    Pré-condição: fontFamily deve ser uma string válida.
                  fontWeight deve ser uma string válida.
                  fontSize deve ser um valor real não negativo.
    Pós-condição: retorna um estilo de texto pronto para ser associado a um texto.
*/
Style     createTextStyle    (char * fontFamily, char * fontWeight, double fontSize);

/*
    Copia todos os atributos para uma nova forma geométrica idêntica
    Pré-condição: é uma forma geométrica válida.
    Pós-condição: retorna uma forma geométrica idêntica realocada na memória.
*/
Geometry copyGeo (Geometry geo);

/// General functions
//  Getters
/*
    PARA O SEGUINTE CONJUNTO DE FUNÇÕES:
        getGeoCords: retorna um vetor de duas posições com as coordenadas x e y respectivamente da forma geométrica.
        getGeoAngle: retorna o ângulo de rotação corrente da forma geométrica.
        getGeoClass: retorna o tipo da forma geométrica segundo convenção estabelecida.
        getGeoBorder_color: retorna a cor da borda da forma geométrica.
        getGeoFill_color: retorna a cor de preenchimento da forma geométrica.
    Pré-condição: a forma geométrica deve ser válida.
*/
double *  getGeoCords        (Geometry geo);
double    getGeoAngle        (Geometry geo);
char      getGeoClass        (Geometry geo);
char   *  getGeoBorder_color (Geometry geo);
char   *  getGeoFill_color   (Geometry geo);
//
//  Setters
/*
    PARA O SEGUINTE CONJUNTO DE FUNÇÕES:
        setGeoCords: define as coordenadas x e y da forma geométrica.
        setGeoAngle: define o ângulo de rotação da forma geométrica.
        setGeoClass: define o tipo da forma geométrica segundo convenção estabelecida.
        setGeoBorder_color: define a cor da borda da forma geométrica.
        setGeoFill_color: define a cor de preenchimento da forma geométrica.
    Pré-condição: a forma geométrica deve ser válida.
*/
void      setGeoCords        (Geometry geo, double x, double y);
void      setGeoAngle        (Geometry geo, double angle);
void      setGeoClass        (Geometry geo, char class);
void      setGeoBorder_color (Geometry geo, char * color);
void      setGeoFill_color   (Geometry geo, char * color);


// Circle functions
/*
    PARA O SEGUINTE CONJUNTO DE FUNÇÕES:
        getGeoRadius: retorna o raio do círculo fornecido.
        setGeoRadius: define o raio da círculo fornecido.
    Pré-condição: a forma geométrica deve ser um círculo.
*/
double    getGeoRadius       (Geometry geo);
void      setGeoRadius       (Geometry geo, double radius);

// Rectangle functions
/*
    PARA O SEGUINTE CONJUNTO DE FUNÇÕES:
        getGeoWidth: retorna a largura do retângulo fornecido.
        getGeoHeight: retorna a altura do retângulo fornecido.
        setGeoWidth: define a largura do retângulo fornecido.
        setGeoHeight: define a altura do retângulo fornecido.
    Pré-condição: a forma geométrica deve ser um retângulo.
*/
double    getGeoWidth        (Geometry geo);
double    getGeoHeight       (Geometry geo);
void      setGeoWidth        (Geometry geo, double width);
void      setGeoHeight       (Geometry geo, double height);

// Line functions
/*
    PARA O SEGUINTE CONJUNTO DE FUNÇÕES:
        getGeoAnchor_1: retorna um vetor de duas posições com as coordenadas x e y respectivamente da âncora 1 da linha fornecida.
        getGeoAnchor_2: retorna um vetor de duas posições com as coordenadas x e y respectivamente da âncora 2 da linha fornecida.
        setGeoAnchor_1: define as coordenadas x e y da âncora 1 da linha fornecida.
        setGeoAnchor_2: define as coordenadas x e y da âncora 2 da linha fornecida.
    Pré-condição: a forma geométrica deve ser uma linha.
*/
double  * getGeoAnchor_1     (Geometry geo);
double  * getGeoAnchor_2     (Geometry geo);
void      setGeoAnchor_1     (Geometry geo, double x, double y);
void      setGeoAnchor_2     (Geometry geo, double x, double y);

// Text functions
/*
    PARA O SEGUINTE CONJUNTO DE FUNÇÕES:
        getGeoAnchor: retorna o caractere de controle que representa a posição da âncora do texto fornecido.
        getGeoText: retorna a string de texto do texto fornecido.
        getGeoStyle: retorna o estilo de texto do texto fornecido.
        setGeoAnchor: define o caractere de controle que representa a posição da âncora do texto fornecido.
        setGeoText: define a string de texto do texto fornecido.
        setGeoStyle: define o estilo de texto do texto fornecido.
    Pré-condição: a forma geométrica deve ser um texto.
*/
char      getGeoAnchor       (Geometry geo);
char    * getGeoText         (Geometry geo);
Style     getGeoStyle        (Geometry geo);
void      setGeoAnchor       (Geometry geo, char anchor);
void      setGeoText         (Geometry geo, char * text);
void      setGeoStyle        (Geometry geo, Style style);

// Style functions
/*
    PARA O SEGUINTE CONJUNTO DE FUNÇÕES:
        getStyleFontWeight: retorna a string que representa o peso da fonte do estilo fornecido.
        getStyleFontFamily: retorna a string que representa a família da fonte do estilo fornecido.
        getStyleFontSize: retorna o tamanho da fonte do estilo fornecido.
        setStyleFontWeight: define a string que representa o peso da fonte do estilo fornecido.
        setStyleFontFamily: define a string que representa a família da fonte do estilo fornecido.
        setStyleFontSize: define o tamanho da fonte do estilo fornecido.
    Pré-condição: o estilo deve ser válido.
*/
void      removeStyle        (Style style);
char    * getStyleFontWeight (Style style);
char    * getStyleFontFamily (Style style);
double    getStyleFontSize   (Style style);
void      setStyleFontWeight (Style style, char * fontWeight);
void      setStyleFontFamily (Style style, char * fontFamily);
void      setStyleFontSize   (Style style, double fontSize);

#endif