#ifndef GEO_FILE_H
#define GEO_FILE_H

#include "radialTree.h"
#include "entity.h"
#include "cmdsFiles.h"

/*
    Módulo que provê as funções essenciais na definição e utilização de um arquivo .geo, conforme
    suas convenções nas descrições de "trabalho II" e "descrição geral de projetos". Um arquivo .geo
    pode conter comandos para criação de círculos (c), retângulos (r), linhas (l), textos (t) e
    definição de estilos de texto padrão (ts). Desta forma, cada uma das seguintes funções executa
    as instruções necessárias para interpretação de uma linha completa de comando válida dentro do
    arquivo .geo e retorna sua representação de formato de uma entidade válida. Para estilo de textos,
    o novo estilo de texto lido substitui o estilo já existente.
*/


int    ReadGeoFile (RadialTree Elements, char * geoPath, Style defaultStyle);

Entity readActCircle        (ArqCmds GeoFile, char * lineBuffer);
Entity readActRectangle     (ArqCmds GeoFile, char * lineBuffer);
Entity readActLine          (ArqCmds GeoFile, char * lineBuffer);
Entity readActText          (ArqCmds GeoFile, char * lineBuffer, Style style);
void   readActTextStyle     (ArqCmds GeoFile, char * lineBuffer, Style createdStyle);

#endif