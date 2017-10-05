%option bison-locations
%option bison-bridge
%option reentrant
%option noyywrap
%option nounput

%{
#include <stdlib.h>
#include "lslmini.hh"
#include "lslmini.tab.h"

#ifdef _MSC_VER
#include <io.h>
#define isatty _isatty
#endif /* _MSC_VER */

#define LLOC_RESET()    yylloc->first_column = yylloc->first_line = yylloc->last_column = yylloc->last_line = 1;
#define LLOC_LINES(num) yylloc->last_column = 1; yylloc->last_line += (num);
#define LLOC_STEP()     yylloc->first_column = yylloc->last_column; yylloc->first_line = yylloc->last_line;
#define YY_USER_ACTION  yylloc->last_column += yyleng;
#define YY_USER_INIT    LLOC_RESET()

char *parse_string(char *input);

%}


N			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{N}+
FS			(f|F)

 /* exclusive state to eat comments of any length without overflowing any buffers */
%x COMMENT
%x C_COMMENT
 /* exclusive state to skip preprocessor commands */
%x PREPROC

%%
%{
	/* HACK */
	yylloc = yylloc_param;
	LLOC_STEP();
%}
"//"                { BEGIN COMMENT; }
<COMMENT>"$[E"{N}{5}"]" {
		ErrorCode e = (ErrorCode) strtoul( yytext+3, NULL, 10 );
		LOG( LOG_INFO, yylloc, "Adding assertion for E%d.", (int)e );
		Logger::get()->add_assertion( yylloc->first_line, e );
	}
<COMMENT>.			{ /* eat comments */ }
<COMMENT>\n			{ BEGIN 0; LLOC_LINES(1); LLOC_STEP(); }

"/*"				{ BEGIN C_COMMENT; }
<C_COMMENT>"*/"		{ BEGIN 0; LLOC_STEP(); }
<C_COMMENT>\n		{ LLOC_LINES(1); LLOC_STEP(); }
<C_COMMENT>.		{ LLOC_STEP(); }

"integer"			{ return(INTEGER); }
"float"				{ return(FLOAT_TYPE); }
"string"			{ return(STRING); }
"key"				{ return(LLKEY); }
"vector"			{ return(VECTOR); }
"quaternion"		{ return(QUATERNION); }
"rotation"			{ return(QUATERNION); }
"list"				{ return(LIST); }

"default"			{ yylval->sval = new char[strlen(yytext) + 1]; strcpy(yylval->sval, yytext); return(STATE_DEFAULT); }
"state"				{ return(STATE); }
"event"				{ return(EVENT); }
"jump"				{ return(JUMP); }
"return"			{ return(RETURN); }
"if"				{ return(IF); }
"else"				{ return(ELSE); }
"for"				{ return(FOR); }
"do"				{ return(DO); }
"while"				{ return(WHILE); }
"print"				{ return(PRINT); }

"."					{ return(PERIOD); }


0[xX]{H}+			{ yylval->ival = strtoul(yytext, NULL, 0);  return(INTEGER_CONSTANT); }
{N}+				{ yylval->ival = strtoul(yytext, NULL, 10); return(INTEGER_CONSTANT); }

"TRUE"				{ yylval->ival = 1; return(INTEGER_TRUE); }
"FALSE"				{ yylval->ival = 0; return(INTEGER_FALSE); }

{L}({L}|{N})*		{
		if (switch_stmt) {
			if (!strcmp(yytext, "switch")) return(SWITCH);
			if (!strcmp(yytext, "break")) return(BREAK);
			if (!strcmp(yytext, "case")) return(CASE);
		}
		yylval->sval = new char[strlen(yytext) + 1]; strcpy(yylval->sval, yytext);
		return(IDENTIFIER);
	}

{N}+{E}{FS}			{
		yylval->fval = (F32)atof(yytext);
		ERROR( yylloc, E_SYNTAX_ERROR, "F can't go after a float number without a decimal point." );
		return(FP_CONSTANT);
	}
{N}+{E}						{ yylval->fval = (F32)atof(yytext); return(FP_CONSTANT); }
{N}*"."{N}+({E})?{FS}?		{ yylval->fval = (F32)atof(yytext); return(FP_CONSTANT); }
{N}+"."{N}*({E})?{FS}?		{ yylval->fval = (F32)atof(yytext); return(FP_CONSTANT); }

L\"(\\.|[^\\"])*\"	{
		yylval->sval = parse_string(yytext);
		ERROR( yylloc, W_L_STRING );
		return(STRING_CONSTANT);
	}
\"(\\.|[^\\"])*\"	{ yylval->sval = parse_string(yytext); return(STRING_CONSTANT); }

"++"				{ return(INC_OP); }
"--"				{ return(DEC_OP); }
"+="				{ return(ADD_ASSIGN); }
"-="				{ return(SUB_ASSIGN); }
"*="				{ return(MUL_ASSIGN); }
"/="				{ return(DIV_ASSIGN); }
"%="				{ return(MOD_ASSIGN); }
";"					{ return(';'); }
"{"					{ return('{'); }
"}"					{ return('}'); }
","					{ return(','); }
"="					{ return('='); }
"("					{ return('('); }
")"					{ return(')'); }
"-"					{ return('-'); }
"+"					{ return('+'); }
"*"					{ return('*'); }
"/"					{ return('/'); }
"%"					{ return('%'); }
"@"					{ return('@'); }
":"					{ return(':'); }
">"					{ return('>'); }
"<"					{ return('<'); }
"]"					{ return(']'); }
"["					{ return('['); }
"=="				{ return(EQ);  }
"!="				{ return(NEQ);  }
">="				{ return(GEQ);  }
"<="				{ return(LEQ);  }
"&"					{ return('&');  }
"|"					{ return('|');  }
"^"					{ return('^');  }
"~"					{ return('~');  }
"!"					{ return('!');  }
"&&"				{ return(BOOLEAN_AND);	}
"||"				{ return(BOOLEAN_OR);	}
"<<"				{ return(SHIFT_LEFT);  }
">>"				{ return(SHIFT_RIGHT); }

^[ \t\f\v\r]*"#"	{ LLOC_STEP(); if (skip_preproc) { BEGIN PREPROC; } }
<PREPROC>[^\n]*\\\r?\n	{ LLOC_LINES(1); LLOC_STEP(); }
<PREPROC>[^\n]*\n	{ BEGIN 0; LLOC_LINES(1); LLOC_STEP(); }


\n					{ LLOC_LINES(1); LLOC_STEP(); }
.					{ LLOC_STEP(); /* ignore bad characters */ }

%%

char *parse_string(char *input) {
	char *str = new char[(strlen(input) - 2) * 2 + 1];
	char *yp  = input + 1;
	char *sp  = str;
	while ( *yp ) {
		if ( *yp == '\\' ) {
			switch ( *++yp ) { 
					case 'n':  *sp++ = '\n'; break;
					case 't':
						*sp++ = ' ';
						*sp++ = ' ';
						*sp++ = ' ';
						*sp++ = ' ';
						break;
					case '\\': *sp++ = '\\'; break;
					case '"':  *sp++ = '"';  break;
					case '\n':
						/* thorny! OK if preprocessor in use;
                                                   causes a syntax disaster if not */
						/* TODO: Emit warning, but how? */
						*sp++ = '\n';
						break;
					default:
						/* TODO: Emit warning, but how? */
						*sp++ = *yp;
						break;
			}
			yp++;
		} else {
			*sp++ = *yp++;
		}
	}
	*--sp = 0;
	return str;
}

