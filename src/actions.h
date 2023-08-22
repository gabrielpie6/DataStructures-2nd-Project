#ifndef ACTIONS_H
#define ACTIONS_H

#include <stdio.h>

#include "geoFile.h"
#include "drawSvg.h"


/*
    Módulo que provê funções usuais para a manipulação de entidades, entidades em árvores Radiais,
    formas geométricas e suas representações em arquivos SVG. As funções contidas neste módulo são de uso
    específico e auxiliar para outros módulos complexos que necessitam destas manipulações.
*/



/*
    Escreve todas as entidades presentes na árvore radial em um arquivo SVG válido, com um estilo de texto style.
    Entidades removidas da árvore radial não são escritas no arquivo SVG.
    Pré-condição: tree é uma árvore radial válida, SVG é um arquivo SVG válido, style é um estilo de texto válido.
    Pós-condição: Todas as entidades presentes na árvore radial são escritas no arquivo SVG, conforme as convenções
                  necessárias da linguagem de marcação.
*/
void   WriteRadialTreeEntsInSVG (ArqSvg SVG, RadialTree tree, Style style);

/*
    Escreve uma dada lista de formas geométricas L em um arquivo SVG válido, com um estilo de texto style a partir
    de uma posição (dx, dy) em relação ao canto superior esquerdo do arquivo SVG (origem).
    Pré-condição: L é uma lista de formas geométricas válidas, SVG é um arquivo SVG válido, style é um estilo de texto válido.
*/
void     WriteGeoListInSvg             (ArqSvg SVG, Lista L, Style style, double dx, double dy);

/*
    Escreve os atributos de uma dada entidade ent válida em um arquivo TXT válido. É utilizada pra transcrever as
    informações básicas contidas na entidade para exportação.
    Pré-condição: TXTFile é um arquivo TXT válido, ent é uma entidade válida.
    Pós-condição: Os atributos da entidade são escritos no arquivo TXT em linha única.
*/
void writeEntAttributtes(FILE * TXTFile, Entity ent);


/*
    Realiza o que é considerado a colheita de todas as entidade inteiramente contidas dentro da colheitadeira harvester
    em steps passos, na direção direction. As entidades colhidas são removidas da árvore radial tree e são contabilizadas,
    o resultado da contabilidade parcial desta colheita é retornado pela função.
    Pré-condição: tree é uma árvore radial válida, harvester é uma entidade do tipo colheitadeira válida, 
                  steps é um inteiro positivo, direction é um caractere válido conforme convenção, 
                  TXTFile é um arquivo TXT válido, Decos é uma lista de formas geométricas válidas.
    Pós-condição: adiciona uma forma geométrica de retâncgulo com bordas pontilhadas na lista Decos, 
                  remove todas as entidades colhidas da árvore radial tree, escreve no arquivo TXTFile
                  as informações básicas de cada entidade colhida, retorna o valor total da colheita.
*/
double DoHarvest (RadialTree tree, Entity harvester, int steps, char direction, FILE * TXTFile, Lista Decos);

/*
    Dada uma entidade válida, realiza a contabilizade desta figura seguindo as regras de contabilização em convenção.
    Pré-condição: ent é uma entidade válida.
    Pós-condição: retorna o valor da contabilização da entidade.
*/
double PlantValue (Entity plant);

/*
    (IMPLEMENTADA ANTES DA INSERÇÃO DA FUNÇÃO DE BUSCA NO MÓDULO radialtree.h)
    Realiza a busca de uma entidade na árvore radial tree a partir de um inteiro id válido e único na árvore.
    Pré-condição: tree é uma árvore radial válida, id é um inteiro válido relacionado a uma entidade existente e
                  único na árvore.
    Pós-condição: retorna o nó relacionado à entidade encontrada na árvore radial tree a partir do inteiro id.
*/
Node searchNodeByEntID(RadialTree tree, int id);


/*
    Verifica se a entidade está inteiramente contida dentro de um círculo de centro (Cx, Cy) e raio Cr.
    Pré-condição: ent é uma entidade válida, Cx, Cy e Cr são números reais válidos, Cr > 0.
    Pós-condição: retorna true se a entidade está inteiramente contida dentro do círculo, false caso contrário.
*/
bool isEntInsideCircle(Entity ent, double Cx, double Cy, double Cr);


/*
    Verifica se um círculo de centro (Cx, Cy) e raio Cr está inteiramente contido dentro de uma entidade.
    Pré-condição: ent é uma entidade válida, Cx, Cy e Cr são números reais válidos, Cr > 0.
    Pós-condição: retorna true se o círculo está inteiramente contido dentro da entidade, false caso contrário.
*/
bool isCircleInsideEnt(Entity ent, double Cx, double Cy, double Cr);


/*
    Verifica se uma entidade está inteiramente contida dentro de um retângulo formado pelas coordenadas opostas
    (x1, y1) e (x2, y2).
    Pré-condição: ent é uma entidade válida, x1, y1, x2 e y2 são números reais válidos, x1 < x2 e y1 < y2.
    Pós-condição: retorna true se a entidade está inteiramente contida dentro do retângulo, false caso contrário.
*/
bool isEntInsideRectangle(Entity ent, double x1, double y1, double x2, double y2);

/*
    Dada uma entidade válida do tipo hortaliça, a função retorna uma string com o nome da espécie da hortaliça.
    Isto é, "abobora' , "repolho", "cebola", "cenoura", "morango". É uma função que deve ser usada apenas para
    escrita/exportação de informações de hortaliças.
    Pré-condição, veg é uma entidade válida do tipo hortaliça.
    Pós-condição: retorna uma string com o nome da espécie da hortaliça.
*/
char * strVegSpecies (Entity veg);

#endif