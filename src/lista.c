#include "lista.h"

#include <stdio.h>
#include <stdlib.h>

/////////////////////////////////////////////
// Implementação das Estruturas
//
struct StNode{
  struct StNode *prox, *ant;
  Item info;
};

struct StLista{
  struct StNode *prim, *ult;
  int capac;
  int length;
};

typedef struct StLista ListaImpl;
typedef struct StNode Node;

struct StIterator{
  Node *curr;
  bool reverso;
};
//
typedef struct StIterator IteratorImpl;
/////////////////////////////////////////////

Lista createLst(int capacidade)
{
    ListaImpl *lst = (ListaImpl *) malloc(sizeof(ListaImpl));
    lst->prim = NULL;
    lst->ult = NULL;
    lst->capac = capacidade;
    lst->length = 0;
    return lst;
}

int lengthLst(Lista L)
{
    ListaImpl *lst = (ListaImpl *) L;
    return lst->length;
}

int maxLengthLst(Lista L)
{
    ListaImpl *lst = (ListaImpl *) L;
    if (lst->capac < 0)
        return -1;
    else
        return lst->capac;
}

bool isEmptyLst(Lista L)
{
    ListaImpl *lst = (ListaImpl *) L;
    return lst->length == 0;
}

bool isFullLst(Lista L)
{
    ListaImpl *lst = (ListaImpl *) L;
    return lst->length == lst->capac;
}

Posic insertLst(Lista L, Item info)
{    
    ListaImpl *lst = ( ListaImpl *)L;
    Node *newNode = (Node *) malloc (sizeof(Node));
    newNode->info = info;
    newNode->prox = NULL;
    
    if (isEmptyLst(L))
        lst->prim = newNode;
    else
        lst->ult->prox = newNode;
        
    newNode->ant = lst->ult;
    lst->ult = newNode;
    lst->length++;
    return newNode;
}

Item popLst(Lista L)
{
    ListaImpl *lst = (ListaImpl *) L;

    if (isEmptyLst(L))
    {
        printf("ERRO: popLst em lista vazia\n");
        return NULL;
    }
    else
    {
        Node *node = lst->prim;
        Item info = node->info;
        removeLst(L, node);

        return info;
    }
}

void removeLst(Lista L, Posic p)
{
    ListaImpl *lst = (ListaImpl *) L;
    Node *node = (Node *) p;
    if (node != NULL)
    {
        if (node->ant != NULL)
            node->ant->prox = node->prox;
        else
            lst->prim = node->prox;

        if (node->prox != NULL)
            node->prox->ant = node->ant;
        else
            lst->ult = node->ant;
        // Cuidado ao desalocar item do tipo Item (ainda desconhecido)
        //free(node->info);
        free(node);
        lst->length--;
    }
    else
        printf("ERRO: removeLst com posicao invalida\n");
}

Item getLst(Lista L, Posic p)
{
    Node *node = (Node *) p;
    if (node != NULL)
        return node->info;
    else
    {
        printf("ERRO: getLst com posicao invalida\n");
        return NULL;
    }
}

Posic insertBeforeLst(Lista L, Posic p, Item info)
{
    ListaImpl *lst = (ListaImpl *) L;
    Node *node = (Node *) p;

    Node *newNode = (Node *) malloc (sizeof(Node));
    newNode->info = info;
    newNode->prox = node;
    newNode->ant = node->ant;
    if (node->ant == NULL)
        lst->prim = newNode;
    else
        node->ant->prox = newNode;
    node->ant = newNode;
    lst->length++;
    return newNode;
}

Posic insertAfterLst(Lista L, Posic p, Item info)
{
    ListaImpl *lst = (ListaImpl *) L;
    Node *node = (Node *) p;

    Node *newNode = (Node *) malloc (sizeof(Node));
    newNode->info = info;
    newNode->ant = node;
    newNode->prox = node->prox;
    if (node->prox == NULL)
        lst->ult = newNode;
    else
        node->prox->ant = newNode;
    node->prox = newNode;
    lst->length++;
    return newNode;
}

Posic getFirstLst(Lista L)
{
    ListaImpl *lst = (ListaImpl *) L;
    return lst->prim;
}

Posic getNextLst(Lista L, Posic p)
{
    ListaImpl *lst = (ListaImpl *) L;
    Node *node = (Node *) p;
    if (lst->ult == node)
        return NIL
    else
        return node->prox;
}

Posic getLastLst(Lista L)
{
    ListaImpl *lst = (ListaImpl *) L;
    if (lengthLst(lst) == 0)
        return NIL
    else
        return lst->ult;
}

Posic getPrevLst(Lista L, Posic p)
{
    ListaImpl *lst = (ListaImpl *) L;
    Node *node = (Node *) p;
    if (lst->prim == node)
        return NIL
    else
        return node->ant;
}

void killLst(Lista L)
{
    ListaImpl *lst = (ListaImpl *) L;
    if (lst != NULL && !isEmptyLst(lst))
    {
        for (Node * node = lst->prim->prox; node != NULL; node = node->prox)
        {
            //free(node->ant->info);
            free(node->ant);
        }
        //free(lst->ult->info);
        free(lst->ult);
        free(lst);
    }
}


/**
 ** Iteradores
 **
 ** (nota: existe uma certa redundancia com getFirst, getLast, 
 **  getNext e getPrevious).
 **/
Iterador createIterador(Lista L, bool reverso)
{
    ListaImpl *lst = (ListaImpl *) L;
    IteratorImpl *iter = (IteratorImpl *) malloc (sizeof(IteratorImpl));
    //iter->curr = lst->prim;
    iter->reverso = reverso;
    return iter;
}

void setIteratorPosition(Lista L, Iterador it, Posic p)
{
    ListaImpl *lst = (ListaImpl *) L;
    IteratorImpl *iter = (IteratorImpl *) it;
    iter->curr = (Node *) p;
}

bool isIteratorEmpty(Lista L, Iterador it)
{
    ListaImpl *lst = (ListaImpl *) L;
    IteratorImpl *iter = (IteratorImpl *) it;
    // DEVERIA verificar se o iterador aponta para ALGUM elemento da lista
    return iter->curr == NULL;
}

Iterador getIteratorNext(Lista L, Iterador it)
{
    ListaImpl *lst = (ListaImpl *) L;
    IteratorImpl *iter = (IteratorImpl *) it;

    if (isIteratorEmpty(L, it))
        return NULL;
    else
        if (iter->reverso == false)
            iter->curr = iter->curr->prox;
        else
            iter->curr = iter->curr->ant;
    return iter;
}

Item getIteratorItem(Lista L, Iterador it)
{
    ListaImpl *lst = (ListaImpl *) L;
    IteratorImpl *iter = (IteratorImpl *) it;
    return iter->curr->info;
}

Posic getIteratorPosic(Lista L, Iterador it)
{
    ListaImpl *lst = (ListaImpl *) L;
    IteratorImpl *iter = (IteratorImpl *) it;
    return iter->curr;
}

void killIterator(Lista L, Iterador it)
{
    IteratorImpl *iter = (IteratorImpl *) it;
    free(iter);
}





/**
 ** High-order functions
 **/
typedef Item (*Apply)(Item item);
typedef bool (*Check)(Item item);
typedef void (*ApplyClosure)(Item item, Clausura c);

Lista map(Lista L, Apply f)
{
    ListaImpl *lst = (ListaImpl *) L;
    ListaImpl *newLst = (ListaImpl *) createLst(lst->capac);

    for (Node * node = lst->prim; node != NULL; node = node->prox)
        insertLst(newLst, f(node->info));
    return newLst;
}

Lista filter(Lista L, Check f)
{
    ListaImpl *lst = (ListaImpl *) L;
    ListaImpl *newLst = (ListaImpl *) createLst(lst->capac);

    for (Node * node = lst->prim; node != NULL; node = node->prox)
        if (f(node->info))
            insertLst(newLst, node->info);
    return newLst;
}

void fold(Lista L, ApplyClosure f, Clausura c)
{
    ListaImpl *lst = (ListaImpl *) L;
    for (Node * node = lst->prim; node != NULL; node = node->prox)
        f(node->info, c);
}

Lista filterClausure(Lista L, CheckClausure f, Clausura c)
{
    ListaImpl *lst = (ListaImpl *) L;
    ListaImpl *newLst = (ListaImpl *) createLst(lst->capac);

    for (Node * node = lst->prim; node != NULL; node = node->prox)
        if (f(node->info, c))
            insertLst(newLst, node->info);
    return newLst;
}