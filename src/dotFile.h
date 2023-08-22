#ifndef DOT_FILE_H
#define DOT_FILE_H

#include "radialtree.h"
#include "entity.h"

/*
    Módulo que provê uma única função destinada a escrita/exportação da estrutura de dados
    árvore radial em um arquivo de texto no formato .dot, que pode ser lido pela ferramenta
    Graphviz, conforme suas convenções próprias. Sua função percorre a árvore e anota mesmo os nós
    delidos e suas respectivas arestas, ao fim, fecha o arquivo gerado no caminho fullfilename.
    char * fullfilename deve ser o caminho completo do arquivo a ser gerado, incluindo o nome do arquivo
    com extensão .dot.
*/

void DrawTree(RadialTree tree, char * fullfilename);

#endif