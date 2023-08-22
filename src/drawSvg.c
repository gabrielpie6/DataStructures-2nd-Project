#include "drawSvg.h"

#include <stdio.h>
#include <stdlib.h>

// Função auxiliar
double Normalizar(double value)
{
    if (value < 0)
        value = 1;
    return value;
}

ArqSvg abreEscritaSvg(char *fn)
{
    FILE *file = fopen(fn, "w");
    if (file != NULL)
        fprintf(file, "<svg xmlns=\"http://www.w3.org/2000/svg\">\n");
    ArqSvg svg = file;
    return svg;
}

void preparaDecoracao(ArqSvg fsvg, char *deco, int decoLen,
		      char *corBorda, char *corPreenchimento,
		      char *larguraBorda, double transparencia,
		      double transparenciaPreenchimento, double transparenciaBorda)
{
    transparencia               = Normalizar(transparencia);
    transparenciaPreenchimento  = Normalizar(transparenciaPreenchimento);
    transparenciaBorda          = Normalizar(transparenciaBorda);

    sprintf(deco,
            "style = \"stroke:%s;fill:%s;stroke-width:%s;opacity:%lf;fill-opacity:%lf;stroke-opacity:%lf\"",
            corBorda, corPreenchimento, larguraBorda, transparencia, transparenciaPreenchimento, transparenciaBorda);
}

void escreveCirculoSvg(ArqSvg fsvg, double xc, double yc, double r, char *deco)
{
    if (fsvg != NULL)
    {
        fprintf(fsvg, "\n\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\"", xc, yc, r);
        if (deco != NULL)
            fprintf(fsvg, "\n\t\t\t%s />", deco);
        else
            fprintf(fsvg, " />");
    }
}

void escreveRetanguloSvg(ArqSvg fsvg, double x, double y, double larg, double alt, char *deco)
{
    if (fsvg != NULL)
    {
        fprintf(fsvg, "\n\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\"", x, y, larg, alt);
        if (deco != NULL)
            fprintf(fsvg, "\n\t\t\t%s />", deco);
        else
            fprintf(fsvg, " />");
    }
}

void escreveLinhaSvg(ArqSvg fsvg, double x1, double y1, double x2, double y2, char *deco)
{
    if (fsvg != NULL)
    {
        fprintf(fsvg, "\n\t<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\"", x1, y1, x2, y2);
        if (deco != NULL)
            fprintf(fsvg, "\n\t\t\t%s />", deco);
        else
            fprintf(fsvg, " />");
    }
}

void preparaDecoracaoTexto(ArqSvg fsvg, char *deco, int decoLen,
			   char *fontFamily, char *fontStyle,
			   char *fontWeight, char *fontSize,
			   char *fontColor, char *fontStroke, char *textAnchor)
{
    sprintf(deco,
            "style = \"font-family:%s;font-style:%s;font-weight:%s;font-size:%s;fill:%s;stroke:%s\"",
            fontFamily, fontStyle, fontWeight, fontSize, fontColor, fontStroke);
}

void escreveTextoSvg(ArqSvg fsvg, double x, double y, char *txt, char *textAnchor, char *decoTxt)
{
    if (fsvg != NULL)
    {
        fprintf(fsvg, "\n\t<text text-anchor=\"%s\" x=\"%lf\" y=\"%lf\"", textAnchor, x, y);
        if (decoTxt != NULL)
            fprintf(fsvg, "\n\t\t\t%s", decoTxt);
        fprintf(fsvg, " >%s</text>", txt);
    }
}

void fechaSvg(ArqSvg fsvg)
{
    FILE * ponteiro = fsvg;
    fprintf(ponteiro, "\n\n</svg>");
    fclose(ponteiro);
}

void applyRotation(char * deco, int decoLen, double theta, double xCenter, double yCenter)
{
    char * buffer = (char *) malloc(decoLen * sizeof(char));
    sprintf(buffer, "%s", deco);
    sprintf(deco, "transform = \"rotate(%lf, %lf, %lf)\"\n\t\t\t%s", theta, xCenter, yCenter, buffer);
}