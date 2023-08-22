#ifndef ARQSVG__
#define ARQSVG__

/*
  Provê operações para escrita de arquivos básicos no formato SVG.

  O arquivo deve ser criado por abreEscritaSvg. A seguir, círculos,
retângulos, linhas e textos podem ser acrescentados ao arquivo.

  As operacoes preparaDecoracao e preparaDecoracaoTexto produzem atributos
para que as figuras e textos (respectivamente) possam ter seus atributos
de apresentação (ex. cor, transparencia, etc) customizados.

  Por fim, o arquivo deve ser fechado por meio da operacao fechaSvg.
Note que nao existem operacoes para modificacao de um arquivo existente.

  Para mais detalhes sobre o formato SVG, consulte a especificação do
formato ou tutoriais, como:

     https://developer.mozilla.org/pt-BR/docs/Web/SVG/Tutorial
 */

typedef void *ArqSvg;

/*
  Cria o arquivo "fn" para escrita. Substitui o arquivo, caso exista.
  Retorna NULL, caso nao tenha sido possivel criar arquivo.
 */
ArqSvg abreEscritaSvg(char *fn);


/*
  Preenche o vetor "deco" (com capacidade de "decoLen" caracteres) com atributos
para formatacao de uma figura SVG. Este vetor pode, posteriormente, ser usado
como argumento em invocacoes de escreveCirculoSvg, escreveRetanguloSvg,
escreveLinhaSvg. Para não incluir algum dos parâmetros na decoracao, 
atribuir NULL (para char *) ou valores negativos (double) ao respectivo parametro.

    Nao considerar valor: NULL para char * e valores negativos para double
    font style: normal | italic | oblique
    font weight: normal | bold | bolder | lighter
    text anchor: start | middle |end
 */
void preparaDecoracao(ArqSvg fsvg, char *deco, int decoLen,
		      char *corBorda, char *corPreenchimento,
		      char *larguraBorda, double transparencia,
		      double transparenciaPreenchimento, double transparenciaBorda);

/*
   Insere uma circunferencia com centro em (xc,yc) de raio r. Atributos de formatacao
(cores, largura de bordas, etc) podem ser informados pela string deco (atribuir NULL, 
caso nao queira atribuir decoracao). As formatacoes mais usuais podem preparadas por
meio da operacao preparaDecoracao.
 */
void escreveCirculoSvg(ArqSvg fsvg, double xc, double yc, double r, char *deco);

/*
   Insere um retangulo com canto inferior esquerdo  em (x,y), de largura larg e altura alt. 
Atributos de formatacao (cores, largura de bordas, etc) podem ser informados pela string 
deco (atribuir NULL, caso nao queira atribuir decoracao). As formatacoes mais usuais 
podem preparadas por meio da operacao preparaDecoracao.
 */
void escreveRetanguloSvg(ArqSvg fsvg, double x, double y, double larg, double alt, char *deco);

/*
   Insere uma linha com extremidades em (x1,y1) e (x2,y2). 
Atributos de formatacao (cores, largura de bordas, etc) podem ser informados pela string 
deco (atribuir NULL, caso nao queira atribuir decoracao). As formatacoes mais usuais 
podem preparadas por meio da operacao preparaDecoracao.
 */
void escreveLinhaSvg(ArqSvg fsvg, double x1, double y1, double x2, double y2, char *deco);

/* Font style:  */
#define FS_NORMAL "normal"
#define FS_ITALIC "italic"
#define FS_OBLIQUE "oblique"

/* Font weight */
#define FW_BOLD "bold"
#define FW_NORMAL "normal"
#define FW_BOLDER "bolder"
#define FW_LIGHTER "lighter"

#define TA_START "start"
#define TA_MIDDLE "middle"
#define TA_END "end"

/*
  Preenche o vetor "deco" (com capacidade de "decoLen" caracteres) com atributos
para formatacao de um texto SVG. Este vetor pode, posteriormente, ser usado
como argumento em invocacoes de escreveTextoSvg.
Para não incluir algum dos parâmetros na decoracao, 
atribuir NULL (para char *) ao respectivo parametro.

  Os valores para os parametros estao relacionados abaixo. Usar as constantes
acima.
   
    font style: normal | italic | oblique
    font weight: normal | bold | bolder | lighter
    text anchor: start | middle |end
 */

void preparaDecoracaoTexto(ArqSvg fsvg, char *deco, int decoLen,
			   char *fontFamily, char *fontStyle,
			   char *fontWeight, char *fontSize,
			   char *fontColor, char *fontStroke, char *textAnchor);

/*
  Insere no arquivo svg o texto txt na coordenada (x,y).
Atributos de formatacao (cores, fonte, etc) podem ser informados pela string 
deco (atribuir NULL, caso nao queira atribuir decoracao). As formatacoes mais usuais 
podem preparadas por meio da operacao preparaDecoracaoTexto.
 */
void escreveTextoSvg(ArqSvg fsvg, double x, double y, char *txt, char *textAnchor, char *decoTxt);

/* Fecha o arquivo fsvg. */
void fechaSvg(ArqSvg fsvg);

void applyRotation(char * deco, int decoLen, double theta, double xCenter, double yCenter);

#endif