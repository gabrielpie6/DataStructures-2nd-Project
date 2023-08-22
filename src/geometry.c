#include "geometry.h"

#include <stdlib.h>
#include <string.h>


/////////////////////////////////////////////
// Implementação das ESTRUTURAS escolhidas
//
struct Circle {
    double radius;
};

struct Rectangle {
    double height;
    double width;
};

struct Line {
    double anchor_1[2];
    double anchor_2[2];
};

struct TextStyle {
    char * font_family;
    char * font_weight;
    double font_size;
};

struct Text {
    char anchor;
    char * text;
    struct TextStyle * style;
};


typedef struct Object {
    //int id;
    double cords[2];
    double angle;
    char * colors[2];
    char class;
    union ClassAttributes {
        struct Circle    * circle;
        struct Rectangle * rectangle;
        struct Line      * line;
        struct Text      * text;
    } attributes;
} object;
/////////////////////////////////////////////



/////////////////////////////////////////////
// FUNÇÕES DE PROCESSAMENTO
// Transformações
// 
void Dislocate_Geo (Geometry geo, double dx, double dy)
{
    object * obj = (object *) geo;
    if (obj->class == 'l')
    {
        obj->attributes.line->anchor_1[0] += dx;
        obj->attributes.line->anchor_1[1] += dy;
        obj->attributes.line->anchor_2[0] += dx;
        obj->attributes.line->anchor_2[1] += dy;
    }
    else
    {
        obj->cords[0] += dx;
        obj->cords[1] += dy; 
    }
}
//
void Rotate_Geo (Geometry geo, double angle)
{
    object * obj = (object *) geo;
    obj->angle += angle;
}
//
void removeGeo(Geometry geo)
{
    object * obj = (object *) geo;
    free(obj->colors[0]);
    free(obj->colors[1]);
    switch (obj->class)
    {
        case 'c':
        {
            free(obj->attributes.circle);
            break;
        }
        case 'r':
        {
            free(obj->attributes.rectangle);
            break;
        }
        case 'l':
        {
            free(obj->attributes.line);
            break;
        }
        case 't':
        {
            free(obj->attributes.text->text);
            free(obj->attributes.text);
            break;
        }
    }
    free(obj);
}
/////////////////////////////////////////////



/////////////////////////////////////////////
// FUNÇÕES DE CRIAÇÃO
//
Geometry createCircle (int id, double x, double y, double radius, char * colorBorder, char * colorFill)
{
    object * obj = malloc(sizeof(object));
    //obj->id = id;
    obj->cords[0] = x;
    obj->cords[1] = y;
    obj->angle = 0;
    obj->colors[0] = malloc(sizeof(char) * COLOR_STRING_SIZE);
    strcpy(obj->colors[0], colorBorder);
    obj->colors[1] = malloc(sizeof(char) * COLOR_STRING_SIZE);
    strcpy(obj->colors[1], colorFill);

    obj->class = 'c';
    obj->attributes.circle = malloc(sizeof(struct Circle));

    struct Circle * circulo = (struct Circle *) obj->attributes.circle;
    circulo->radius = radius;

    return (Geometry) obj;
}

Geometry createRectangle (int id, double x, double y, double width, double height, char * colorBorder, char * colorFill)
{
    object * obj = malloc(sizeof(object));
    //obj->id = id;
    obj->cords[0] = x;
    obj->cords[1] = y;
    obj->angle = 0;
    obj->colors[0] = malloc(sizeof(char) * COLOR_STRING_SIZE);
    strcpy(obj->colors[0], colorBorder);
    obj->colors[1] = malloc(sizeof(char) * COLOR_STRING_SIZE);
    strcpy(obj->colors[1], colorFill);

    obj->class = 'r';
    obj->attributes.rectangle = malloc(sizeof(struct Rectangle));

    struct Rectangle * retangulo = (struct Rectangle *) obj->attributes.rectangle;
    retangulo->width = width;
    retangulo->height = height;

    return (Geometry) obj;
}

Geometry createLine (int id, double x1, double y1, double x2, double y2, char * colorFill)
{
    object * obj = malloc(sizeof(object));
    //obj->id = id;
    // Cords armazena o ponto médio da linha
    obj->cords[0] = (x1 + x2)/(2.0);
    obj->cords[1] = (y1 + y2)/(2.0);
    obj->angle = 0;
    
    obj->colors[1] = NULL;
    obj->colors[0] = malloc(sizeof(char) * COLOR_STRING_SIZE);
    strcpy(obj->colors[0], colorFill);

    obj->class = 'l';
    obj->attributes.line = malloc(sizeof(struct Line));

    // anchor_1 e anchor_2 armazenam os pontos de ancoragem da linha
    struct Line * linha = (struct Line *) obj->attributes.line;
    linha->anchor_1[0] = x1;
    linha->anchor_1[1] = y1;
    linha->anchor_2[0] = x2;
    linha->anchor_2[1] = y2;

    return (Geometry) obj;
}

Geometry createText (int id, double x, double y, char * colorBorder, char * colorFill, char anchor, char * text)
{
    object * obj = malloc(sizeof(object));
    //obj->id = id;
    obj->cords[0] = x;
    obj->cords[1] = y;
    obj->angle = 0;
    obj->colors[0] = malloc(sizeof(char) * COLOR_STRING_SIZE);
    strcpy(obj->colors[0], colorBorder);
    obj->colors[1] = malloc(sizeof(char) * COLOR_STRING_SIZE);
    strcpy(obj->colors[1], colorFill);

    obj->class = 't';
    obj->attributes.text = malloc(sizeof(struct Text));

    struct Text * texto = (struct Text *) obj->attributes.text;
    texto->anchor = anchor;
    texto->text = malloc(sizeof(char) * (strlen(text) + 1));
    strcpy(texto->text, text);
    texto->style = NULL;

    return (Geometry) obj;
}

Style createTextStyle (char * fontFamily, char * fontWeight, double fontSize)
{
    struct TextStyle * style = malloc(sizeof(struct TextStyle));
    style->font_family = malloc(sizeof(char) * COLOR_STRING_SIZE);
    strcpy(style->font_family, fontFamily);
    style->font_weight = malloc(sizeof(char) * COLOR_STRING_SIZE);
    strcpy(style->font_weight, fontWeight);
    style->font_size = fontSize;

    return (Style) style;
}

Geometry copyGeo (Geometry geo)
{
    Geometry eGeo = geo;
    switch (getGeoClass(geo))
    {
        case 'c':
        {
            double theta = getGeoAngle(eGeo);
            eGeo = createCircle(-2, getGeoCords(eGeo)[0], getGeoCords(eGeo)[1], getGeoRadius(eGeo), getGeoBorder_color(eGeo), getGeoFill_color(eGeo));
            setGeoAngle(eGeo, theta);
            break;
        }
        case 'r':
        {
            double theta = getGeoAngle(eGeo);
            eGeo = createRectangle(-2, getGeoCords(eGeo)[0], getGeoCords(eGeo)[1], getGeoWidth(eGeo), getGeoHeight(eGeo), getGeoBorder_color(eGeo), getGeoFill_color(eGeo));
            setGeoAngle(eGeo, theta);
            break;
        }
        case 'l':
        {
            double theta = getGeoAngle(eGeo);
            eGeo = createLine(-2, getGeoAnchor_1(eGeo)[0], getGeoAnchor_1(eGeo)[1], getGeoAnchor_2(eGeo)[0], getGeoAnchor_2(eGeo)[1], getGeoBorder_color(eGeo));
            setGeoAngle(eGeo, theta);
            break;
        }
        case 't':
        {
            Style style = getGeoStyle(eGeo);
            double theta = getGeoAngle(eGeo);
            eGeo = createText(-2, getGeoCords(eGeo)[0], getGeoCords(eGeo)[1], getGeoBorder_color(eGeo), getGeoFill_color(eGeo), getGeoAnchor(eGeo), getGeoText(eGeo));
            setGeoStyle(eGeo, style);
            setGeoAngle(eGeo, theta);
            break;
        }
    }
    return eGeo;
}
/////////////////////////////////////////////



/////////////////////////////////////////////
// GENERAL FUNCTIONS
// Getters
double * getGeoCords (Geometry geo)
{
    object * obj = (object *) geo;
    return &obj->cords[0];
}
double getGeoAngle (Geometry geo)
{
    object * obj = (object *) geo;
    return obj->angle;
}
char getGeoClass (Geometry geo)
{
    object * obj = (object *) geo;
    return obj->class;
}
char * getGeoBorder_color (Geometry geo)
{
    object * obj = (object *) geo;
    return obj->colors[0];
}
char * getGeoFill_color (Geometry geo)
{
    object * obj = (object *) geo;
    return obj->colors[1];
}
//
// Setters
//
void setGeoCords (Geometry geo, double x, double y)
{
    object * obj = (object *) geo;
    obj->cords[0] = x;
    obj->cords[1] = y;
}
void setGeoAngle (Geometry geo, double angle)
{
    object * obj = (object *) geo;
    obj->angle = angle;
}
void setGeoClass (Geometry geo, char class)
{
    object * obj = (object *) geo;
    obj->class = class;
}
void setGeoBorder_color (Geometry geo, char * color)
{
    object * obj = (object *) geo;
    strcpy(obj->colors[0], color);
}
void setGeoFill_color (Geometry geo, char * color)
{
    object * obj = (object *) geo;
    strcpy(obj->colors[1], color);
}
/////////////////////////////////////////////



/////////////////////////////////////////////
// GEOMETRY ATTRIBUTES FUNCTIONS
//
// Circle functions
//
double getGeoRadius (Geometry geo)
{
    object * obj = (object *) geo;
    struct Circle * circulo = (struct Circle *) obj->attributes.circle;
    return circulo->radius;
}
void setGeoRadius (Geometry geo, double radius)
{
    object * obj = (object *) geo;
    struct Circle * circulo = (struct Circle *) obj->attributes.circle;
    circulo->radius = radius;
}
//
// Rectangle functions
//
double getGeoWidth (Geometry geo)
{
    object * obj = (object *) geo;
    struct Rectangle * retangulo = (struct Rectangle *) obj->attributes.rectangle;
    return retangulo->width;
}
double getGeoHeight (Geometry geo)
{
    object * obj = (object *) geo;
    struct Rectangle * retangulo = (struct Rectangle *) obj->attributes.rectangle;
    return retangulo->height;
}
void setGeoWidth (Geometry geo, double width)
{
    object * obj = (object *) geo;
    struct Rectangle * retangulo = (struct Rectangle *) obj->attributes.rectangle;
    retangulo->width = width;
}
void setGeoHeight (Geometry geo, double height)
{
    object * obj = (object *) geo;
    struct Rectangle * retangulo = (struct Rectangle *) obj->attributes.rectangle;
    retangulo->height = height;
}
//
// Line functions
//
double * getGeoAnchor_1 (Geometry geo)
{
    object * obj = (object *) geo;
    struct Line * linha = (struct Line *) obj->attributes.line;
    return &linha->anchor_1[0];
}
double * getGeoAnchor_2 (Geometry geo)
{
    object * obj = (object *) geo;
    struct Line * linha = (struct Line *) obj->attributes.line;
    return &linha->anchor_2[0];
}
void setGeoAnchor_1 (Geometry geo, double x, double y)
{
    object * obj = (object *) geo;
    struct Line * linha = (struct Line *) obj->attributes.line;
    linha->anchor_1[0] = x;
    linha->anchor_1[1] = y;
}
void setGeoAnchor_2 (Geometry geo, double x, double y)
{
    object * obj = (object *) geo;
    struct Line * linha = (struct Line *) obj->attributes.line;
    linha->anchor_2[0] = x;
    linha->anchor_2[1] = y;
}
//
// Text functions
//
char getGeoAnchor (Geometry geo)
{
    object * obj = (object *) geo;
    struct Text * texto = (struct Text *) obj->attributes.text;
    return texto->anchor;
}
char * getGeoText (Geometry geo)
{
    object * obj = (object *) geo;
    struct Text * texto = (struct Text *) obj->attributes.text;
    return texto->text;
}
Style getGeoStyle (Geometry geo)
{
    object * obj = (object *) geo;
    struct Text * texto = (struct Text *) obj->attributes.text;
    return texto->style;
}
void setGeoAnchor (Geometry geo, char anchor)
{
    object * obj = (object *) geo;
    struct Text * texto = (struct Text *) obj->attributes.text;
    texto->anchor = anchor;
}
void setGeoText (Geometry geo, char * text)
{
    object * obj = (object *) geo;
    struct Text * texto = (struct Text *) obj->attributes.text;
    strcpy(texto->text, text);
}
void setGeoStyle (Geometry geo, Style style)
{
    object * obj = (object *) geo;
    struct Text * texto = (struct Text *) obj->attributes.text;
    texto->style = style;
}
//
// Style functions
//
void removeStyle (Style style)
{
    struct TextStyle * textStyle = (struct TextStyle *) style;
    free(textStyle->font_family);
    free(textStyle->font_weight);
    //free(&textStyle->font_size);
    free(textStyle);
}
char * getStyleFontWeight (Style style)
{
    struct TextStyle * textStyle = (struct TextStyle *) style;
    return textStyle->font_weight;
}
char * getStyleFontFamily (Style style)
{
    struct TextStyle * textStyle = (struct TextStyle *) style;
    return textStyle->font_family;
}
double getStyleFontSize (Style style)
{
    struct TextStyle * textStyle = (struct TextStyle *) style;
    return textStyle->font_size;
}
void setStyleFontWeight (Style style, char * fontWeight)
{
    struct TextStyle * textStyle = (struct TextStyle *) style;
    strcpy(textStyle->font_weight, fontWeight);
}
void setStyleFontFamily (Style style, char * fontFamily)
{
    struct TextStyle * textStyle = (struct TextStyle *) style;
    strcpy(textStyle->font_family, fontFamily);
}
void setStyleFontSize (Style style, double fontSize)
{
    struct TextStyle * textStyle = (struct TextStyle *) style;
    textStyle->font_size = fontSize;
}
/////////////////////////////////////////////