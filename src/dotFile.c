#include "dotFile.h"

#include <stdio.h>


void printNodeEnt(RadialTree tree, Node node, void * aux)
{
    void ** clausure       = (void **) aux;
    FILE * dot             = (FILE * ) clausure[0];
    int  * removedIDsCount = (int  * ) clausure[1];

    Entity entFather = getInfoRadialT(tree, node);
    Node nodeSon;
    int idFather = getEntId(entFather);
    int id;




    // Busca por outros removidos
    for (int i = 0; i < getSectorsRadialT(tree); i++)
    {
        nodeSon = getSonNodeRadialT(tree, node, i);
        if (nodeSon != NULL && !getNodeState(tree, nodeSon))
            printNodeEnt(tree, nodeSon, aux);
    }

    if (!getNodeState(tree, node))
    {
        idFather = *removedIDsCount - idFather;
        fprintf(dot, "\t%d [style=dotted color=red label=\"%d\"];\n", idFather, *removedIDsCount - idFather);
    }
    fprintf(dot, "\t%d -> { ", idFather);
    for (int i = 0; i < getSectorsRadialT(tree); i++)
    {
        nodeSon = getSonNodeRadialT(tree, node, i);
        if (nodeSon != NULL)
        {
            id = getEntId(getInfoRadialT(tree, nodeSon));
            if (!getNodeState(tree, nodeSon)) id = *removedIDsCount - id;
            fprintf(dot, "%d ", id);
        }
    }
    fprintf(dot, "};\n");
}

void DrawTree(RadialTree tree, char * fullfilename)
{
    FILE * dot        = fopen(fullfilename, "w");
    int    removedsID = -1000;
    void * aux[]      = {dot, &removedsID};

    if (dot == NULL)
    {
        printf("ERRO: [in DrawTree]: nao foi possivel abrir arquivo .dot\n");
        return;
    }
    
    
    fprintf(dot, "digraph BST\n");
    fprintf(dot, "{\n");
    fprintf(dot, "\tnode [fontname=\"Arial\"];\n");

    if (!getNodeState(tree, getRootNodeRadialT(tree))) printNodeEnt(tree, getRootNodeRadialT(tree), (void *) aux);

    RadialPreorder(tree, getRootNodeRadialT(tree), printNodeEnt, (void *) aux);

    fprintf(dot, "}");
    fclose(dot);
}