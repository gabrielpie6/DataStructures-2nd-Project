#include "geoFile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int ReadGeoFile (RadialTree Elements, char * geoPath, Style defaultStyle)
{
    /////////////////////////////////////////////
    //       LEITURA DO ARQUIVO .geo           //
    /////////////////////////////////////////////
    ArqCmds   GeoFile      = abreArquivoCmd(geoPath);
    char    * geoFileName  = geoPath;
    Style     style        = defaultStyle;


    char buffer    [DEFAULT_BUFFER_SIZE];
    char parameter [SIMPLE_PARAMETER_SIZE];
    

    if (GeoFile == NULL) {printf("ERRO: [in: ReadGeoFile]: nao foi possivel abrir arquivo '%s'\n", geoFileName); return 1;}
    else
    {
        Entity entity;

        while (leLinha(GeoFile, buffer, DEFAULT_BUFFER_SIZE))
        {
            getParametroI(GeoFile, buffer, 0, parameter, SIMPLE_PARAMETER_SIZE);

                 if (strcmp(parameter, "ts") == 0)           readActTextStyle (GeoFile, buffer, style);
            else if (strcmp(parameter, "c" ) == 0) {entity = readActCircle    (GeoFile, buffer);        insertRadialT(Elements, getGeoCords(getEntGeo(entity))[0], getGeoCords(getEntGeo(entity))[1], (Info) entity);}
            else if (strcmp(parameter, "r" ) == 0) {entity = readActRectangle (GeoFile, buffer);        insertRadialT(Elements, getGeoCords(getEntGeo(entity))[0], getGeoCords(getEntGeo(entity))[1], (Info) entity);}
            else if (strcmp(parameter, "l" ) == 0) {entity = readActLine      (GeoFile, buffer);        insertRadialT(Elements, getGeoAnchor_1(getEntGeo(entity))[0], getGeoAnchor_1(getEntGeo(entity))[1], (Info) entity);}
            else if (strcmp(parameter, "t" ) == 0) {entity = readActText      (GeoFile, buffer, style); insertRadialT(Elements, getGeoCords(getEntGeo(entity))[0], getGeoCords(getEntGeo(entity))[1], (Info) entity);}
            else     printf("ERRO: [in: ReadGeoFile]: comando '%s' nao reconhecido em '%s'\n", parameter, geoFileName);
        }
        fechaArquivoCmd(GeoFile);
        return 0;
    }
}














Entity readActCircle (ArqCmds GeoFile, char * lineBuffer)
{
    // Is a circle
    char attributes[SIMPLE_PARAMETER_SIZE];
    char border_color[SIMPLE_PARAMETER_SIZE];
    char fill_color[SIMPLE_PARAMETER_SIZE];


    getParametroI(GeoFile, lineBuffer, 1, attributes, SIMPLE_PARAMETER_SIZE);
    int id = atoi(attributes);
    getParametroI(GeoFile, lineBuffer, 2, attributes, SIMPLE_PARAMETER_SIZE);
    double x = atof(attributes);
    getParametroI(GeoFile, lineBuffer, 3, attributes, SIMPLE_PARAMETER_SIZE);
    double y = atof(attributes);
    getParametroI(GeoFile, lineBuffer, 4, attributes, SIMPLE_PARAMETER_SIZE);
    double radius = atof(attributes);
    getParametroI(GeoFile, lineBuffer, 5, border_color, SIMPLE_PARAMETER_SIZE);
    getParametroI(GeoFile, lineBuffer, 6, fill_color, SIMPLE_PARAMETER_SIZE);
    
    
    Geometry circle = createCircle(id, x, y, radius, border_color, fill_color);
    Entity entity = createEntity(circle, id);
    return entity;
}

Entity readActRectangle (ArqCmds GeoFile, char * lineBuffer)
{
    char attributes[SIMPLE_PARAMETER_SIZE];
    char border_color[SIMPLE_PARAMETER_SIZE];
    char fill_color[SIMPLE_PARAMETER_SIZE];


    // Is a rectangle
    getParametroI(GeoFile, lineBuffer, 1, attributes, SIMPLE_PARAMETER_SIZE);
    int id = atoi(attributes);
    getParametroI(GeoFile, lineBuffer, 2, attributes, SIMPLE_PARAMETER_SIZE);
    double x = atof(attributes);
    getParametroI(GeoFile, lineBuffer, 3, attributes, SIMPLE_PARAMETER_SIZE);
    double y = atof(attributes);
    getParametroI(GeoFile, lineBuffer, 4, attributes, SIMPLE_PARAMETER_SIZE);
    double width = atof(attributes);
    getParametroI(GeoFile, lineBuffer, 5, attributes, SIMPLE_PARAMETER_SIZE);
    double height = atof(attributes);
    getParametroI(GeoFile, lineBuffer, 6, border_color, SIMPLE_PARAMETER_SIZE);
    getParametroI(GeoFile, lineBuffer, 7, fill_color, SIMPLE_PARAMETER_SIZE);

    Geometry rectangle = createRectangle(id, x, y, width, height, border_color, fill_color);
    Entity entity = createEntity(rectangle, id);
    return entity;
}

Entity readActLine (ArqCmds GeoFile, char * lineBuffer)
{
    char attributes[SIMPLE_PARAMETER_SIZE];
    char border_color[SIMPLE_PARAMETER_SIZE];

    // Is a line
    getParametroI(GeoFile, lineBuffer, 1, attributes, SIMPLE_PARAMETER_SIZE);
    int id = atoi(attributes);
    getParametroI(GeoFile, lineBuffer, 2, attributes, SIMPLE_PARAMETER_SIZE);
    double x1 = atof(attributes);
    getParametroI(GeoFile, lineBuffer, 3, attributes, SIMPLE_PARAMETER_SIZE);
    double y1 = atof(attributes);
    getParametroI(GeoFile, lineBuffer, 4, attributes, SIMPLE_PARAMETER_SIZE);
    double x2 = atof(attributes);
    getParametroI(GeoFile, lineBuffer, 5, attributes, SIMPLE_PARAMETER_SIZE);
    double y2 = atof(attributes);
    getParametroI(GeoFile, lineBuffer, 6, border_color, SIMPLE_PARAMETER_SIZE);

    Geometry line = createLine(id, x1, y1, x2, y2, border_color);
    Entity entity = createEntity(line, id);
    return entity;
}

Entity readActText (ArqCmds GeoFile, char * lineBuffer, Style style)
{
    char attributes[SIMPLE_PARAMETER_SIZE];
    char border_color[SIMPLE_PARAMETER_SIZE];
    char fill_color[SIMPLE_PARAMETER_SIZE];
    char text_buffer[DEFAULT_BUFFER_SIZE];

    // Is a text
    getParametroI(GeoFile, lineBuffer, 1, attributes, SIMPLE_PARAMETER_SIZE);
    int id = atoi(attributes);
    getParametroI(GeoFile, lineBuffer, 2, attributes, SIMPLE_PARAMETER_SIZE);
    double x = atof(attributes);
    getParametroI(GeoFile, lineBuffer, 3, attributes, SIMPLE_PARAMETER_SIZE);
    double y = atof(attributes);
    getParametroI(GeoFile, lineBuffer, 4, border_color, SIMPLE_PARAMETER_SIZE);
    getParametroI(GeoFile, lineBuffer, 5, fill_color, SIMPLE_PARAMETER_SIZE);
    getParametroI(GeoFile, lineBuffer, 6, attributes, SIMPLE_PARAMETER_SIZE);
    char anchor = attributes[0];
    getParametroDepoisI(GeoFile, lineBuffer, 7, text_buffer, DEFAULT_BUFFER_SIZE);

    // Remove o caractere de fim de linha EOL (13) do final da string (se houver)
    if (text_buffer[strlen(text_buffer) - 1] == 13) text_buffer[strlen(text_buffer) - 1] = '\0';


    Geometry text = createText(id, x, y, border_color, fill_color, anchor, text_buffer);
    setGeoStyle(text, style);
    Entity entity = createEntity(text, id);
    return entity;
}

void readActTextStyle (ArqCmds GeoFile, char * lineBuffer, Style created_already_style)
{
    char attributes[SIMPLE_PARAMETER_SIZE];

    // Is a text style
    getParametroI(GeoFile, lineBuffer, 1, attributes, SIMPLE_PARAMETER_SIZE);
    setStyleFontFamily(created_already_style, attributes);
    getParametroI(GeoFile, lineBuffer, 2, attributes, SIMPLE_PARAMETER_SIZE);

    if      (strcmp(attributes, "n" ) == 0) setStyleFontWeight(created_already_style, "normal");
    else if (strcmp(attributes, "b" ) == 0) setStyleFontWeight(created_already_style, "bold");
    else if (strcmp(attributes, "b+") == 0) setStyleFontWeight(created_already_style, "bolder");
    else if (strcmp(attributes, "l" ) == 0) setStyleFontWeight(created_already_style, "lighter");
    else
    {
        setStyleFontWeight(created_already_style, attributes);  
        printf("AVISO: [in readActTextStyle]: comando ts em *.geo com font weight '%s' desconhecido\n", attributes);
        printf("     - Deveria ser: 'n', 'b', \"b+\" ou 'l' -> '%s' foi atribuido exatamente em font weigth do estilo\n", attributes);
    }
    getParametroI(GeoFile, lineBuffer, 3, attributes, SIMPLE_PARAMETER_SIZE);
    setStyleFontSize(created_already_style, atof(attributes));
}
