/* EXERCICIO: escrever a descricao do modulo e das operacoes */
// arquivo organizado em linhas: // aaaa bbb cccc dddd eeee

#ifndef READ_COMMANDS_FILES_H
#define READ_COMMANDS_FILES_H

#include <stdbool.h>

/*
    Módulo destinado a ler e operar comandos e parâmetros de um arquivo de texto organizado em linhas
    Denomina-se este arquivo de "arquivo de comandos" (ArqCmds).
*/

typedef void *ArqCmds;

/// @brief  Abre um arquivo de texto com comandos e parâmetros separados em linhas
///         -- Exemplo.:
///         () 10.0 20.0  5.0 blue
///         [] 5.0 6.0 8.000 9.000 yellow green
///         @ 30.00 31.00 normal acorda pedrinho
/// @result tipo ArqCmds se abertura for bem sucedida, caso contrário NULL 
ArqCmds abreArquivoCmd(char *fn);


/// @brief  Lê uma linha do arquivo de texto com comandos fornecido e armazena em um
///         vetor de caracteres fornecido previamente
/// @param ac arquivo de texto com comandos
/// @param buf vetor de caracteres (string)
/// @param bufLen tamanho do vetor de caracteres
/// @return true se operação bem-sucedida, false caso contrário 
bool leLinha (ArqCmds ac, char *buf, int bufLen);
// aaaa bbbb ccccc dddd eeee


/// @brief Obtém o i-ésimo parâmetro do buffer delimitado por espaços em branco
/// @param ac arquivo do tipo ArqCmds
/// @param buf vetor de caracteres contendo todos os parâmetros possíveis
/// @param i índice do parâmetro requisitado (i >= 0)
/// @param paramI buffer onde o i-ésimo parâmetro será alocado
/// @param paramILen tamanho do vetor de caracteres paramI
void getParametroI(ArqCmds ac, char *buf, int i, char *paramI, int paramILen);
// i==2: ccccc


/// @brief Obtém todos os parâmetros de índices <= i do buffer delimitado por espaços em branco
/// @param ac arquivo do tipo ArqCmds
/// @param buf vetor de caracteres contendo todos os parâmetros possíveis
/// @param i índice do parâmetro requisitado (i >= 0)
/// @param paramI buffer onde o i-ésimo parâmetro será alocado
/// @param paramILen tamanho do vetor de caracteres paramI
void getParametroDepoisI(ArqCmds ac, char *buf, int i, char *paramI, int paramILen);
// i==2: ccccc dddd eeee


/// @brief Realiza o fechamento do arquivo ac
/// @param ac arquivo do timpo ArqCmds
void fechaArquivoCmd(ArqCmds ac);

#endif