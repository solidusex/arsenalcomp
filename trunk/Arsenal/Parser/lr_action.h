/*
 * The Arsenal Library
 * Copyright (c) 2009 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.It is provided "as is" without express 
 * or implied warranty.
 *
 */


#ifndef __PARSER_LR_ACTION__
#define __PARSER_LR_ACTION__

#include "parser.h"
#include "parser_in.h"
#include "lr_item.h"

AR_NAMESPACE_BEGIN

/************************************************语法分析表*********************************************************************/

typedef enum
{
		PSR_SHIFT,
		PSR_REDUCE,
		PSR_ACCEPT,
		PSR_ERROR
}psrActionType_t;

typedef struct __action_record_tag psrAction_t;

typedef struct __action_record_tag
{
		psrActionType_t			type;
		
		size_t					shift_to;
		const psrRule_t			*rule;
		size_t					delim;
		

		
		size_t					prec;
		size_t					reduce_count;/*规约时用到，产生式如果为%Epsilon，则count为0*/
		psrAction_t				*next;
		
}psrAction_t;

extern const psrAction_t	*PSR_ErrorAction;


typedef struct __parser_action_tag
{
		psrSymbList_t	term_set;
		psrSymbList_t	nonterm_set;

		int_t			*goto_tbl;
		size_t			goto_row;
		size_t			goto_col;

		size_t			row;
		size_t			col;
		psrAction_t		**actions;
		psrSymbList_t	*expected_set;
}psrActionTable_t;




const psrAction_t*		PSR_GetAction(const psrActionTable_t *tbl, size_t state, const psrSymb_t *symb);
int_t					PSR_GetState(const psrActionTable_t *tbl, size_t state, const psrSymb_t *symb);

const psrSymbList_t*	PSR_GetExpectedSymb(const psrActionTable_t *tbl, size_t state);


psrActionTable_t*		PSR_CreateActionTable_SLR(const psrGrammar_t *grammar);
psrActionTable_t*		PSR_CreateActionTable_LR1(const psrGrammar_t *grammar);
psrActionTable_t*		PSR_CreateActionTable_LALR(const psrGrammar_t *grammar);

void					PSR_DestroyActionTable(psrActionTable_t *tbl);



void PSR_PrintActionTable(const psrActionTable_t *tbl, const psrGrammar_t *grammar, size_t width, arString_t *str);
void PSR_ReportConflict(const psrActionTable_t *tbl, const psrGrammar_t *grammar, arString_t *str);

size_t PSR_CountConflict(const psrActionTable_t *tbl);













AR_NAMESPACE_END




#endif

