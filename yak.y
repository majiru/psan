%{
#include <stdio.h>
#include <string.h>

#include "yak.tab.h"
#include "dat.h"
#include "fncs.h"

extern int yylex();
extern int readInputForLexer(char* buffer,int *numBytesRead,int maxBytesToRead);

void
yyerror(const char *str)
{
	fprintf(stderr,"error: %s\n",str);
}

int yywrap()
{
        return 1;
}

%}

%union {
	int num;
	char *str;
}

%token <num> NUMBER
%token <str> 	VERB VAR STRING

%%
file:
	| file cmd
	;
cmd:
	VERB VAR list ';'
	{
		if(strcmp($1, "Set") == 0){
			regObj($2);
		} else if(strcmp($1, "Update") == 0){
			updateObj($2);
		} else {
			yyerror("Gave arguments for non Set\n");
		}
	}
	| VERB VAR ';'
	{
		if(strcmp($1, "Get") == 0)
			cacheObj($2);
		else
			yyerror("No arguments supplied for non Get");
	}
	;
list:
	assign
	| list assign
assign:
	VAR '=' STRING
	{
		regPair($1, $3, STR);
	}
	| VAR '=' NUMBER
	{
		regPair($1, &$3, NUM);
	}
	;
%%
