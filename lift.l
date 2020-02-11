%{
#include <stdio.h>
#include "yak.tab.h"

#undef YY_INPUT
extern int (*reader)(char*, int*,int);
#define YY_INPUT(b,r,s) reader(b,&r,s)

%}

%%
^(Get|Update|Set)	yylval.str=strdup(yytext);return VERB;
\"[ \n\ta-zA-Z0-9]*\"	yylval.str=strdup(yytext);return STRING;
[a-zA-Z][a-zA-Z0-9]*	yylval.str=strdup(yytext);return VAR;
[0-9]+			yylval.num=atoi(yytext); return NUMBER;
[ \t]+			/* ignore whitespace */;
\n			return ';';
=			return '=';
%%
