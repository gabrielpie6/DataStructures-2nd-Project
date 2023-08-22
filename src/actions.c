#include "actions.h"
#include "analyticgeometry.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>


const char *VegTypes[] = {"abobora", "repolho", "cenoura", "cebola", "morango"};
char * strVegSpecies (Entity veg)
{
    char ** strs = (char **) VegTypes;
    switch(getEntSpecies(veg))
    {
        case 'p':
        {
            return strs[0];
            break;
        }
        case 'r':
        {
            return strs[1];
            break;
        }
        case 'c':
        {
            return strs[2];
            break;
        }
        case 'o':
        {
            return strs[3];
            break;
        }
        case 's':
        {
            return strs[4];
            break;
        }
    }
}


void writeGeoInSVG(Geometry element, Clausura c)
{
    void ** clausure =  (void   **) c;
    ArqSvg  SVG      =  (ArqSvg   ) clausure[0];
    Style   style    =  (Style    ) clausure[1];
    double  dx       = *((double *) clausure[2]);
    double  dy       = *((double *) clausure[3]);

    char deco[DEFAULT_BUFFER_SIZE];
    char font_size[SHORT_PARAMETER_SIZE];
    char text_anchor[20];
    switch (getGeoClass(element))
    {
        case 'c':
        {
            getGeoCords(element)[0] += dx;
            getGeoCords(element)[1] += dy;
            preparaDecoracao(SVG, deco, DEFAULT_BUFFER_SIZE, getGeoBorder_color(element), getGeoFill_color(element), "1", 1, 1, 1);
            escreveCirculoSvg(SVG, getGeoCords(element)[0], getGeoCords(element)[1], getGeoRadius(element), deco);
            break;
        };
        case 'r':
        {
            getGeoCords(element)[0] += dx;
            getGeoCords(element)[1] += dy;
            preparaDecoracao(SVG, deco, DEFAULT_BUFFER_SIZE, getGeoBorder_color(element), getGeoFill_color(element), "1", 1, 1, 1);
            escreveRetanguloSvg(SVG, getGeoCords(element)[0], getGeoCords(element)[1], getGeoWidth(element), getGeoHeight(element), deco);
            break;
        };
        case 'l':
        {
            getGeoAnchor_1(element)[0] += dx;
            getGeoAnchor_1(element)[1] += dy;
            getGeoAnchor_2(element)[0] += dx;
            getGeoAnchor_2(element)[1] += dy;
            preparaDecoracao(SVG, deco, DEFAULT_BUFFER_SIZE, getGeoBorder_color(element), getGeoFill_color(element), "1", 1, 1, 1);
            escreveLinhaSvg(SVG, getGeoAnchor_1(element)[0], getGeoAnchor_1(element)[1], getGeoAnchor_2(element)[0], getGeoAnchor_2(element)[1], deco);
            break;
        };
        case 't':
        {
            getGeoCords(element)[0] += dx;
            getGeoCords(element)[1] += dy;
            switch(getGeoAnchor(element))
            {
                case 'i':
                    strcpy(text_anchor, "start");
                    break;
                case 'm':
                    strcpy(text_anchor, "middle");
                    break;
                case 'f':
                    strcpy(text_anchor, "end");
                    break;
            }
            style = getGeoStyle(element);
            sprintf(font_size, "%lf", getStyleFontSize(style));
            preparaDecoracaoTexto(SVG, deco, DEFAULT_BUFFER_SIZE, getStyleFontFamily(style), text_anchor, getStyleFontWeight(style), font_size, getGeoFill_color(element), getGeoBorder_color(element), text_anchor);
            applyRotation(deco, DEFAULT_BUFFER_SIZE, getGeoAngle(element), getGeoCords(element)[0], getGeoCords(element)[1]);
            escreveTextoSvg(SVG, getGeoCords(element)[0], getGeoCords(element)[1], getGeoText(element), text_anchor, deco);
            break;
        };
    }
}
void writeEntInSVG(Info i, double x, double y, void * aux)
//void writeEntInSVG(RadialTree tree, Node n, void * aux)
{
    Entity ent = (Entity) i;
    Geometry element = getEntGeo(ent);
    void ** clausure = (void **) aux;
    ArqSvg SVG = (ArqSvg) clausure[0];
    Style style = (Style) clausure[1];


    double dx = 0;
    double dy = 0;
    void * clausure2[4];
    clausure2[0] = (void *) SVG;
    clausure2[1] = (void *) style;
    clausure2[2] = (void *) &dx;
    clausure2[3] = (void *) &dy;
    writeGeoInSVG(element, clausure2);
}

void WriteRadialTreeEntsInSVG (ArqSvg SVG, RadialTree tree, Style style)
{
    void * clausure[2];
    clausure[0] = (void *) SVG;
    clausure[1] = (void *) style;

    visitaProfundidadeRadialT(tree, writeEntInSVG, clausure);
    //RadialPreorder(tree, getRootNodeRadialT(tree), writeEntInSVG, clausure);
}
void WriteGeoListInSvg (ArqSvg SVG, Lista L, Style style, double dx, double dy)
{
    void * clausure[4];
    clausure[0] = (void *) SVG;
    clausure[1] = (void *) style;
    clausure[2] = (void *) &dx;
    clausure[3] = (void *) &dy;
    fold(L, writeGeoInSVG, clausure);
}

void writeEntAttributtes(FILE * TXTFile, Entity ent)
{
    Geometry geo = getEntGeo(ent);
    int id = getEntId(ent);
    fprintf(TXTFile, "\t[%d] ", id);
    switch (getEntClass(ent))
    {
        case 'v':
        {
            fprintf(TXTFile, "%s - (%.2lf, %.2lf) Fator COMPROMETIMENTO: %.2lf%% Fator ADUBACAO: %.2lf%%\n", strVegSpecies(ent),
                getGeoCords(geo)[0], getGeoCords(geo)[1],
                getEntCont(ent)*100, getEntFert(ent)*100
            );
            break;
        }
        case 'h':
        {
            // Não é preciso
            break;
        }
        case 'm':
        {
            switch (getGeoClass(geo))
            {
                case 'l':
                {
                    fprintf(TXTFile, "linha - {(%.2lf, %.2lf), (%.2lf, %.2lf)}\n",
                        getGeoAnchor_1(geo)[0]     , getGeoAnchor_1(geo)[1],
                        getGeoAnchor_2(geo)[0]     , getGeoAnchor_2(geo)[1]
                    );
                    break;
                }
                case 't':
                {
                    fprintf(TXTFile, "texto (não-hortalica) - (%.2lf, %.2lf)\n",
                        getGeoCords(geo)[0]     , getGeoCords(geo)[1]
                    );
                    break;
                }
                default:
                {
                    printf("FORMATO DESCOONHECIDO\n");
                    break;
                }
            }
        }
    }
}





bool isEntInsideRectangle(Entity ent, double x1, double y1, double x2, double y2)
{
    // CONSIDERAR COMPLETAMENTE DENTRO!
    Geometry geo = getEntGeo(ent);
    switch(getGeoClass(geo))
    {
        case 'c':
        {
            double r = getGeoRadius(geo);
            double x = getGeoCords(geo)[0];
            double y = getGeoCords(geo)[1];
            return ((x1 + r <= x && x <= x2 - r) && (y1 + r <= y && y <= y2 - r));
            break;
        }
        case 'r':
        {
            double w = getGeoWidth(geo);
            double h = getGeoHeight(geo);
            double x = getGeoCords(geo)[0];
            double y = getGeoCords(geo)[1];
            return (((x1 <= x     && x     <= x2) && (y1 <= y     && y     <= y2)) /*Vertice 1*/ &&
                    ((x1 <= x + w && x + w <= x2) && (y1 <= y + h && y + h <= y2)) /*Vertice 2*/);
            break;
        }
        case 't':
        {
            return isPointInsideRectangle(getGeoCords(geo)[0], getGeoCords(geo)[1], x1, y1, x2, y2);
            break;
        }
        case 'l':
        {
            double xA = getGeoAnchor_1(geo)[0];
            double yA = getGeoAnchor_1(geo)[1];
            double xB = getGeoAnchor_2(geo)[0];
            double yB = getGeoAnchor_2(geo)[1];

            return (((x1 <= xA && xA <= x2) && (y1 <= yA && yA <= y2)) /*Vertice 1*/ &&
                    ((x1 <= xB && xB <= x2) && (y1 <= yB && yB <= y2)) /*Vertice 2*/);
            break;
        }
    }
}







bool isPointInsideCircle(double x, double y, double Cx, double Cy, double Cr)
{
    return (pow(x - Cx, 2) + pow(y - Cy, 2) <= pow(Cr, 2));
}
bool isEntInsideCircle(Entity ent, double Cx, double Cy, double Cr)
{
    // CONSIDERAR COMPLETAMENTE DENTRO!
    Geometry geo = getEntGeo(ent);
    switch(getGeoClass(geo))
    {
        case 'c':
        {
            double r = getGeoRadius(geo);
            double x = getGeoCords(geo)[0];
            double y = getGeoCords(geo)[1];
            return ((r <= Cr) && isPointInsideCircle(x, y, Cx, Cy, Cr) &&
                    (r <= Cr - distance(x, y, Cx, Cy)));
            break;
        }
        case 'r':
        {
            double w = getGeoWidth(geo);
            double h = getGeoHeight(geo);
            double x = getGeoCords(geo)[0];
            double y = getGeoCords(geo)[1];
            return ((isPointInsideCircle(x    , y    , Cx, Cy, Cr)) /*Vertice 1*/ &&
                    (isPointInsideCircle(x + w, y    , Cx, Cy, Cr)) /*Vertice 2*/ &&
                    (isPointInsideCircle(x + w, y + h, Cx, Cy, Cr)) /*Vertice 3*/ &&
                    (isPointInsideCircle(x    , y + h, Cx, Cy, Cr)) /*Vertice 4*/);
            break;
        }
        case 't':
        {
            return (isPointInsideCircle(getGeoCords(geo)[0], getGeoCords(geo)[1], Cx, Cy, Cr));
            break;
        }
        case 'l':
        {
            double xA = getGeoAnchor_1(geo)[0];
            double yA = getGeoAnchor_1(geo)[1];
            double xB = getGeoAnchor_2(geo)[0];
            double yB = getGeoAnchor_2(geo)[1];

            return ((isPointInsideCircle(xA, yA, Cx, Cy, Cr)) /*Vertice 1*/ &&
                    (isPointInsideCircle(xB, yB, Cx, Cy, Cr)) /*Vertice 2*/);
            break;
        }
    }
}

bool isCircleInsideEnt(Entity ent, double Cx, double Cy, double Cr)
{
    // CONSIDERAR COMPLETAMENTE DENTRO!
    Geometry geo = getEntGeo(ent);
    switch(getGeoClass(geo))
    {
        case 'c':
        {
            double r = getGeoRadius(geo);
            double x = getGeoCords(geo)[0];
            double y = getGeoCords(geo)[1];
            return ((Cr <= r) && isPointInsideCircle(Cx, Cy, x, y, r) &&
                    (Cr <= r - distance(Cx, Cy, x, y)));
            break;
        }
        case 'r':
        {
            double x1 =      getGeoCords  (geo)[0];
            double y1 =      getGeoCords  (geo)[1];
            double x2 = x1 + getGeoWidth  (geo);
            double y2 = y1 + getGeoHeight (geo);
            return ((x1 + Cr <= Cx && Cx <= x2 - Cr) && (y1 + Cr <= Cy && Cy <= y2 - Cr));
            break;
        }
        case 't':
        {
            return (isPointInsideCircle(getGeoCords(geo)[0], getGeoCords(geo)[1], Cx, Cy, Cr));
            break;
        }
        case 'l':
        {
            double xA = getGeoAnchor_1(geo)[0];
            double yA = getGeoAnchor_1(geo)[1];
            double xB = getGeoAnchor_2(geo)[0];
            double yB = getGeoAnchor_2(geo)[1];

            return ((isPointInsideCircle(xA, yA, Cx, Cy, Cr)) /*Vertice 1*/ &&
                    (isPointInsideCircle(xB, yB, Cx, Cy, Cr)) /*Vertice 2*/);
            break;
        }
    }
}

double DoHarvest (RadialTree tree, Entity harvester, int steps, char direction, FILE * TXTFile, Lista Decos)
{
    Geometry harvGeo = getEntGeo(harvester);
    double w  = getGeoWidth(harvGeo);
    double h  = getGeoHeight(harvGeo);
    double x1 = getGeoCords(harvGeo)[0];
    double y1 = getGeoCords(harvGeo)[1];
    double x2 = x1 + w;
    double y2 = y1 + h;

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


    double Harvest = 0;
    Node node;
    Entity ent;

    Lista Plants[steps];
    for (int i = 0; i < steps; i++)
    {
        Plants[i] = createLst(-1);
        insertLst(Decos, createRectangle(-1, x1, y1, w, h, "blue;stroke-dasharray:2 3", "none"));
        if (getInfosDentroRegiaoRadialT(tree, x1, y1, x2, y2, (FdentroDeRegiao) isEntInsideRectangle, Plants[i]))
        {
            // Contabilizar plantas
            for (Posic p = getFirstLst(Plants[i]); p != NIL p = getNextLst(Plants[i], p))
            {
                node     = (Node  ) getLst(Plants[i], p);
                ent      = (Entity) getInfoRadialT(tree, node);
                if (getEntClass(ent) != 'h')
                {
                    Harvest += (double) PlantValue(ent);
                    if (getEntClass(ent) == 'v')
                    {
                        incrementEntSpeciesCount(harvester, getEntSpecies(ent));
                    }
                    removeNoRadialT(tree, node);
                    // Remover item da lista também (???)

                    writeEntAttributtes(TXTFile, ent);
                }
            }
        }
        x1 += X;
        x2 += X;
        y1 += Y;
        y2 += Y;
    }
    // Desalocar listaS (???)
    return Harvest;
}

double PlantValueFormula (double InitialWeight, double Contamination, double Fertilization)
{
    return (1 - Contamination) * (1 + floor(Fertilization) * 0.1) * InitialWeight;
}
double PlantValue (Entity plant)
{
    double value = 0;
    switch(getEntClass(plant))
    {
        case 'v':
        {
            double contamination = getEntCont(plant);
            double fertilization = getEntFert(plant);
            switch(getEntSpecies(plant))
            {
                case 'p':
                {
                    value = PlantValueFormula(2000, contamination, fertilization);
                    break;
                }
                case 'r':
                {
                    value = PlantValueFormula(1000, contamination, fertilization);
                    break;
                }
                case 'c':
                {
                    value = PlantValueFormula(70, contamination, fertilization);
                    break;
                }
                case 'o':
                {
                    value = PlantValueFormula(200, contamination, fertilization);
                    break;
                }
                case 's':
                {
                    value = PlantValueFormula(20, contamination, fertilization);
                    break;
                }
            }
            break;
        }
        case 'm':
        {
            Geometry geo = getEntGeo(plant);
            switch(getGeoClass(geo))
            {
                case 't':
                {
                    value = 15;
                    break;
                }
                case 'l':
                {
                    value = 10 * distance(getGeoAnchor_1(geo)[0], getGeoAnchor_1(geo)[1], getGeoAnchor_2(geo)[0], getGeoAnchor_2(geo)[1]);
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return value;
}

bool compareNodeEntID (Node n, void * aux)
{
    void ** clausure = (void **) aux;
    RadialTree tree  = (RadialTree) clausure[0];
    int * id         = (int *)      clausure[1];

    return getEntId(getInfoRadialT(tree, n)) == *id;
}
Node searchNodeByEntID(RadialTree tree, int id)
{
    void * aux[2];
    aux[0] = (void *) tree;
    aux[1] = (void *) &id;
    Node target = buscaNodePorFuncao(tree, compareNodeEntID, (void *) aux);
    return target;
}