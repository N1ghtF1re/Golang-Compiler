%option noyywrap

%{
#include "lexer.h"

%}
/*
DIGITS
*/
decimal_digit 		[0-9]
octal_digit   		[0-7]
hex_digit     		[0-9a-fA-F]

/*
INTEGER LITERALS
*/
decimal_lit 		[1-9]({decimal_digit})*
octal_lit   		0({octal_digit})*
hex_lit			0([xX]){hex_digit}+
int_lit			{decimal_lit}|{octal_lit}|{hex_lit}

/*
FLOAT LITERALS
*/
decimals  		{decimal_digit}+
exponent  		([eE])([\+-]?){decimals}
float_lit 		({decimals}(\.){decimals}?{exponent}?)|{decimals}{exponent}|(\.){decimals}{exponent}?

/*
CHARS
*/
unicode_char		(.)
unicode_char_wo_quote	[^\n\"]
newline			(\n)
unicode_letter		[:aplha:]
unicode_digit		[:digit:]

/*
ID
*/
identifier		[_A-Za-z]([_A-Za-z0-9])*

/*
RUNE LITERALS
*/
escaped_char		(\\)([abfnrtv\\'\"])
big_u_value		(\\)(U)({hex_digit}){8}
little_u_value		(\\)(u)({hex_digit}){4}
hex_byte_value		(\\)(x)({hex_digit}){2}
octal_byte_value	(\\)({octal_digit}){3}
byte_value		{octal_byte_value}|{hex_byte_value}
unicode_value		{unicode_char_wo_quote}|{little_u_value}|{big_u_value}|{escaped_char}
rune_lit		(')({unicode_value}|{byte_value})(')

/*
STRING LITERALS
*/
raw_string_lit        	(`)({unicode_char}|{newline})*(`)
interpreted_string_lit	(\")({unicode_value}|{byte_value})*(\")
string_lit		{raw_string_lit}|{interpreted_string_lit}

/*
TYPES
*/
numeric_type		((u)?(int)(8|16|32|64)?)|((float)(32|64))|byte|rune|((complex)(64|128))|uintptr
boolean_type		boolean
string_type		string
pimitive_type		{numeric_type}|{boolean_type}|{string_type}

/*TypeLit		{ArrayType}|{StructType}|{PointerType}|{FunctionType}|{InterfaceType}|{SliceType}|{MapType}|{ChannelType}*/


%% 		
import([ ])*\([^\)]*\)				
import(.)				

(package)				{	
						printf(" PACKAGE ");
						tokenList_add_withoutvalue(PACKAGE);
					}

{string_lit}				{	
						printf("(%s STRING)", yytext);
						tokenList_add_string_lit(yytext);
					}
{rune_lit}				{	
						printf("(%s CHAR)", yytext);
						tokenList_add_char_lit(yytext);
					}
{pimitive_type}				{	
						printf("(%s PRIMTYPE)", yytext);
						tokenList_add_prim_type(yytext);
					}

func					{
						printf(" FUNC ");
						tokenList_add_withoutvalue(FUNC);
					}
var					{
						printf(" VAR ");
						tokenList_add_withoutvalue(VAR);
					}
for					{
						printf(" FOR ");
						tokenList_add_withoutvalue(FOR);
					}
if					{
						printf(" IF ");
						tokenList_add_withoutvalue(IF);
					}
else					{
						printf(" ELSE ");
						tokenList_add_withoutvalue(ELSE);
					}

\(					{
						printf(" OPEN ");
						tokenList_add_withoutvalue(OPEN);
					}
\)					{
						printf(" CLOSE ");
						tokenList_add_withoutvalue(CLOSE);
					}

\}					{
						printf(" BLOPEN ");
						tokenList_add_withoutvalue(BLOPEN);
					}
\{					{
						printf(" BLCLOSE ");
						tokenList_add_withoutvalue(BLCLOSE);
					}

{float_lit}				{	
						printf("(%s FLOAT)", yytext);
						tokenList_add_float_lit(yytext);
					}
{int_lit}				{	
						printf("(%s INT)", yytext);
						tokenList_add_int_lit(yytext);
					}
{identifier}				{
						printf("(%s ID)", yytext);
						tokenList_add_id(yytext);
					}

!=					{
						printf(" NEQUAL ");
						tokenList_add_withoutvalue(NEQUAL);
					}
==					{
						printf(" EQUAL ");
						tokenList_add_withoutvalue(EQUAL);
					}
:=					{
						printf(" INIT ");
						tokenList_add_withoutvalue(INIT);
					}
=					{
						printf(" ASSIGN ");
						tokenList_add_withoutvalue(ASSIGN);
					}
!					{
						printf(" NOT ");
						tokenList_add_withoutvalue(NOT);
					}
\+					{
						printf(" PLUS ");
						tokenList_add_withoutvalue(PLUS);
					}
-					{
						printf(" MINUS ");
						tokenList_add_withoutvalue(MINUS);
					}
\*					{
						printf(" MUL ");
						tokenList_add_withoutvalue(MUL);
					}
\/					{
						printf(" DIV ");
						tokenList_add_withoutvalue(DIV);
					}

\.					{
						printf(" DOT ");
						tokenList_add_withoutvalue(DOT);
					}
,					{
						printf(" COMMA ");
						tokenList_add_withoutvalue(COMMA);
					}
[\t]					printf("");
[ ]					printf("");
[\n]					printf("");
.					printf("ERROR %s ERROR", yytext);

%%




