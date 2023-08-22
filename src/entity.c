#include "entity.h"
#include "analyticgeometry.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>


struct Vegetable {
    char species;
    Geometry geo;
    double area;
    double fertilization;
    double contamination;
};

struct Harvester {
    Geometry geo;
    double * dim;
    int pumpkings;
    int cabbages;
    int carrots;
    int onions;
    int strawberries;
};

struct Miscelaneous {
    Geometry geo;
};


typedef struct StObject {
    char class;
    int id;
    union Classes {
        struct Vegetable    * vegt;
        struct Harvester    * harv;
        struct Miscelaneous * misc;
    } classes;

} Object;





Entity createEntity (Geometry geo, int id)
{
    Object * obj = malloc(sizeof(Object));
    obj->id = id;

    switch (getGeoClass(geo))
    {
        // Círculo = Abóbora
        case 'c':
        {
            obj->class = 'v';
            obj->classes.vegt = malloc(sizeof(struct Vegetable));
            obj->classes.vegt->geo = geo;
            obj->classes.vegt->fertilization = 0;
            obj->classes.vegt->contamination = 0;
            obj->classes.vegt->species = 'p';
            obj->classes.vegt->area = CircleArea(getGeoRadius(geo));
            break;
        }
        // Retângulo = Repolho
        case 'r':
        {
            obj->class = 'v';
            obj->classes.vegt = malloc(sizeof(struct Vegetable));
            obj->classes.vegt->geo = geo;
            obj->classes.vegt->fertilization = 0;
            obj->classes.vegt->contamination = 0;
            obj->classes.vegt->species = 'r';
            obj->classes.vegt->area = RectangleArea(getGeoWidth(geo), getGeoHeight(geo));
            
            break;
        }
        // Texto = outros 3 tipos de hortaliças
        case 't':
        {
            if (strcmp(getGeoText(geo), "@") == 0)
            {
                obj->class = 'v';
                obj->classes.vegt = malloc(sizeof(struct Vegetable));
                obj->classes.vegt->geo = geo;
                obj->classes.vegt->fertilization = 0;
                obj->classes.vegt->contamination = 0;
                obj->classes.vegt->species = 'o';
                obj->classes.vegt->area = 0.1;
            }
            else if (strcmp(getGeoText(geo), "%%") == 0)
            {
                obj->class = 'v';
                obj->classes.vegt = malloc(sizeof(struct Vegetable));
                obj->classes.vegt->geo = geo;
                obj->classes.vegt->fertilization = 0;
                obj->classes.vegt->contamination = 0;
                obj->classes.vegt->species = 'c';
                obj->classes.vegt->area = 0.1;
            }
            else if (strcmp(getGeoText(geo), "*") == 0)
            {
                obj->class = 'v';
                obj->classes.vegt = malloc(sizeof(struct Vegetable));
                obj->classes.vegt->geo = geo;
                obj->classes.vegt->fertilization = 0;
                obj->classes.vegt->contamination = 0;
                obj->classes.vegt->species = 's';
                obj->classes.vegt->area = 0.1;
            }
            else
            {
                obj->class = 'm';
                obj->classes.misc = malloc(sizeof(struct Miscelaneous));
                obj->classes.misc->geo = geo;
            }
            break;
        }
        // Linha = Miscelânea
        case 'l':
        {
            obj->class = 'm';
            obj->classes.misc = malloc(sizeof(struct Miscelaneous));
            obj->classes.misc->geo = geo;
            break;
        }
    }

    return (Entity) obj;
}


Entity copyEnt (Entity ent, int newId)
{
    Geometry newGeo = copyGeo(getEntGeo(ent));
    Entity newEnt = createEntity(newGeo, newId);
    //if (getEntClass(ent) == 'h') ConvertVegToHarvester(newEnt);
    return newEnt;
}







int getEntId (Entity ent)
{
    Object * obj = (Object *) ent;
    return obj->id;
}
Geometry getEntGeo (Entity ent)
{
    Object * obj = (Object *) ent;
    switch(obj->class)
    {
        case 'v':
            return obj->classes.vegt->geo;
        case 'h':
            return obj->classes.harv->geo;
        case 'm':
            return obj->classes.misc->geo;
    }
}

char getEntClass (Entity ent)
{
    Object * obj = (Object *) ent;
    return obj->class;
}




void ConvertVegToHarvester (Entity ent)
{
    Object * obj = (Object *) ent;
    if (obj->class == 'v' && obj->classes.vegt->species == 'r')
    {
        Geometry aux                    = obj->classes.vegt->geo;
        free(obj->classes.vegt);
        obj->class                      = 'h';
        obj->classes.harv               = malloc(sizeof(struct Harvester));
        obj->classes.harv->geo          = aux;
        obj->classes.harv->dim          = malloc(sizeof(double) * 2);
        obj->classes.harv->dim[0]       = getGeoWidth(aux);
        obj->classes.harv->dim[1]       = getGeoHeight(aux);
        obj->classes.harv->pumpkings    = 0;
        obj->classes.harv->cabbages     = 0;
        obj->classes.harv->carrots      = 0;
        obj->classes.harv->onions       = 0;
        obj->classes.harv->strawberries = 0;
    }
    else
    {
        printf("AVISO: [in ConvertVegToHarvester] figura de id '%d' nao e' um retangulo.\n", obj->id);
        printf("        -> '%d' permanece como hortalica/miscelânea.\n", obj->id);
    }
}
bool isEntAVeg(Entity ent)
{
    Object * obj = (Object *) ent;
    return obj->class == 'v';
}

// Vegetables Functions
double getEntCont (Entity veg)
{
    Object * obj = (Object *) veg;
    return obj->classes.vegt->contamination;
}
double getEntFert (Entity veg)
{
    Object * obj = (Object *) veg;
    return obj->classes.vegt->fertilization;
}
void   addEntCont (Entity veg, double cont)
{
    Object * obj = (Object *) veg;
    obj->classes.vegt->contamination += cont;
}
void   addEntFert (Entity veg, double fert)
{
    Object * obj = (Object *) veg;
    obj->classes.vegt->fertilization += fert;
}
char   getEntSpecies (Entity veg)
{
    Object * obj = (Object *) veg;
    return obj->classes.vegt->species;
}
double getEntArea (Entity veg)
{
    Object * obj = (Object *) veg;
    return obj->classes.vegt->area;
}


//Harvester Functions
void incrementEntSpeciesCount(Entity harvester, char species)
{
    Object * obj = (Object *) harvester;
    switch (species)
    {
        case 'p':
            obj->classes.harv->pumpkings++;
            break;
        case 'r':
            obj->classes.harv->cabbages++;
            break;
        case 'c':
            obj->classes.harv->carrots++;
            break;
        case 'o':
            obj->classes.harv->onions++;
            break;
        case 's':
            obj->classes.harv->strawberries++;
            break;
        default:
        {
            return;
        }
    }
}

int  getEntSpeciesCount(Entity harvester, char species)
{
    Object * obj = (Object *) harvester;
    switch (species)
    {
        case 'p':
            return obj->classes.harv->pumpkings;
        case 'r':
            return obj->classes.harv->cabbages;
        case 'c':
            return obj->classes.harv->carrots;
        case 'o':
            return obj->classes.harv->onions;
        case 's':
            return obj->classes.harv->strawberries;
    }
}