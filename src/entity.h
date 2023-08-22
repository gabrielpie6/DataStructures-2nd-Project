#ifndef ENTITY_H
#define ENTITY_H

#include "geometry.h"
#include <stdbool.h>
/*
CONVENÇÃO DE ENTIDADES (getEntType)):
        - Hortaliça 'v'
            - Abóbora: 'p'
            - Repolho: 'r'
            - Cenoura: 'c'
            - Cebola:  'o'
            - Morango: 's'
        - Colheitadeira: 'h'
        - Miscelânea: 'm'
*/


typedef void * Entity;

Entity createEntity (Geometry geo, int id);
Entity copyEnt (Entity ent, int newId);

int getEntId (Entity ent);
Geometry getEntGeo (Entity ent);
char getEntClass (Entity ent);
char getEntSpecies (Entity ent);

void ConvertVegToHarvester (Entity ent);
bool isEntAVeg(Entity ent);

// Vegetables Functions
double getEntCont (Entity veg);
double getEntFert (Entity veg);
void   addEntCont (Entity veg, double cont);
void   addEntFert (Entity veg, double fert);
double getEntArea (Entity veg);

//Harvester Functions
void incrementEntSpeciesCount(Entity harvester, char species);
int  getEntSpeciesCount(Entity harvester, char species);


#endif