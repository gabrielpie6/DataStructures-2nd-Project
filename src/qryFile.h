#ifndef QRY_FILE_H
#define QRY_FILE_H

#include "radialTree.h"
#include "cmdsFiles.h"
#include "entity.h"

#include <stdio.h>



/*
    Módulo que provê as funções essenciais e básicas para leitura, interpretação e execução dos comandos
    contidos dentro de um arquivo do tipo .qry, conforme as especificações do trabalho. Pode-se
    informar uma colheitadeira (cl), mover uma entidade (mv), colher uma região (hvt), contaminar com
    pragas uma região (ct), aplicar defensivos em uma região (cr), aplicar fertilizantes em uma região
    (ad), espalhar sementes de hortaliças (st), reportar atributos de figuras (d?) e consultar as colheitadeiras
    existentes (c?). Os comandos são executados conforme a ordem em que aparecem no arquivo e são sensiveis
    a esta organização. Evidentemente, para todas as execuções deve haver um caminho válido para a consulta de um
    arquivo .qry, bem como um caminho válido para a saída de um arquivo .txt considerado relatório de execuções de
    comandos, também conforme convenções e especificações do trabalho.

    Cada linha dentro de um arquivo .qry válido é interpretada como um comando/instrução a ser executada.
    Uma linha com comando desconheço é ignorada.
*/



int ReadQryFile (RadialTree Elements, char * qryPath, char * outputPath, char * geo_qryCombination, Style style, Lista Decos);

void DefineHarvester (ArqCmds QryFile, RadialTree Elements, char * lineBuffer, FILE * TXTFile, Lista Decos);
void HarvestRegion (ArqCmds QryFile, RadialTree Elements, char * lineBuffer, FILE * TXTFile, Lista Decos);
void moveEntity (ArqCmds QryFile, RadialTree Elements, char * lineBuffer, FILE * TXTFile, Lista Decos);

void ContaminateRegion (ArqCmds QryFile, RadialTree Elements, char * lineBuffer, FILE * TXTFile, Lista Decos, Style style);
void HealRegion (ArqCmds QryFile, RadialTree Elements, char * lineBuffer, FILE * TXTFile, Lista Decos);
void FertilizeRegion (ArqCmds QryFile, RadialTree Elements, char * lineBuffer, FILE * TXTFile, Lista Decos);
void SpreadRegion (ArqCmds QryFile, RadialTree Elements, char * lineBuffer, FILE * TXTFile, Lista Decos);

void ReportAttributtes (ArqCmds QryFile, RadialTree Elements, char * lineBuffer, FILE * TXTFile, Lista Decos);
void ReportHarvesters (ArqCmds QryFile, RadialTree Elements, char * lineBuffer, FILE * TXTFile, Lista Decos);

#endif