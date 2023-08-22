#ifndef _RADIAL_TREE__
#define _RADIAL_TREE__

#include "lista.h"
#include <stdbool.h>

/*
 * Uma Arvore Radial e' uma arvore n-aria, espacial, nao balanceada.
 * A cada no' r da Arvore e' associado um ponto de 2 dimensoes (r.x e r.y) denominado ancora
 * do no' e uma informacao relacionada tal ponto.
 * Um no' divide o plano simetricamente em n regioes (0..n-1), tambem denomindadas setores,
 * centradas no ponto (r.x e r.y),  cada regiao associada a sua respectiva subarvore.
 * Cada setor Si e' delimitado por duas semi-retas, Rinf e Rsup, com origem na ancora e
 * com inclinacao, respectivamente:
 *
 *      InclinacaoRetaInf = i * 360/n
 *      InclinacaoRetaSup = (i + 1) * 360/n
 *
 * e compreende a regiao `a esquerda de Rinf e `a direita de Rsup, orientadas da ancora
 * para infinito.
 *
 * A arvore Radial implementa 2 percursos: em largura e em profundidade.
 * Tais percursos recebem como parametro uma funcao que processa a
 * informacao associada ao no' visitado.
 *
 * Tambem sao providas operacoes de selecao espacial:
 *   A PRIMEIRA seleciona os nos da arvore cujas ancoras
 * sao internas a uma regiao retangular.
 *   A SEGUNDA seleciona os nos da Arvore cujas informacoes associadas
 * podem ser consideradas internas a uma regiao retangular.
 * Esta operacao recebe por parametro uma funcao que decide se
 * tal informacao e' considerada (ou nao) interna.
 *   A TERCEIRA, similarmente, seleciona os nos da arvore cujo um dado
 * ponto e' interno 'a informacao associada ao no'.
 *
 * Nos removidos nao sao desalocados imediatamente, mas apenas
 * marcados como delidos.
 * Eventualmente, uma sub-arvore e' reorganizada sem os nos marcados como removidos.
 * Isto ocorre, imediatamente apos o no ser marcado como removido,  quando o fator de 
 * degradacao fd da sub-arvore exceder a um dado limiar definido na criacao da arvore.
 *
 *     fd = #removidos/#total_nos > limiar.
 *
 */

typedef void *RadialTree;
typedef void *Node;
typedef void *Info;

typedef void (*Function)(RadialTree tree, Node node, void *aux);

typedef bool (*FindFunction)(Node node, void *aux);
/*
  * Uma funcao deste tipo deve processar a informacao associada ao no' node e definir
  * se o no' é desejado. Retorna true (1) para DESEJADO e false (0) caso contra'rio.
*/

typedef bool (*FdentroDeRegiao)(Info i, double x1, double y1, double x2, double y2);
/*
 * Uma funcao deste tipo deve retornar verdadeiro se a informacao i esta'
 * "dentro" da regiao retangular delimitada pelos pontos opostos (x1,y1) e (x2,y2).
 * Retorna falso, caso contrario.
 */

typedef bool (*FpontoInternoAInfo)(Info i, double x, double y);
/*
 * Uma funcao deste tipo deve retornar verdadeiro se o ponto (x,y)
 * deva ser considerado
 * "interno" da informacao i.
 */

typedef void (*FvisitaNo)(Info i, double x, double y, void *aux);
/*
 * Processa a informacao i associada a um no' da arvore, cuja ancora
 * e' o ponto (x,y). O parametro aux aponta para conjunto de dados
 * (provavelmente um registro) que sao compartilhados entre as
 * sucessivas invocacoes a esta funcao.
 */

RadialTree newRadialTree(int numSetores, double fd);
/*
 * Retorna uma arvore Radial vazia de numSetores setores e com fator 
 * de degradacao fd.
 *    0 <= fd < 1.0
 */

Node insertRadialT(RadialTree t, double x, double y, Info i);
/*
 * Insere a informacao i, associada 'a ancora (x,y) na arvore t.
 * Retorna um indicador para o no' inserido.
 */

Node getNodeRadialT(RadialTree t, double x, double y, double epsilon);
/*
 * Retorna o no' cuja ancora seja o ponto (x,y). Aceita-se uma pequena discrepancia
 * entre a coordenada da ancora (anc.x,anc.y) e o ponto (x,y) de epsilon unidades.
 * Ou seja, |anc.x - x | < epsilon e |anc.y - y | < epsilon.
 * Retorna NULL caso nao tenha encontrado o no'.
 */

Node getRootNodeRadialT(RadialTree t);
/* Retorna o no' considerado raiz da a'rvore radial */

Node getSonNodeRadialT(RadialTree t, Node n, int i);
/* Retorna o i-esimo filho do no' n, i >= 0 */

void removeNoRadialT(RadialTree t, Node n);
/*
 * Marca como removido o no' n. Caso, apos a remocao, o fator de degradacao
 * superar o limiar definido na criacao, a arvore e' recriada sem os nos delidos.
 */

bool getNodeState(RadialTree t, Node n);
/* Retorna verdadeiro se o no' n esta' ativo na arvore, falso caso contrario */

Info getInfoRadialT(RadialTree t, Node n);
/* Retorna a informacao associada ao no' n */

double * getPointRadialT(RadialTree t, Node n);
/* Retorna a coordenada (x, y) associada ao no' n */

int getSectorsRadialT(RadialTree t);
/* Retorna o numero de setores angulares da arvore t */

bool getNodesDentroRegiaoRadialT(RadialTree t, double x1, double y1, double x2, double y2, Lista L);
/* Insere na lista L os nos (Node) da arvore t cujas ancoras estao dentro da regiao delimitada pelos
   pontos (x1,y1) e (x2,y2).
   Retorna falso, caso nao existam nos dentro da regiao; verdadeiro, caso contrario.
 */

bool getInfosDentroRegiaoRadialT(RadialTree t, double x1, double y1, double x2, double y2,
				 FdentroDeRegiao f, Lista L);
/* Insere na lista L os nos cujas respectivas informacoes associadas estao dentro da regiao
   delimitada pelos pontos (x1,y1) e (x2,y2). A funcao f e' usada para determinar se uma informacao
   armazenada na arvore esta' dentro da regiao.
   Retorna falso caso nao existam informacoes internas; verdadeiro, caso contrario.
 */

bool getInfosAtingidoPontoRadialT(RadialTree t, double x, double y, FpontoInternoAInfo f, Lista L);
/* Insere na lista L  os nos para os quais o ponto (x,y) possa ser considerado
  interno 'as  informacoes associadas ao no'. A funcao f e' invocada para determinar
  se o ponto (x,y) e' interno a uma informacao especifica.
  Retorna falso caso nao existam informacoes internas; verdadeiro, caso contrario.
 */

void visitaProfundidadeRadialT(RadialTree t, FvisitaNo f, void *aux);
/* Percorre a arvore em profundidade. Invoca a funcao f em cada no visitado.
   O apontador aux e' parametro em cada invocacao de f; assim alguns
   dados podem ser compartilhados entre as diversas invocacoes de f.
 */

void visitaLarguraRadialT(RadialTree t, FvisitaNo f, void *aux);
/* Similar a visitaProfundidadeRadialT, porem, faz o percurso em largura.
 */






/* FUNÇÕES ADICIONAIS */
void RadialPreorder(RadialTree tree, Node relativenode, Function f, void * aux);

/*
 Possui princípio parecido com a função RadialPreorder: percorre a árvore em pré-ordem 
 aplicando a função (FindFunction) f em cada (Node) nó percorrido, contudo, com base no
 valor booleano retornado por f é definido se a busca deve continuar ou não. Se f retornar
 true (1), então significa que o nó foi achado e a busca é interrompida, retornando o nó,
 enquanto que se f retornar false (0), a busca deve continuar.
 */
Node buscaNodePorFuncao(RadialTree tree, FindFunction f, void * aux);

int getTotalNodesCountRadialT(RadialTree t);
int getRemovedNodesCountRadialT(RadialTree t);

RadialTree GenerateBalancedRadialT(RadialTree oldtree);
#endif
