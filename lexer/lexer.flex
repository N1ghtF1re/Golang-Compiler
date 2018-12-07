%option noyywrap

%{
enum Token {
	ID,
	STRING_LIT,
	CHAR_LIT,
	FLOAT_LIT,
	INT_LIT,
	PRIM_TYPE,
	FUNC,
	VAR,
	FOR,
	IF,
	ELSE,
	OPEN,
	CLOSE,
	BLOPEN,
	BLCLOSE,
	NEQUAL,
	EQUAL,
	INIT,
	ASIGN,
	NOT,
	PLUS,
	MINUS,
	MUL,
	DIV,
	DOT,
	COMMA
};

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
package(.)*				
import([ ])*\([^\)]*\)				
import(.)				


{string_lit}				printf("STRING ");
{rune_lit}				printf("CHAR ");
{pimitive_type}				printf("PRIMTYPE ");

func					printf("FUNC ");
var					printf("VAR ");
for					printf("FOR ");
if					printf("IF ");
else					printf("ELSE ");

\(					printf("OPEN ");
\)					printf("CLOSE ");

\}					printf("BLOPEN ");
\{					printf("BLCLOSE ");

{float_lit}				printf("FLOAT ");
{int_lit}				printf("INT ");
{identifier}				printf("ID ");

!=					printf("NEQUAL");
==					printf("EQUAL ");
:=					printf("INIT ");
=					printf("ASSIGN ");
!					printf("NOT ");
\+					printf("PLUS ");
-					printf("MINUS ");
\*					printf("MUL ");
\/					printf("DIV ");

\.					printf("DOT ");
,					printf("COMMA ");


%%

int main() {
	yylex();
	
}
