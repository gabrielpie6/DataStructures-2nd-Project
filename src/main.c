#include "radialtree.h"
#include "entity.h"
#include "geoFile.h"
#include "qryFile.h"
#include "dotFile.h"
#include "cmdsFiles.h"
#include "path.h"
#include "drawSvg.h"
#include "actions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função para printagem
void printNode(RadialTree tree, Node node, void * aux)
{
    Entity ent = getInfoRadialT(tree, node);
    int id = getEntId(ent);
    double * p = getPointRadialT(tree, node);
    printf("(%.2lf, %.2lf) [%3d]\n", p[0], p[1], id);
}


void main(int argc, char * argv[])
{
    int i, j;

    char * entryArg    = NULL;
    char * outputArg   = NULL;
    char * geoArg      = NULL;
    char * qryArg      = NULL;

    char * geoFile     = NULL;
    char * geoFilePath = NULL;
    char * geoFileName = NULL;

    char * qryFile     = NULL;
    char * qryFilePath = NULL;
    char * qryFileName = NULL;

    char * entryPath   = NULL;
    char * outputPath  = NULL;

    int numsectors;
    double factor;

    // SEPARADOR DE PARÂMETROS
    for (i = 1; i < argc; i++)
    {
        // Diretório de base de ENTRADA (Opcional)
        if (strcmp(argv[i], "-e") == 0)
        {
            // TRATAR CASOS DE DIRETÓRIOS
            entryArg = argv[i + 1];
            entryPath = malloc(sizeof(char) * (strlen(entryArg) + 1));
            normalizePath(entryArg, entryPath, strlen(entryPath) + 1);
        }
        else
        // Diretório base de SAÍDA (OBRIGATÓRIO)
        if (strcmp(argv[i], "-o") == 0)
        {
            // TRATAR CASOS DE DIRETÓRIO
            outputArg = argv[i + 1];
            outputPath = malloc(sizeof(char) * (strlen(outputArg) + 1));
            normalizePath(outputArg, outputPath, strlen(outputPath) + 1);
        }
        else
        // Endereço ou nome de arg.geo (deve estar contigo em -e) (OBRIGATÓRIO)
        if (strcmp(argv[i], "-f") == 0)
        {
            // TRATAR ENDEREÇO E NOME
            geoArg = argv[i + 1];
            geoFileName = malloc(sizeof(char) * (strlen(geoArg) + 1));
            geoFilePath = malloc(sizeof(char) * (strlen(geoArg) + 1));
            getFileName(geoArg, geoFileName, strlen(geoArg) + 1);
            getPath(geoArg, geoFilePath, strlen(geoArg) + 1);

            // Verificar se tem ./ no início, então remover
            if (geoFilePath[0] == '.' && geoFilePath[1] == '/')
                sprintf(geoFilePath, "%s", geoFilePath + 2);
            if (geoFilePath[0] == '.')
                geoFilePath[0] = '\0';
            geoFileName = realloc(geoFileName, sizeof(char) * (strlen(geoFileName) + 1));
            geoFilePath = realloc(geoFilePath, sizeof(char) * (strlen(geoFilePath) + 1));
        }
        else
        // Endereço ou nome de arg.qry (deve estar contigo em -e) (Opcional)
        if (strcmp(argv[i], "-q") == 0)
        {
            // TRATAR ENDEREÇO E NOME
            qryArg = argv[i + 1];
            qryFileName = malloc(sizeof(char) * (strlen(qryArg) + 1));
            qryFilePath = malloc(sizeof(char) * (strlen(qryArg) + 1));
            getFileName(qryArg, qryFileName, strlen(qryArg) + 1);
            getPath(qryArg, qryFilePath, strlen(qryArg) + 1);

            // Verificar se tem ./ no início, então remover
            if (qryFilePath[0] == '.' && qryFilePath[1] == '/')
                sprintf(qryFilePath, "%s", qryFilePath + 2);
            if (qryFilePath[0] == '.')
                qryFilePath[0] = '\0';
            qryFileName = realloc(qryFileName, sizeof(char) * (strlen(qryFileName) + 1));
            qryFilePath = realloc(qryFilePath, sizeof(char) * (strlen(qryFilePath) + 1));
        }
        else
        //
        if (strcmp(argv[i], "-ns") == 0)
        {
            //printf("num: [%s]\n", argv[i + 1]);
            numsectors = atoi(argv[i + 1]);
        }
        else
        //
        if (strcmp(argv[i], "-fd") == 0)
        {
            //printf("factor: [%s]\n", argv[i + 1]);
            factor = atof(argv[i + 1]);
        }
    }

    // Tratamento de caminho para o diretório base de entrada
    if (entryArg == NULL)
    {
        // Não foi especificado um diretório de entrada, então seguir diretriz: 
        // "Caso não seja informado, o diretório de entrada é o diretório corrente da aplicação."
        entryPath = malloc(sizeof(char) * 2);
        strcpy(entryPath, ".");
    }

    geoFile = malloc(sizeof(char) * (strlen(entryPath) + 1 + strlen(geoFilePath) + 1 + strlen(geoFileName) + 1));

    // Tratamento do caminho para .geo
    strcpy(geoFile, entryPath);
    if (strcmp(geoFilePath, "") != 0)
        sprintf(geoFile, "%s/%s", geoFile, geoFilePath);
    sprintf(geoFile, "%s/%s", geoFile, geoFileName);

    if (qryArg != NULL)
    {
        // Existe um .qry
        qryFile = malloc(sizeof(char) * (strlen(entryPath) + 1 + strlen(qryFilePath) + 1 + strlen(qryFileName) + 1));
        // Tratamento do caminho para .qry
        strcpy(qryFile, entryPath);
        if (strcmp(qryFilePath, "") != 0)
            sprintf(qryFile, "%s/%s", qryFile, qryFilePath);
        sprintf(qryFile, "%s/%s", qryFile, qryFileName);
    }
    /*
    // Vizualização dos caminhos
    printf("- entryPath:  %s\n", entryPath);
    printf("- outputPath: %s\n", outputPath);
    printf("- geoFile:    %s\n", geoFile);
    printf("- qryFile:    %s\n", qryFile);
    printf("- geoPath:    %s\n", geoFilePath);
    printf("- qryPath:    %s\n", qryFilePath);
    printf("- geoName:    %s\n", geoFileName);
    printf("- qryName:    %s\n", qryFileName);
    */








    /*  Convenção de valores-padrão e inicialização
        Como o estilo dos textos é único e aplicado a todos os textos, então pode ser armazenado como uma variável pseudo "global"
    */
    
    Style style = createTextStyle("arial", "normal", 16);
    


    /////////////////////////////////////////////
    //       LEITURA DO ARQUIVO .geo           //
    /////////////////////////////////////////////
    factor = factor/100.0;
    RadialTree Elements = newRadialTree(numsectors, factor);
    if (ReadGeoFile(Elements, geoFile, style))
    {
        printf("ERRO: [in main]: nao foi possivel ler arquivo .geo\n");
        return;
    }

    ////////////////////////////////////
    // TRATAMENTO DOS NOMES DOS ARQUIVOS
    //
    char * geoFileNameWithoutExt = malloc(sizeof(char) * (strlen(geoFileName) + 1));
    getFileNameWithoutExt(geoFileName, geoFileNameWithoutExt, strlen(geoFileName) + 1);
    geoFileNameWithoutExt = realloc(geoFileNameWithoutExt, sizeof(char) * (strlen(geoFileNameWithoutExt) + 1));
    
    
    char * mainSvgFile = malloc(sizeof(char) * (strlen(outputPath) + 1 + strlen(geoFileNameWithoutExt) + strlen(".svg") + 1));
    sprintf(mainSvgFile, "%s/%s.svg", outputPath, geoFileNameWithoutExt);

    char * dotFile = malloc(sizeof(char) * (strlen(outputPath) + 1 + strlen(geoFileNameWithoutExt) + strlen(".dot") + 1));
    sprintf(dotFile, "%s/%s.dot", outputPath, geoFileNameWithoutExt);


    ////////////////////////////////////

    // Produção do arquivo .svg após leitura do .geo
    ArqSvg MainSVG = abreEscritaSvg(mainSvgFile);
    WriteRadialTreeEntsInSVG(MainSVG, Elements, style);
    fechaSvg(MainSVG);
    DrawTree(Elements, dotFile);


    /////////////////////////////////////////////
    //       LEITURA DO ARQUIVO .qry           //
    /////////////////////////////////////////////
    Lista Decos = createLst(-1);
    if (qryArg != NULL)
    {
        char * qryFilenameWithoutExt = malloc(sizeof(char) * (strlen(qryFileName) + 1));
        getFileNameWithoutExt(qryFileName, qryFilenameWithoutExt, strlen(qryFileName) + 1);
        qryFilenameWithoutExt = realloc(qryFilenameWithoutExt, sizeof(char) * (strlen(qryFilenameWithoutExt) + 1));

        char * geo_qry = malloc(sizeof(char) * (strlen(geoFileNameWithoutExt) + 1 + strlen(qryFilenameWithoutExt) + 1));
        sprintf(geo_qry, "%s-%s", geoFileNameWithoutExt, qryFilenameWithoutExt);
        
        char * finalSvgFile = malloc(sizeof(char) * (strlen(outputPath) + 1 + strlen(geoFileNameWithoutExt) + 1 + strlen(qryFilenameWithoutExt) + strlen(".svg") + 1));
        sprintf(finalSvgFile, "%s/%s-%s.svg", outputPath, geoFileNameWithoutExt, qryFilenameWithoutExt);

        
        if (ReadQryFile(Elements, qryFile, outputPath, geo_qry, style, Decos))
        {
            printf("ERRO: [in main]: nao foi possivel ler arquivo .qry\n");
            return;
        }
        ArqSvg FinalSVG = abreEscritaSvg(finalSvgFile);
        WriteRadialTreeEntsInSVG(FinalSVG, Elements, style);
        if (!isEmptyLst(Decos)) WriteGeoListInSvg(FinalSVG, Decos, style, 0, 0);
        fechaSvg(FinalSVG);

        char * finaldotFile = malloc(sizeof(char) * (strlen(outputPath) + 1 + strlen(geoFileNameWithoutExt) + 1 + strlen(qryFilenameWithoutExt) + strlen(".dot") + 1));
        sprintf(finaldotFile, "%s/%s-%s.dot", outputPath, geoFileNameWithoutExt, qryFilenameWithoutExt);
        DrawTree(Elements, finaldotFile);
    }
    printf("Executado com sucesso\n");
}