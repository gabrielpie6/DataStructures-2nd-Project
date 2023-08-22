#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "path.h"

// sizeof(normPath) >= sizeof(path)
void normalizePath(char *path, char *normPath, int lenNormPath)
{
    int pathlen = strlen(path);

    strcpy(normPath, path);
    if ((pathlen - 1) >= 0 && path[pathlen - 1] == '/')
        normPath[pathlen - 1] = '\0';
}


/*
   Dado um caminho completo, retorna seu respectivo path.

   fullPath           => path
   ================================
   "aaa/bbb/ccc.txt"  => "aab/bbb"
   "ccc.txt"          => ""
   "aaa/bbb/ccc"      => "aaa/bbb"
 */
void getPath(char *fullPath, char *path, int lenPath)
{
    int i = strlen(fullPath) - 1;
    int lenFullPath = strlen(fullPath) + 1;
    char *buffer = malloc(sizeof(char) * lenFullPath);
    
    strcpy(buffer, fullPath);

    // Buscar por '/' olhando da direita para esquerda
    while (i >= 0 && fullPath[i] != '/')
        i--;
    if (i >= 0)
    {
        buffer[i] = '\0';
        strcpy(path, buffer);
    }
    else
        strcpy(path, "");
    //strcpy(buffer, path);
    //normalizePath(buffer, path, lenPath);
    
    free(buffer);
}


/*
    Dado um caminho completo (fullPath), copia em fileName o nome do arquivo 
    (possivelmente) com sua extensao. Exemplos:

    fullPath          => fileName
    ==============================
    "aaa/bb/cc.txt"   => "cc.txt"
    "cc.txt"          => "cc.txt"
    "cc."             => "cc."
    "cc"              => "cc"
    ".txt"            => ".txt"
 */
void getFileName(char *fullPath, char *fileName, int lenFileName)
{
    int i = strlen(fullPath) - 1;
    int lenFullPath = strlen(fullPath) + 1;
    char *buffer = malloc(sizeof(char) * lenFullPath);
    
    strcpy(buffer, fullPath);

    // Buscar por '/' olhando da direita para esquerda
    while (i >= 0 && fullPath[i] != '/')
        i--;
    if (i >= 0)
    {
        strcpy(fileName, buffer + i + 1);
    }
    else
        strcpy(fileName, fullPath);
    
    free(buffer);
}



/*
   Semelhante a joinFilePath, porem, a extensao do arquivo e´ informado explicitamente
   pelo parametro ext. Exemplos:

   path       fileName  ext     => fullPath
   =================================================
   "aaa/bbb"  "ccc"     ".txt"  => "aaa/bbb/ccc.txt"
   "aaa/bbb/  "ccc"     ".txt"  => "aaa/bbb/ccc.txt"
   ""         "ccc"     ".txt"  => "ccc.txt"
   "aaa/bbb"  "ccc"     ""      => "aaa/bbb/ccc"
 */
void joinAll(char *path, char *fileName, char *ext, char *fullPath, int lenFullPath)
{
    int lenPath = strlen(path) + 1;
    char *buffer = malloc(sizeof(char) * lenPath);

    normalizePath(path, buffer, lenPath);

    strcat(fullPath, buffer);
    if (strcmp(path, "") != 0)
        strcat(fullPath, "/");
    strcat(fullPath, fileName);
    strcat(fullPath, ext);

    free(buffer);
}



/*
  Dado um caminho (path) e um nome de arquivo (possivelmente, com sua extensao),
  copia para fullPath o caminho completo do arquivo. Exemplos:

   path         fileName =>  fullPath
   =============================================
   "aaa/bbb"    "c.txt"  =>  "aaa/bbb/c.txt"
   "aaa/bbb/"   "c.txt"  =>  "aaa/bbb/c.txt"
   ""           "c.txt"  =>  "c.txt"
   "aaa/bbb"    "c"      =>  "aaa/bbb/c"
 */
void joinFilePath(char *path, char *fileName, char *fullPath, int lenFullPath)
{
    int lenPath = strlen(path) + 1;
    char *buffer = malloc(sizeof(char) * lenPath);

    normalizePath(path, buffer, lenPath);

    strcat(fullPath, buffer);
    if (strcmp(path, "") != 0)
        strcat(fullPath, "/");
    strcat(fullPath, fileName);

    free(buffer);
}


/*
    Dado um caminho (fullPath), copia cada um de seus componentes
    nos parametros path (caminho do arquivo), nomeArq (nome do arquivo, sem
    seu path e sem sua extensao) e extArq (extensao do arquivo).
    Estes parametros sao vetores de tamanhos, respectivamente, lenPath, 
    lenNomeArq e lenExtArq. Exemplos:

    fullPath               path        nomeArqq   extArq
    =====================================================
    "aaa/bbb/ccc.txt" =>   "aaa/bbb"   "ccc"      ".txt"
    "ccc.txt"         =>   ""          "ccc"      ".txt"
    ".txt"            =>   ""          ""         ".txt"
    "ccc.ddd.txt"     =>   ""          "ccc.ddd"  ".txt"
    "ccc"             =>   ""          "ccc"      ""  
 */
// Estou considerando que SEMPRE haverá um nomeAqrqq ou extArq, mesmo que path falte.
//      (Pode não haver caminho, mas não é permitido haver apenas caminho como parâmetro)
void splitPath(char *fullPath,
	       char *path, int lenPath,
	       char *nomeArq, int lenNomeArq,
	       char *extArq, int lenExtArq)
{
    int i, k;
    int lenFullPath = strlen(fullPath) + 1;
    char *buffer = malloc(sizeof(char) * lenFullPath);
    
    strcpy(buffer, fullPath);

    // Buscar por '.' olhando da direita para esquerda
    i = strlen(fullPath) - 1;
    while (i >= 0 && fullPath[i] != '.')
        i--;
    if (i >= 0)
    {
        strcpy(extArq, fullPath + i);
        buffer[i] = '\0';

        // Buscar por '/' olhando da direita para esquerda
        k = strlen(fullPath) - 1;
        while (k >= 0 && fullPath[k] != '/')
            k--;
        if (k >= 0)
        {
            strcpy(nomeArq, buffer + k + 1);
            buffer[k] = '\0';

            strcpy(path, buffer);
        }
        else
        {
            strcpy(nomeArq, buffer);
            strcpy(path, "");
        }
    }
    else
    {
        normalizePath(fullPath, path, lenPath);
        strcpy(nomeArq, "");
        strcpy(extArq, "");
    }

    free(buffer);
}

/*
   Dado um caminho completo (fullPath), copia em ext a extensao do arquivo.
   Se o arquivo nao tiver extensao, copia uma string vazia.

   fullPath           => ext
   ================================
   "aaa/bbb/ccc.txt"  => ".txt"
   "ccc.txt"          => ".txt"
   "aaa/bbb/ccc"      => ""
 */
void getExt(char *fullPath, char *ext, int lenExt)
{
    int i = strlen(fullPath) - 1;
    while (i >= 0 && fullPath[i] != '.')
        i--;
    if (i >= 0)
        strcpy(ext, fullPath + i);
    else
        strcpy(ext, "");
}
/*
   Dado um caminho completo (fullPath), copia em fileName o nome do arquivo 
   sem sua extensao. Exemplos:

   fullPath          => fileName
   ==============================
   "aaa/bb/cc.txt"   => "cc"
   "cc.txt"          => "cc"
   "cc."             => "cc"
   "cc"              => "cc"
   ".txt"            => ""
 */
void getFileNameWithoutExt(char *fullPath, char *fileName, int lenFileName)
{
    int i = strlen(fullPath) - 1;
    while (i >= 0 && fullPath[i] != '.')
        i--;
    if (i >= 0)
    {
        strncpy(fileName, fullPath, i);
        fileName[i] = '\0';
    }
    else
        strcpy(fileName, fullPath);
}