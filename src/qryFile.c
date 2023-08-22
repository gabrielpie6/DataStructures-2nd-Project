#include "qryFile.h"
#include "actions.h"
#include "analyticgeometry.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>












int ReadQryFile (RadialTree Elements, char * qryPath, char * outputPath, char * geo_qryCombination, Style style, Lista Decos)
{
    ArqCmds QryFile = abreArquivoCmd(qryPath);
    if (QryFile == NULL)
    {
        printf("ERRO: nao foi possivel abrir arquivo '%s'\n", qryPath);
        return false;
    }
    else
    {
        char buffer[DEFAULT_BUFFER_SIZE];
        char parameter[SIMPLE_PARAMETER_SIZE];
        char * txtFile = malloc(sizeof(char) * (strlen(outputPath) + 1 + strlen(geo_qryCombination) + strlen(".txt") + 1));
        sprintf(txtFile, "%s/%s.txt", outputPath, geo_qryCombination);
        FILE * TXTFile = fopen(txtFile, "w");

        while (leLinha(QryFile, buffer, DEFAULT_BUFFER_SIZE))
        {
            getParametroI(QryFile, buffer, 0, parameter, SIMPLE_PARAMETER_SIZE);

            if (strcmp(parameter, "cl" ) == 0) DefineHarvester   (QryFile, Elements, buffer, TXTFile, Decos); else
            if (strcmp(parameter, "hvt") == 0) HarvestRegion     (QryFile, Elements, buffer, TXTFile, Decos); else
            if (strcmp(parameter, "mv" ) == 0) moveEntity        (QryFile, Elements, buffer, TXTFile, Decos); else
            if (strcmp(parameter, "ct" ) == 0) ContaminateRegion (QryFile, Elements, buffer, TXTFile, Decos, style); else
            if (strcmp(parameter, "cr" ) == 0) HealRegion        (QryFile, Elements, buffer, TXTFile, Decos); else
            if (strcmp(parameter, "ad" ) == 0) FertilizeRegion   (QryFile, Elements, buffer, TXTFile, Decos); else
            if (strcmp(parameter, "st" ) == 0) SpreadRegion      (QryFile, Elements, buffer, TXTFile, Decos); else
            if (strcmp(parameter, "d?" ) == 0) ReportAttributtes (QryFile, Elements, buffer, TXTFile, Decos); else
            if (strcmp(parameter, "c?" ) == 0) ReportHarvesters  (QryFile, Elements, buffer, TXTFile, Decos); else
            {
                printf("ERRO: [in ReadQryFile]: comando '%s' nao reconhecido em '%s'\n", parameter, qryPath);
                fechaArquivoCmd(QryFile);
                fclose(TXTFile);
                return 1; // 1 = Problema
            }
        }
        fclose(TXTFile);
    }
    fechaArquivoCmd(QryFile);
    return 0; // 0 = Sucesso
}




void moveEntInRadialT(RadialTree tree, Node n, double dx, double dy)
{
    Entity   ent = getInfoRadialT(tree, n);
    Geometry geo = getEntGeo(ent);
    Dislocate_Geo(geo, dx, dy);

    double X = getGeoCords(geo)[0];
    double Y = getGeoCords(geo)[1];
    if (getGeoClass(geo) == 'l')
    {
        X = getGeoAnchor_1(geo)[0];
        Y = getGeoAnchor_1(geo)[1];
    }
    if (getEntClass(ent) == 'h') return;

    Node newNode = insertRadialT(tree, X, Y, (Info) ent);

    // Remoção do nó da árvore
    removeNoRadialT(tree, n);
}


void DefineHarvester (ArqCmds QryFile, RadialTree Elements, char * lineBuffer, FILE * TXTFile, Lista Decos)
{
    char parameter[SIMPLE_PARAMETER_SIZE];

    getParametroI(QryFile, lineBuffer, 1, parameter, SIMPLE_PARAMETER_SIZE);
    int id = atoi(parameter);

    Node target = searchNodeByEntID(Elements, id);
    if (target == NULL)
    {
        printf("AVISO: [in DefineHarvester]: figura de id '%d' nao encontrada no Banco de Dados.\n", id);
        printf("        -> comando 'cl' ignorado.\n");
        return;
    }
    else if (getEntClass(getInfoRadialT(Elements, target)) == 'v') ConvertVegToHarvester(getInfoRadialT(Elements, target));
}
void HarvestRegion (ArqCmds QryFile, RadialTree Elements, char * lineBuffer, FILE * TXTFile, Lista Decos)
{
    char parameter[SIMPLE_PARAMETER_SIZE];

    getParametroI(QryFile, lineBuffer, 1, parameter, SIMPLE_PARAMETER_SIZE);
    int id = atoi(parameter);
    getParametroI(QryFile, lineBuffer, 2, parameter, SIMPLE_PARAMETER_SIZE);
    int steps = atoi(parameter);
    getParametroI(QryFile, lineBuffer, 3, parameter, SIMPLE_PARAMETER_SIZE);
    char direction = parameter[0];


    Node node = searchNodeByEntID(Elements, id);
    if (node == NULL)
    {
        printf("AVISO: [in HarvestRegion]: figura '%d' nao e' uma colheitadeira.\n", id);
        printf("        -> comando 'hvt' ignorado.\n");
        return;
    }

    fprintf(TXTFile, "\n[*] %s\n", lineBuffer);
    fprintf(TXTFile, "\tFIGURA(S) COLHIDA(S):\n");
    Entity harvester = (Entity) getInfoRadialT(Elements, node);
    double colheita = DoHarvest(Elements, harvester, steps, direction, TXTFile, Decos);


    Geometry rectangle = getEntGeo(harvester);
    double w = getGeoWidth (rectangle);
    double h = getGeoHeight(rectangle);
    double X, Y;
    switch(direction)
    {
        case 'n':
        {
            X = 0;
            Y = h;
            break;
        }
        case 's':
        {
            X = 0;
            Y = -h;
            break;
        }
        case 'l':
        {
            X = -w;
            Y = 0;
            break;
        }
        case 'o':
        {
            X = w;
            Y = 0;
            break;
        }
    }

    moveEntInRadialT(Elements, node, X * (steps - 1), Y * (steps - 1));
    fprintf(TXTFile, "\t[CONTABILIDADE DA COLHEITA]: %.2lf\n", colheita);
}
















void moveEntity (ArqCmds QryFile, RadialTree Elements, char * lineBuffer, FILE * TXTFile, Lista Decos)
{
    char parameter[SIMPLE_PARAMETER_SIZE];

    getParametroI(QryFile, lineBuffer, 1, parameter, SIMPLE_PARAMETER_SIZE);
    int id = atoi(parameter);

    getParametroI(QryFile, lineBuffer, 2, parameter, SIMPLE_PARAMETER_SIZE);
    double dx = atof(parameter);
    getParametroI(QryFile, lineBuffer, 3, parameter, SIMPLE_PARAMETER_SIZE);
    double dy = atof(parameter);

    /* PROBLEMA: Ao deslocar uma forma de posiçao, implica que a posição desta também deve ser atualizada
                 na posição armazena na árvore espacial radial, pois ambas compartilham uma mesma coordenada
                 espacial no plano. Desta forma, como SABEMOS que sua posição deve ser atualizada para manter
                 o funcionamento correto das funções provenientes de radialtree.h, pode-se dar como solução:
                    - Retirar o nó da árvore, atualizar sua posição, e inseri-lo novamente na árvore. Assim
                      garantindo o bom funcionamento da estrutura.
    */
   Node target  = searchNodeByEntID(Elements, id);
   if (target == NULL)
   {
        printf("AVISO: [in moveEntity]: tentou movimentar figura '%d' inexistente no Banco de Dados\n", id);
        printf("        -> comando '%s' ignorado\n", lineBuffer);
        return;
   }
   Entity ent   = getInfoRadialT(Elements, target);
   Geometry geo = getEntGeo(ent);



   fprintf(TXTFile, "\n[*] %s\n", lineBuffer);
   fprintf(TXTFile, "\t[%d] ", id);
   switch (getEntClass(ent))
   {
        case 'v':
        {
            fprintf(TXTFile, "%s - Pos. ORIGINAL (%.2lf, %.2lf) Pos. FINAL (%.2lf, %.2lf)\n", strVegSpecies(ent),
                getGeoCords(geo)[0]     , getGeoCords(geo)[1],
                getGeoCords(geo)[0] + dx, getGeoCords(geo)[1] + dy 
            );
            break;
        }
        case 'h':
        {
            fprintf(TXTFile, "colheitadeira - Pos. ORIGINAL (%.2lf, %.2lf) Pos. FINAL (%.2lf, %.2lf)\n",
                getGeoCords(geo)[0]     , getGeoCords(geo)[1],
                getGeoCords(geo)[0] + dx, getGeoCords(geo)[1] + dy 
            );
            break;
        }
        case 'm':
        {
            switch (getGeoClass(geo))
            {
                case 'l':
                {
                    fprintf(TXTFile, "linha - Pos. ANCORAS ORIGINAL {(%.2lf, %.2lf), (%.2lf, %.2lf)}\n",
                        getGeoAnchor_1(geo)[0]     , getGeoAnchor_1(geo)[1],
                        getGeoAnchor_2(geo)[0]     , getGeoAnchor_2(geo)[1]
                    );
                    fprintf(TXTFile, "        Pos. ANCORAS    FINAL {(%.2lf, %.2lf), (%.2lf, %.2lf)}\n",
                        getGeoAnchor_1(geo)[0] + dx, getGeoAnchor_1(geo)[1] + dy,
                        getGeoAnchor_2(geo)[0] + dx, getGeoAnchor_2(geo)[1] + dy
                    );
                    break;
                }
                case 't':
                {
                    fprintf(TXTFile, "texto (não-hortalica) - Pos. ORIGINAL (%.2lf, %.2lf) Pos. FINAL (%.2lf, %.2lf)\n",
                        getGeoCords(geo)[0]     , getGeoCords(geo)[1],
                        getGeoCords(geo)[0] + dx, getGeoCords(geo)[1] + dy 
                    );
                    break;
                }
                default:
                {
                    printf("FORMATO DESCONHECIDO\n");
                    break;
                }
            }
        }
   }

   moveEntInRadialT(Elements, target, dx, dy);
}


// Função abstrata que aplica o efeito específico sob a figura (hortaliça) que foi atingida por uma gotícula.
typedef void (*EffectFunction)(RadialTree Elements, Node figure, double DropArea, void * aux);
// Função que implementa a dispersão uniforme de gotículas circulares de dado raio dentro de uma região retangular fornecida.
void DistributeDrops(RadialTree Elements, EffectFunction f, double x, double y, double width, double height, double radius, Lista Decos, void * aux)
{
    Lista Figures = createLst(-1);
    
    if(getInfosDentroRegiaoRadialT(Elements, x, y, x + width, y + height, (FdentroDeRegiao) isEntInsideRectangle, Figures));
    {
        int Nx = floor(width / (2 * radius));
        int Ny = floor(height / (2 * radius));
        int i,j;
        double Cx, Cy, AreaVeg, AreaGot = CircleArea(radius);
        int N;
        for (Posic p = getFirstLst(Figures); p != NIL p = getNextLst(Figures, p))
        {
            Node   node = (Node  ) getLst         (Figures, p);
            Entity ent  = (Entity) getInfoRadialT (Elements, node);
            if(isEntAVeg(ent)) // É um vegetal, então pode ser AFETADO
            {
                N = 0;
                for(i = 0; i < Nx; i++)
                {
                    for(j = 0; j < Ny; j++)
                    {
                        Cx = x + (1 + 2 * i) * radius;
                        Cy = y + (1 + 2 * j) * radius;
                        insertLst(Decos, createCircle(-1, Cx, Cy, radius, "red;stroke-dasharray:2 3", "none"));
                        if (isCircleInsideEnt(ent, Cx, Cy, radius)) N++;
                    }
                }
                if (N > 0) f(Elements, node, N * AreaGot, aux);
            }
        }
    }
    // Desalocar lista (Lista) e itens (Item), mas não os conteúdos
    // (...)
    Geometry Frame = createRectangle(-1, x, y, width, height, "red;stroke-dasharray:2 3", "none");
    insertLst(Decos, Frame);
}


void ContaminateVeg(RadialTree Elements, Node figure, double DropArea, void * aux)
{
    /*              TXT REPORT             */
    void ** clausure = (void **) aux;
    FILE *  TXTFile  = (FILE * ) clausure[0];
    Lista   Decos    = (Lista  ) clausure[1];
    Style   style    = (Style  ) clausure[2];
    fprintf(TXTFile, "\t");


    // Foi AFETADO -> aplicar efeito
    Entity ent     = (Entity) getInfoRadialT(Elements, figure);
    Geometry geo   = getEntGeo(ent);
    double AreaVeg = getEntArea(ent);
    double damage;
    if (getGeoClass(geo) == 't' || getGeoClass(geo) == 'l') damage = 0.1;
    else damage = DropArea / AreaVeg;
    double pastcont = getEntCont(ent);
    addEntCont(ent, (double) damage);
    if (getEntCont(ent) > 0.75)
    {
        // Hortaliça DEVE morrer
        Geometry death_mark = createText(-1, getGeoCords(geo)[0], getGeoCords(geo)[1], "red", "red", 'm', "X");
        setGeoStyle(death_mark, style);
        insertLst(Decos, death_mark);
        removeNoRadialT(Elements, figure);
        fprintf(TXTFile, "(ELIMINADA) ");
    }

    fprintf(TXTFile, "[%d]: %s - (%.2lf, %.2lf) FATOR DE COMPROMETIMENTO (anterior): %.2lf%% DANO RECEBIDO: %.2lf%%\n", getEntId(ent), strVegSpecies(ent), getGeoCords(getEntGeo(ent))[0], getGeoCords(getEntGeo(ent))[1], pastcont*100, damage*100);
}
void ContaminateRegion (ArqCmds QryFile, RadialTree Elements, char * lineBuffer, FILE * TXTFile, Lista Decos, Style style)
{
    char parameter[SIMPLE_PARAMETER_SIZE];

    getParametroI(QryFile, lineBuffer, 1, parameter, SIMPLE_PARAMETER_SIZE);
    double x = atof(parameter);
    getParametroI(QryFile, lineBuffer, 2, parameter, SIMPLE_PARAMETER_SIZE);
    double y = atof(parameter);
    getParametroI(QryFile, lineBuffer, 3, parameter, SIMPLE_PARAMETER_SIZE);
    double width = atof(parameter);
    getParametroI(QryFile, lineBuffer, 4, parameter, SIMPLE_PARAMETER_SIZE);
    double height = atof(parameter);
    getParametroI(QryFile, lineBuffer, 5, parameter, SIMPLE_PARAMETER_SIZE);
    double radius = atof(parameter);

    fprintf(TXTFile, "\n[*] %s\n", lineBuffer);
    fprintf(TXTFile, "\tHORTALICAS AFETADAS(S)\n");
    void * aux[] = {TXTFile, Decos, style};
    DistributeDrops(Elements, ContaminateVeg, x, y, width, height, radius, Decos, aux);
    insertLst(Decos, createCircle(-1, x, y, radius, "red", "none"));
}

void HealVeg(RadialTree Elements, Node figure, double DropArea, void * aux)
{
    // Foi AFETADO -> aplicar efeito
    Entity ent     = (Entity) getInfoRadialT(Elements, figure);
    Geometry geo = getEntGeo(ent);
    double AreaVeg = getEntArea(ent);
    double pastcont = getEntCont(ent);
    double heal = 0;

    FILE * TXTFile = (FILE *) aux;
    fprintf(TXTFile, "\t");
    if (pastcont > 0.0)
    {
        if (getGeoClass(geo) == 't' || getGeoClass(geo) == 'l') heal = 0.1;
        else heal = DropArea / AreaVeg;
        addEntCont(ent, -heal);
    }
    else // Normalização do contador de contaminação (0 <= contamination <= 0.75)
    {
        addEntCont(ent, (double) (-getEntCont(ent)));
        fprintf(TXTFile, "(SEM EFEITO) ");
    }

    fprintf(TXTFile, "[%d]: %s - (%.2lf, %.2lf) FATOR DE COMPROMETIMENTO (anterior): %.2lf%% CURA RECEBIDA: %.2lf%%\n",
        getEntId(ent),
        strVegSpecies(ent),
        getGeoCords(getEntGeo(ent))[0],
        getGeoCords(getEntGeo(ent))[1],
        pastcont*100,
        heal*100
    );
}
void HealRegion (ArqCmds QryFile, RadialTree Elements, char * lineBuffer, FILE * TXTFile, Lista Decos)
{
    char parameter[SIMPLE_PARAMETER_SIZE];

    getParametroI(QryFile, lineBuffer, 1, parameter, SIMPLE_PARAMETER_SIZE);
    double x = atof(parameter);
    getParametroI(QryFile, lineBuffer, 2, parameter, SIMPLE_PARAMETER_SIZE);
    double y = atof(parameter);
    getParametroI(QryFile, lineBuffer, 3, parameter, SIMPLE_PARAMETER_SIZE);
    double width = atof(parameter);
    getParametroI(QryFile, lineBuffer, 4, parameter, SIMPLE_PARAMETER_SIZE);
    double height = atof(parameter);
    getParametroI(QryFile, lineBuffer, 5, parameter, SIMPLE_PARAMETER_SIZE);
    double radius = atof(parameter);

    fprintf(TXTFile, "\n[*] %s\n", lineBuffer);
    fprintf(TXTFile, "\tHORTALICAS AFETADAS(S)\n");
    DistributeDrops(Elements, HealVeg, x, y, width, height, radius, Decos, (void *) TXTFile);
    insertLst(Decos, createCircle(-1, x, y, radius, "yellow", "none"));
}

void FertilizeVeg(RadialTree Elements, Node figure, double DropArea, void * aux)
{
    // Foi AFETADO -> aplicar efeito
    Entity ent      = (Entity) getInfoRadialT(Elements, figure);
    Geometry geo = getEntGeo(ent);
    double AreaVeg  = getEntArea(ent);
    double pastfert = getEntFert(ent);
    double fert ;
    if (getGeoClass(geo) == 't' || getGeoClass(geo) == 'l') fert = 0.1;
    else fert = DropArea / AreaVeg;
    addEntFert(ent, fert);

    FILE * TXTFile  = (FILE *) aux; 
    fprintf(TXTFile, "[%d]: %s - (%.2lf, %.2lf) FATOR DE ADUBACAO (anterior): %.2lf%% ADUBACAO RECEBIDA: %.2lf%%\n",
        getEntId(ent),
        strVegSpecies(ent),
        getGeoCords(getEntGeo(ent))[0],
        getGeoCords(getEntGeo(ent))[1],
        pastfert * 100,
        fert     * 100
    );
}
void FertilizeRegion (ArqCmds QryFile, RadialTree Elements, char * lineBuffer, FILE * TXTFile, Lista Decos)
{
    char parameter[SIMPLE_PARAMETER_SIZE];

    getParametroI(QryFile, lineBuffer, 1, parameter, SIMPLE_PARAMETER_SIZE);
    double x = atof(parameter);
    getParametroI(QryFile, lineBuffer, 2, parameter, SIMPLE_PARAMETER_SIZE);
    double y = atof(parameter);
    getParametroI(QryFile, lineBuffer, 3, parameter, SIMPLE_PARAMETER_SIZE);
    double width = atof(parameter);
    getParametroI(QryFile, lineBuffer, 4, parameter, SIMPLE_PARAMETER_SIZE);
    double height = atof(parameter);
    getParametroI(QryFile, lineBuffer, 5, parameter, SIMPLE_PARAMETER_SIZE);
    double radius = atof(parameter);

    fprintf(TXTFile, "\n[*] %s\n", lineBuffer);
    fprintf(TXTFile, "\tHORTALICAS AFETADAS(S)\n");
    DistributeDrops(Elements, FertilizeVeg, x, y, width, height, radius, Decos, (void *) TXTFile);
    insertLst(Decos, createCircle(-1, x, y, radius, "green", "none"));
}
void SpreadRegion (ArqCmds QryFile, RadialTree Elements, char * lineBuffer, FILE * TXTFile, Lista Decos)
{
    char parameter[SIMPLE_PARAMETER_SIZE];

    getParametroI(QryFile, lineBuffer, 1, parameter, SIMPLE_PARAMETER_SIZE);
    double x = atof(parameter);
    getParametroI(QryFile, lineBuffer, 2, parameter, SIMPLE_PARAMETER_SIZE);
    double y = atof(parameter);
    getParametroI(QryFile, lineBuffer, 3, parameter, SIMPLE_PARAMETER_SIZE);
    double width = atof(parameter);
    getParametroI(QryFile, lineBuffer, 4, parameter, SIMPLE_PARAMETER_SIZE);
    double height = atof(parameter);
    getParametroI(QryFile, lineBuffer, 5, parameter, SIMPLE_PARAMETER_SIZE);
    double factor = atof(parameter);
    getParametroI(QryFile, lineBuffer, 6, parameter, SIMPLE_PARAMETER_SIZE);
    double dx = atof(parameter);
    getParametroI(QryFile, lineBuffer, 7, parameter, SIMPLE_PARAMETER_SIZE);
    double dy = atof(parameter);
    getParametroI(QryFile, lineBuffer, 8, parameter, SIMPLE_PARAMETER_SIZE);
    int j = atoi(parameter);
    
    int k = 0;
    Lista Figures = createLst(-1);
    Node node;
    Entity ent, entCopy;
    Geometry geo;

    fprintf(TXTFile, "[*] %s\n", lineBuffer);
    fprintf(TXTFile, "\tHORTALICAS ATINIGIDAS\n");

    if (getInfosDentroRegiaoRadialT(Elements, x, y, x + width, y + height, (FdentroDeRegiao) isEntInsideRectangle, Figures))
    {
        if (factor >= 1)
        {
            for (Posic p = getFirstLst(Figures); p != NIL p = getNextLst(Figures, p))
            {
                node     = (Node  ) getLst(Figures, p);
                ent      = (Entity) getInfoRadialT(Elements, node);
                if (getEntClass(ent) == 'v')
                {
                    writeEntAttributtes(TXTFile, ent);
                    for (int i = 0; i < (int)(floor(factor)); i++)
                    {
                        entCopy = copyEnt(ent, j + k);
                        geo = getEntGeo(entCopy);
                        k++;
                        Dislocate_Geo(geo, dx + 10*i, dy + 5*i);
                        insertRadialT(Elements, getGeoCords(geo)[0], getGeoCords(geo)[1], entCopy);
                    }
                }
            }
        }
        else // Fatores "complexos"...
        {

        }
        
    }
    Geometry Frame1 = createRectangle(-1, x, y, width, height, "red;stroke-dasharray:2 3", "none");
    insertLst(Decos, Frame1);
    Geometry Frame2 = createRectangle(-1, x + dx, y + dy, width, height, "red;stroke-dasharray:2 3", "none");
    insertLst(Decos, Frame2);
    insertLst(Decos, createCircle(-1, x + dx, y + dy, 10, "red;stroke-dasharray:2 3", "red"));
}


void ReportHarvest (Info info, double x, double y, void * aux)
{
    Entity ent = (Entity) info;
    FILE * TXTFile = (FILE *) aux;

    if (getEntClass(ent) == 'h')
    {
        int pumpkins     = getEntSpeciesCount(ent, 'p');
        int cabbages     = getEntSpeciesCount(ent, 'r');
        int carrots      = getEntSpeciesCount(ent, 'c');
        int onions       = getEntSpeciesCount(ent, 'o');
        int strawberries = getEntSpeciesCount(ent, 's');
        int total = pumpkins + cabbages + carrots + onions + strawberries;
        fprintf(TXTFile, "\tColheitadeira [%d]: (%.2lf, %.2lf), w: %.2lf, h: %.2lf\n", 
            getEntId(ent),
            x,
            y,
            getGeoWidth(getEntGeo(ent)),
            getGeoHeight(getEntGeo(ent))
        );
        fprintf(TXTFile, "\t\tHORTALICAS COLHIDAS: %d - aboboras: %d repolhos: %d cenouras: %d cebolas: %d morangos: %d\n", 
            total, pumpkins, cabbages, carrots, onions, strawberries);
    }

}

void ReportAttributtes (ArqCmds QryFile, RadialTree Elements, char * lineBuffer, FILE * TXTFile, Lista Decos)
{
    char parameter[SIMPLE_PARAMETER_SIZE];

    getParametroI(QryFile, lineBuffer, 1, parameter, SIMPLE_PARAMETER_SIZE);
    int id = atoi(parameter);

    Node target = searchNodeByEntID(Elements, id);
    if (target == NULL)
    {
        printf("AVISO: [in ReportAttributtes]: figura '%d' nao existe no Banco de Dados\n", id);
        printf("        -> comando '%s' ignorado\n", lineBuffer);
        return;
    }
    Entity ent = getInfoRadialT(Elements, target);
    fprintf(TXTFile, "\n[*] %s\n", lineBuffer);
    if (getEntClass(ent) == 'h') ReportHarvest(ent, getGeoCords(getEntGeo(ent))[0], getGeoCords(getEntGeo(ent))[1], TXTFile);
    else writeEntAttributtes(TXTFile, ent);
}



void ReportHarvesters (ArqCmds QryFile, RadialTree Elements, char * lineBuffer, FILE * TXTFile, Lista Decos)
{
    fprintf(TXTFile, "\n[*] %s\n", lineBuffer);
    visitaProfundidadeRadialT(Elements, ReportHarvest, (void *) TXTFile);
}