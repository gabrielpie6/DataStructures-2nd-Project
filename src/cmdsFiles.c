#include <stdio.h>
#include "cmdsFiles.h"

ArqCmds abreArquivoCmd(char *fn)
{
    FILE * file = fopen(fn, "r");
    return file;
}

// PROBLEMA: conteudo maior que bufLen
bool leLinha (ArqCmds ac, char *buf, int bufLen)
{
    if (fscanf(ac, "%[^\n]\n", buf) == EOF)
        return false;
    else
        return true;
}

// PROBLEMA: parametro maior que paramILen
// PROBLEMA: índice de parâmetro não existir (i > total de índices - 1)
// PROBLEMA: índice negativo
void getParametroI(ArqCmds ac, char *buf, int i, char *paramI, int paramILen)
{
    int k = -1, amount = 0;
    
    if (i > 0)
    {
        while (amount != i)
        {
            amount++;
            k++;
            while (buf[k] != ' ')
                k++;
        }
    }
    sscanf(buf + k + 1, "%s", paramI);
}

// PROBLEMA: parametro maior que paramILen
// PROBLEMA: índice de parâmetro não existir (i > total de índices - 1)
// PROBLEMA: índice negativo
void getParametroDepoisI(ArqCmds ac, char *buf, int i, char *paramI, int paramILen)
{
    /*
    int k;
    
    sscanf(buf, "%s %[^\0]", paramI, paramI);
    if (i > 0)
        for (k = 1; k <= i; k++)
            sscanf(paramI, "%s %[^\0]", paramI, paramI);
    */

    int k = -1, amount = 0;
        
    if (i > 0)
        while (amount != i)
        {
            amount++;
            k++;
            while (buf[k] != ' ')
                k++;
        }
    sprintf(paramI, "%s", buf + k + 1);
}

void fechaArquivoCmd(ArqCmds ac)
{
    fclose(ac);
}