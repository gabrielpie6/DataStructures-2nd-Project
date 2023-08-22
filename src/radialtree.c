#include "radialtree.h"
#include "analyticgeometry.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct StRNode {
    bool state;
    Info data;
    double point[2];
    struct StRNode ** sons;
} RNode;

typedef struct StRTree {
    int n_sectors;
    double factor;
    int total_nodes_count;
    int removed_nodes_count;
    RNode * root;
} RTree;






RadialTree newRadialTree(int numSetores, double fd)
{
    RTree * tree = malloc(sizeof(RTree));
    tree->n_sectors = numSetores;
    tree->factor = fd;
    tree->removed_nodes_count = 0;
    tree->total_nodes_count   = 0;
    tree->root = NULL;
    return (RadialTree) tree;
}

Node createNode(double x, double y, Info i, int n_sectors)
{
    RNode * node = malloc(sizeof(RNode));
    node->state = true;
    node->data = i;
    node->point[0] = x;
    node->point[1] = y;
    node->sons = calloc(n_sectors, sizeof(RNode *));
}



Node insertRadialT(RadialTree t, double x, double y, Info i)
{
    RTree * tree = (RTree *) t;
    tree->total_nodes_count++;

    // Alocar nó
    RNode * node = (RNode *) createNode(x, y, i, tree->n_sectors);

    // Posionar nó na árvore
    int sector;
    RNode * aux = tree->root;

    if (aux == NULL) tree->root = node;
    else
    {
        // Verificar se há espaço para o nó, se não, continuar buscando um espaço livre
        sector = SegFunction(x - aux->point[0], y - aux->point[1], tree->n_sectors);
        while (aux->sons[sector] != NULL)
        {
            aux = aux->sons[sector];
            sector = SegFunction(x - aux->point[0], y - aux->point[1], tree->n_sectors);
        }
        aux->sons[sector] = node;
    }


    return (Node) node;
}




Info getInfoRadialT(RadialTree t, Node n)
{
    RNode * node = (RNode *) n;
    return node->data;
}

double * getPointRadialT(RadialTree t, Node n)
{
    RNode * node = (RNode *) n;
    return &node->point[0];
}

Node getRootNodeRadialT(RadialTree t)
{
    RTree * tree = (RTree *) t;
    return (Node) tree->root;
}

Node getSonNodeRadialT(RadialTree t, Node n, int i)
{
    RNode * node = (RNode *) n;
    return (Node) node->sons[i];
}

int getSectorsRadialT(RadialTree t)
{
    RTree * tree = (RTree *) t;
    return tree->n_sectors;
}
int getTotalNodesCountRadialT(RadialTree t)
{
    RTree * tree = (RTree *) t;
    return tree->total_nodes_count;
}
int getRemovedNodesCountRadialT(RadialTree t)
{
    RTree * tree = (RTree *) t;
    return tree->removed_nodes_count;
}



void RadialPreorder(RadialTree tree, Node relativenode, Function f, void * aux)
{
    if (relativenode == NULL) return;

    RTree * t    = (RTree *) tree;
    RNode * node = (RNode *) relativenode;

    if (node->state) f(tree, (Node) relativenode, aux);

    for (int i = 0; i < t->n_sectors; i++)
        if (node->sons[i] != NULL) RadialPreorder(tree, node->sons[i], f, aux);
}

// RadialTree GenerateBalancedRadialT(RadialTree oldtree); // Protótipo para a função complexa que está ao fim 
void removeNoRadialT(RadialTree t, Node n)
{
    RTree * tree = (RTree *) t;
    RNode * node = (RNode *) n;
    node->state  = false;
    tree->removed_nodes_count++;

    double fd = (double)(tree->removed_nodes_count / tree->total_nodes_count);
    if (fd >= tree->factor) // Atingiu limiar para balanceamento
    {
        RadialTree newtree = GenerateBalancedRadialT(t);
        // Desalocar RadialTree antiga
        // (...)
        RTree * oldt = (RTree *) t;
        RTree * newt = (RTree *) newtree;
        oldt->root   = newt->root;
    }
}

bool getNodeState(RadialTree t, Node n)
{
    RNode * node = (RNode *) n;
    return node->state;
}

void isNodeInsideRectangle (RadialTree tree, Node node, void * clausure)
{
    void ** infos = (void **) clausure;
    double * x1 = (double *) infos[0];
    double * y1 = (double *) infos[1];
    double * x2 = (double *) infos[2];
    double * y2 = (double *) infos[3];
    Lista     L = (Lista   ) infos[4];
    FdentroDeRegiao CmpFunction = (FdentroDeRegiao) infos[5];

    if (CmpFunction(getInfoRadialT(tree, node), *x1, *y1, *x2, *y2))
        insertLst(L, node);
}






void SearchNodeByRegion(RadialTree t, Node n, Lista L, double x, double y, double w, double h)
{
    RTree * tree = (RTree *) t;
    RNode * node = (RNode *) n;
    if (isPointInsideRectangle(node->point[0], node->point[1], x, y, x+w, y+h))
    {
        insertLst(L, (Item) n);
        for (int i = 0; i < tree->n_sectors; i++)
            SearchNodeByRegion(t, node->sons[i], L, x, y, w, h);
        return;
    }



    // Enviar a busca para as outras regiões viáveis:
    double Ox = node->point[0];
    double Oy = node->point[1];

    double points[][2] = {{x, y}, {x + w, y}, {x + w, y + h}, {x, y + h}};
    int size = tree->n_sectors;
    int greater = -1;
    int lesser  = size;
    int value;

    for (int i = 0; i < 4; i++)
    {
        value = SegFunction(points[i][0], points[i][1], size);
        if (value > greater) greater = value;
        else if (value < lesser) lesser = value;
    }

    if ((Oy < y && y < Oy + h) && (Ox < x)) // Caso problemático
    {
        int aux = greater;
        greater = lesser;
        lesser = aux;
    }
    for (int i = lesser; (i - 1) % size != greater; i++)
        SearchNodeByRegion(t, node->sons[i], L, x, y, w, h);
}

bool getInfosDentroRegiaoRadialT(RadialTree t, double x1, double y1, double x2, double y2,
				 FdentroDeRegiao f, Lista L)
{
    void * clausure[6];
    clausure[0] = (void *) &x1;
    clausure[1] = (void *) &y1;
    clausure[2] = (void *) &x2;
    clausure[3] = (void *) &y2;
    clausure[4] = (void *) L;
    clausure[5] = (void *) f;

    RadialPreorder(t, getRootNodeRadialT(t), isNodeInsideRectangle, (void *) clausure);
    return !isEmptyLst(L);
}


bool getNodesDentroRegiaoRadialT(RadialTree t, double x1, double y1, double x2, double y2, Lista L)
{
 
}












void RadialPreorder2(RadialTree tree, Node relativenode, FindFunction f, void * aux, int * found, Node * TargetNode)
{
    if (relativenode == NULL || *found == 1) return;

    if (f(relativenode, aux))
    {
        *found = 1;
        *TargetNode = relativenode;
        return;
    }

    RTree * t    = (RTree *) tree;
    RNode * node = (RNode *) relativenode;
    for (int i = 0; i < t->n_sectors; i++)
        if (node->sons[i] != NULL) RadialPreorder2(tree, node->sons[i], f, aux, found, TargetNode);
}

Node buscaNodePorFuncao(RadialTree tree, FindFunction f, void * aux)
{
    RTree * t    = (RTree *) tree;
    RNode * node = (RNode *) t->root;
    RNode * TargetNode = NULL;

    int found = 0;
    RadialPreorder2(tree, (Node) node, f, aux, &found, (Node *) &TargetNode);
    return (Node) TargetNode;
}




void RadialPreOrder(RadialTree tree, Node relativenode, FvisitaNo f, void * aux)
{
    if (relativenode == NULL) return;

    RTree * t    = (RTree *) tree;
    RNode * node = (RNode *) relativenode;
    Info    data = node->data;

    if (node->state) f(data, node->point[0], node->point[1], aux);

    for (int i = 0; i < t->n_sectors; i++)
        if (node->sons[i] != NULL) RadialPreOrder(tree, node->sons[i], f, aux);
}
void visitaProfundidadeRadialT(RadialTree t, FvisitaNo f, void *aux)
{
    RTree * tree = (RTree *) t;
    RadialPreOrder(t, (Node) tree->root, f, aux);
}












struct NodeLength {
    Info data;
    double point[2];
    double length;
};


void InsertActiveNodesLength(Info i, double x, double y, void * aux)
{
    void ** clausure = (void **) aux;
    struct NodeLength  * ActiveNodes = (struct NodeLength *) clausure[0];
    int                * index       = (int               *) clausure[1];
    double             * Sx          = (double            *) clausure[2];
    double             * Sy          = (double            *) clausure[3];

    (*Sx) += x;
    (*Sy) += y;

    ActiveNodes[*index].data     = i;
    ActiveNodes[*index].point[0] = x;
    ActiveNodes[*index].point[1] = y;
    (*index)++;
}

int compareNodeLength(const void * p, const void * q)
{
    struct NodeLength * a = (struct NodeLength *) p;
    struct NodeLength * b = (struct NodeLength *) q;
    return (a->length - b->length);
}

RadialTree GenerateBalancedRadialT(RadialTree oldtree)
{
    // Fazer bounding-box dox elementos
    RTree * oldt = (RTree *) oldtree;
    RNode * root = oldt->root;

    int TotalEnableNodes = oldt->total_nodes_count - oldt->removed_nodes_count;
    struct NodeLength ActiveNodes[TotalEnableNodes];
    

    
    double Sx = 0;
    double Sy = 0;
    int index = 0;
    void * InsertSumClausure[] = {(void *) &ActiveNodes[0], &index, (void *) &Sx, (void *) &Sy};
    // Inserir no vetor e obter o ponto-médio
    // (...)
    RadialPreOrder(oldtree, (Node) root, InsertActiveNodesLength, InsertSumClausure);
    double medX = Sx / TotalEnableNodes;
    double medY = Sy / TotalEnableNodes;


    // Percorrer novamente o vetor agora calculando a distância dos nós até o ponto-médio
    for (int i = 0; i < TotalEnableNodes; i++)
        ActiveNodes[i].length = distance(medX, medY, ActiveNodes[i].point[0], ActiveNodes[i].point[1]);


    // Ordenar pela menor menor distancia até o ponto-médio
    qsort(ActiveNodes, TotalEnableNodes, sizeof(struct NodeLength), compareNodeLength);

    // Inserir os nós em uma nova árvore na ordem dada por qsort
    RadialTree newtree = newRadialTree(oldt->n_sectors, oldt->factor);
    for (int i = 0; i < TotalEnableNodes; i++) 
        insertRadialT(newtree, ActiveNodes[i].point[0], ActiveNodes[i].point[1], ActiveNodes[i].data);
    return newtree;
}