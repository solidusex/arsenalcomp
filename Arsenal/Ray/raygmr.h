
#if(0)

%name	delim 		:	"[ \r\n\t]"			
				;
%name	comment		:	"/\*([^\*]|\*+[^\*/])*\*+/"	
				;
%name	comment_line	:	"//[^\n]*\n"			
				;
%name	digit			:	"[0-9]"				
				;
%name	number		:	"{digit}+"				
				;
%name	letter		:	"[A-Z_a-z]"
				;


%name	int_type_suffix	:	"((u|U)?(l|L))|((u|U)(l|L)?)"
				;


%name	octal_escape	:	"(\\[0-3][0-7][0-7])|(\\[0-7][0-7])|(\\[0-7])"
				;


%name hex_digit		:	"[0-9a-fA-F]"
				;

%name hex_literal 	:	"0(x|X){hex_digit}+{int_type_suffix}?"
				;


%name dec_literal	:	"(0|[1-9][0-9]*){int_type_suffix}?"
				;


%name oct_literal	:	"0[0-7]+{int_type_suffix}?"
				;



%name	exponet		:	"(e|E)(\+|\-)?[0-9]+"
				;

%name float_type_suffix	:	"(f|F|d|D)"
					;

%name float_literal	:	"([0-9]+\.[0-9]*{exponet}?{float_type_suffix}?)"
				;




%name skip_lexem	: 	"{delim}|{comment}|{comment_line}"
				;

/*  '"'  == \x22,  '\'' == \x27 */

%name	escape_seq		:	"(\\(b|t|n|f|r|\x22|\x27|\\))|{octal_escape}"
				;

%name	string_literal	: 	"(\x22({escape_seq}|[^\x22\\])*\x22)"
				;

%name	char_literal	:	"\x27({escape_seq}|[^\x27\\])\x27"
				;


%token %skip 	:	"{delim}+|{comment}+|{comment_line}+",1;





%token	STRING_LITERAL	:	"{string_literal}";


%token	OCT_CONSTANT	:	"{oct_literal}";
%token	HEX_CONSTANT	:	"{hex_literal}";
%token	DEC_CONSTANT	:	"{dec_literal}";
%token	FLOAT_CONSTANT	:	"{float_literal}";
%token	CHAR_CONSTANT	:	"{char_literal}";

%token	IDENTIFIER		:	"{letter}({letter}|{digit})*";


%name		keyword_lhd	:	"[A-Z_a-z0-9]"
					;


%token	"auto"	:	'"auto"(?!{keyword_lhd})'	,1		;		//AUTO
%token	"extern"	:	'"extern"(?!{keyword_lhd})'	,1		;	//EXTERN
%token	"register"	:	'"register"(?!{keyword_lhd})'	,1		;	//REGISTER
%token	"volatile"	:	'"volatile"(?!{keyword_lhd})'	,1		;	//VOLATILE
%token	"static"	:	'"static"(?!{keyword_lhd})'		,1		;	//STATIC


%token	"switch"	:	'"switch"(?!{keyword_lhd})'		,1		;	//SWITCH
%token	"for"		:	'"for"(?!{keyword_lhd})'	,1		;	//FOR
%token	"goto"	:	'"goto"(?!{keyword_lhd})'	,1		;	//GOTO
%token	"return"	:	'"return"(?!{keyword_lhd})'		,1		;	//RETURN
%token	"do"		:	'"do"(?!{keyword_lhd})'		,1		;	//DO
%token	"while"	:	'"while"(?!{keyword_lhd})'	,1		;	//WHILE
%token	"if"		:	'"if"(?!{keyword_lhd})'		,1		;	//IF
%token	"else"	:	'"else"(?!{keyword_lhd})'	,1		;	//ELSE
%token	"enum"	:	'"enum"(?!{keyword_lhd})'	,1		;	//ENUM					;
%token	"continue"	:	'"continue"(?!{keyword_lhd})'	,1		;	//CONTINUE
%token	"default"	:	'"default"(?!{keyword_lhd})'	,1		;	//DEFAULT
%token	"case"	:	'"case"(?!{keyword_lhd})'	,1		;	//CASE
%token	"break"	:	'"break"(?!{keyword_lhd})'	,1		;	//BREAK


%token	"const"	:	'"const"(?!{keyword_lhd})'	,1		;	//CONST
%token	"sizeof"	:	'"sizeof"(?!{keyword_lhd})'		,1		;	//SIZEOF



%token	"struct"	:	'"struct"(?!{keyword_lhd})'		,1		;	//STRUCT
%token	"union"	:	'"union"(?!{keyword_lhd})'		,1		;	//UNION
%token	"typedef"	:	'"typedef"(?!{keyword_lhd})'	,1		;	//TYPEDEF


%token	"void"	:	'"void"(?!{keyword_lhd})'		,1		;	//VOID
%token	"signed"	:	'"signed"(?!{keyword_lhd})'		,1	;	//SIGNED
%token	"unsigned"	:	'"unsigned"(?!{keyword_lhd})'	,1		;	//UNSIGNED

%token	"byte"	:	'"byte"(?!{keyword_lhd})'	,1		;	//BYTE
%token	"int"		:	'"int"(?!{keyword_lhd})'	,1		;	//INT
%token	"long"	:	'"long"(?!{keyword_lhd})'		,1		;	//LONG
%token	"char"	:	'"char"(?!{keyword_lhd})'	,1		;	//CHAR
%token	"short"	:	'"short"(?!{keyword_lhd})'		,1		;	//SHORT
%token	"double"	:	'"double"(?!{keyword_lhd})'	,1		;	//DOUBLE
%token	"float"	:	'"float"(?!{keyword_lhd})'	,1		;	//FLOAT



%token	"..."		:	'"..."'		,1		;	//ELLIPSIS
%token	">>="		:	'">>="'		,2		;	//RIGHT_ASSIGN
%token	"<<="		:	'"<<="'		,2		;	//LEFT_ASSIGN



%token	"+="		:	'"+="'		,1		;	//ADD_ASSIGN
%token	"-="		:	'"-="'		,1		;	//SUB_ASSIGN
%token	"*="		:	'"*="'		,1		;	//MUL_ASSIGN
%token	"/="		:	'"/="'		,1		;	//DIV_ASSIGN
%token	"%="		:	'"%="'		,1		;	//MOD_ASSIGN
%token	"&="		:	'"&="'		,1		;	//AND_ASSIGN


%token	"^="		:	'"^="'		,1		;	//XOR_ASSIGN
%token	"|="		:	'"|="'		,1		;	//OR_ASSIGN
%token	">>"		:	'">>"'		,1		;	//RIGHT_OP
%token	"<<"		:	'"<<"'		,1		;	//LEFT_OP
%token	"++"		:	'"++"'		,1		;	//INC_OP
%token	"--"		:	'"--"'		,1		;	//DEC_OP


%token	"->"		:	'"->"'		,1		;	//PTR_OP
%token	"&&"		:	'"&&"'		,1		;	//AND_OP
%token	"||"		:	'"||"'		,1		;	//OR_OP

%token	"<="		:	'"<="'		,1		;	//LE_OP
%token	">="		:	'">="'		,1		;	//GE_OP
%token	"=="		:	'"=="'		,1		;	//EQ_OP
%token	"!="		:	'"!="'		,1		;	//NE_OP

%token	";"		:	'";"'			,0		;	//;
%token	"{"		:	'"{"'			,0		;	//{
%token	"}"		:	'"}"'			,0		;	//}
%token	","		:	'","'			,0		;	//,
%token	":"		:	'":"'			,0		;	//:
%token	"="		:	'"="'			,0		;	//=
%token	"("		:	'"("'			,0		;	//(
%token	")"		:	'")"'			,0		;	//)
%token	"["		:	'"["'			,0		;	//[
%token	"]"		:	'"]"'			,0		;	//]
%token	"."		:	'"."'			,0		;	//.
%token	"&"		:	'"&"'			,0		;	//&
%token	"!"		:	'"!"'			,0		;	//!
%token	"~"		:	'"~"'			,0		;	//~
%token	"-"		:	'"-"'			,0		;	//-
%token	"+"		:	'"+"'			,0		;	//+
%token	"*"		:	'"*"'			,0		;	//*
%token	"/"		:	'"/"'			,0		;	///
%token	"%"		:	'"%"'			,0		;	//%
%token	"<"		:	'"<"'			,0		;	//<
%token	">"		:	'">"'			,0		;	//>
%token	"^"		:	'"^"'			,0		;	//^
%token	"|"		:	'"|"'			,0		;	//|
%token	"?"		:	'"?"'			,0		;	//?

%token	TYPE_ID	:	'^'			,0		;	//ռλ��
%token	FAKE_EOI	:	'^'			,0		;	//ռλ��


%left		"||"				;
%left		"&&"				;
%left		"|"				;
%left		"^"				;
%left		"&"				;
%left		"=="	"!="			;
%left		"<"	"<="	">"	">="	;
%left		"<<"	">>"			;
%left		"+"	"-"			;
%left		"*"	"/"	"%"		;

%noassoc	IF_STMT_PREC						;
%noassoc	IF_STMT_ELSE_STMT_PREC				;


translation_unit	:	external_declaration
				|	translation_unit external_declaration
				;


constant			:	OCT_CONSTANT
				|	HEX_CONSTANT
				|	DEC_CONSTANT
				|	FLOAT_CONSTANT
				|	CHAR_CONSTANT
				|	STRING_LITERAL
				;

primary_expression	:	IDENTIFIER
				|	constant
				|	"(" expression ")"
				;

postfix_expression	:	primary_expression
				|	postfix_expression "[" expression "]"
				|	postfix_expression "(" ")"
				|	postfix_expression "(" argument_expression_list ")"
				|	postfix_expression "." IDENTIFIER
				|	postfix_expression "->" IDENTIFIER
				|	postfix_expression "++"
				|	postfix_expression "--"
				;

argument_expression_list	:	assignment_expression
					|	argument_expression_list "," assignment_expression
					;

unary_expression	:	postfix_expression
				|	"++" unary_expression
				|	"--" unary_expression
				|	unary_operator cast_expression
				|	"sizeof" unary_expression
				|	"sizeof" "(" type_name ")"
				;

unary_operator	:	"&"
			|	"*"
			|	"+"
			|	"-"
			|	"~"
			|	"!"
			;

cast_expression	:	unary_expression
			|	"(" type_name ")" cast_expression
			;


binary_expression	:	binary_expression	"+"	binary_expression
				|	binary_expression	"-"	binary_expression
				|	binary_expression	"*"	binary_expression
				|	binary_expression	"/"	binary_expression
				|	binary_expression	"%"	binary_expression
				|	binary_expression	"<<"	binary_expression
				|	binary_expression	">>"	binary_expression
				|	binary_expression	"<"	binary_expression
				|	binary_expression	"<="	binary_expression
				|	binary_expression	">"	binary_expression
				|	binary_expression	">="	binary_expression
				|	binary_expression	"=="	binary_expression
				|	binary_expression	"!="	binary_expression
				|	binary_expression	"&"	binary_expression
				|	binary_expression	"^"	binary_expression
				|	binary_expression	"|"	binary_expression
				|	binary_expression	"&&"	binary_expression
				|	binary_expression	"||"	binary_expression
				|	cast_expression
				;



constant_expression		:	binary_expression
					|	binary_expression	"?"	expression	":"	constant_expression
					;

assignment_expression		:	constant_expression
					|	unary_expression assignment_operator assignment_expression
					;

assignment_operator		:	"="
					|	"*="
					|	"/="
					|	"%="
					|	"+="
					|	"-="
					|	"<<="
					|	">>="
					|	"&="
					|	"^="
					|	"|="
					;

expression			:	assignment_expression
					|	expression "," assignment_expression
					;

declaration			:	declaration_specifiers ";"
					|	declaration_specifiers init_declarator_list ";"
					|	declaration_specifiers  error ";"
					|	error ";"
					;


storage_class_specifier	:	"typedef"
					|	"extern"
					|	"static"
					|	"auto"
					|	"register"
					;

declaration_specifiers	:	storage_class_specifier
					|	storage_class_specifier declaration_specifiers
					|	type_specifier
					|	type_qualifier
					|	type_specifier declaration_specifiers
					|	type_qualifier declaration_specifiers
					;

init_declarator_list		:	init_declarator
					|	init_declarator_list "," init_declarator
					;

init_declarator			:	declarator
					|	declarator "=" initializer
					;



type_specifier			:	"void"
					|	"byte"
					|	"char"
					|	"short"
					|	"int"
					|	"long"
					|	"float"
					|	"double"
					|	"signed"
					|	"unsigned"
					|	struct_or_union_specifier
					|	enum_specifier
					|	TYPE_ID
					;



struct_or_union_specifier	:	struct_or_union IDENTIFIER "{" struct_declaration_list "}"
					|	struct_or_union "{" struct_declaration_list "}"
					|	struct_or_union IDENTIFIER
					|	struct_or_union "{" error "}"
					|	struct_or_union error ";"
					;

struct_or_union			:	"struct"
					|	"union"
					;

struct_declaration_list	:	struct_declaration
					|	struct_declaration_list struct_declaration
					;

struct_declaration		:	specifier_qualifier_list struct_declarator_list ";"
					;

specifier_qualifier_list	:	type_specifier specifier_qualifier_list
					|	type_specifier
					|	type_qualifier specifier_qualifier_list
					|	type_qualifier
					;

struct_declarator_list	:	struct_declarator
					|	struct_declarator_list "," struct_declarator
					;

struct_declarator		:	declarator
					|	":" constant_expression
					|	declarator ":" constant_expression
					;

enum_specifier			:	"enum" "{" enumerator_list "}"
					|	"enum" IDENTIFIER "{" enumerator_list "}"
					|	"enum" IDENTIFIER
					;

enumerator_list			:	enumerator
					|	enumerator_list "," enumerator
					;

enumerator				:	IDENTIFIER
					|	IDENTIFIER "=" constant_expression
					;

type_qualifier			:	"const"
					| 	"volatile"		
					;

declarator				:	pointer direct_declarator
					|	direct_declarator
					;

direct_declarator		:	IDENTIFIER
					|	"(" declarator ")"
					|	direct_declarator "[" constant_expression "]"
					|	direct_declarator "[" "]"
					|	direct_declarator "(" parameter_type_list ")"
					|	direct_declarator "(" ")"

					|	direct_declarator "(" error ")"
					|	direct_declarator "[" error "]"
					;

pointer				:	"*"
					|	"*" type_qualifier_list
					|	"*" type_qualifier_list pointer
					|	"*" pointer
					;

type_qualifier_list		:	type_qualifier
					|	type_qualifier_list type_qualifier
					;

parameter_type_list		:	parameter_list
					|	parameter_list "," "..."
					;

parameter_list			:	parameter_declaration
					|	parameter_list "," parameter_declaration
					;

parameter_declaration		:	declaration_specifiers declarator
					|	declaration_specifiers abstract_declarator
					|	declaration_specifiers
					;
		

type_name				:	specifier_qualifier_list
					|	specifier_qualifier_list abstract_declarator
					;

abstract_declarator		:	pointer
					|	direct_abstract_declarator
					|	pointer direct_abstract_declarator
					;


direct_abstract_declarator	:	"(" abstract_declarator ")"
					|	"[" "]"
					|	"[" constant_expression "]"
					|	direct_abstract_declarator "[" "]"
					|	direct_abstract_declarator "[" constant_expression "]"
					|	"(" ")"
					|	"(" parameter_type_list ")"
					|	direct_abstract_declarator "(" ")"
					|	direct_abstract_declarator "(" parameter_type_list ")"

					|	"(" error ")"
					|	"["	error "]"
					;

initializer			:	assignment_expression
					|	"{" initializer_list "}"
					|	"{" initializer_list "," "}"
					|	"{" error "}"
					;

initializer_list	:	initializer
					|	initializer_list "," initializer
					;

statement			:	labeled_statement
					|	compound_statement
					|	expression_statement
					|	selection_statement
					|	iteration_statement
					|	jump_statement
					;

labeled_statement	:	"case" constant_expression ":" statement
					|	"default" ":" statement
					|	IDENTIFIER ":" statement
					;

compound_statement		:	"{" "}"
					|	"{" statement_list "}"
					|	"{" declaration_list "}"
					|	"{" declaration_list statement_list "}"
					|	"{" error  "}"
					|	"{" declaration_list error "}"
					|	"{" declaration_list statement_list error "}"
					;

declaration_list	:	declaration
					|	declaration_list declaration
					;

statement_list			:	statement
					|	statement_list statement
					;

expression_statement		:	";"
					|	expression ";"
					;

selection_statement		:	"if" "(" expression ")" statement	%prec IF_STMT_PREC
					|	"if" "(" expression ")" statement "else" statement	%prec IF_STMT_ELSE_STMT_PREC
					|	"switch" "(" expression ")" statement
					;

iteration_statement		:	"while" "(" expression ")" statement
					|	"do" statement "while" "(" expression ")" ";"
					|	"for" "(" expression_statement expression_statement ")" statement
					|	"for" "(" expression_statement expression_statement expression ")" statement
					;


jump_statement		:	"goto" IDENTIFIER ";"
					|	"continue" ";"
					|	"break" ";"
					|	"return" ";"
					|	"return" expression ";"
					;

external_declaration		:	function_definition
					|	declaration
					;

function_definition		:	declaration_specifiers declarator compound_statement
					;


#endif