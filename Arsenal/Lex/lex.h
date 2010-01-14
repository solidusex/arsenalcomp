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


#ifndef __LEX_H__
#define __LEX_H__

#include "../Common/common.h"
#include "match.h"

AR_NAMESPACE_BEGIN





void	LEX_Init();
void	LEX_UnInit();



typedef struct __lex_action_tag
{
		size_t			value;
		size_t			priority;
		bool_t			is_skip;
}lexAction_t;




/*typedef struct __lex_tag lex_t;*/

typedef struct __lex_tag 
{
		struct __rgx_name_set_tag		*name_tbl;
		
		struct __prog_set_tag			*prog_set;
		
		arIOCtx_t						io_ctx;
}lex_t;


lex_t*	LEX_Create(const arIOCtx_t *io);
void	LEX_Destroy(lex_t *lex);
void	LEX_ResetIOContext(lex_t *lex, const arIOCtx_t *io);

bool_t	LEX_InsertName(lex_t *lex, const wchar_t *name, const wchar_t *expr);
bool_t	LEX_InsertRule(lex_t *lex, const wchar_t *rule, const lexAction_t *action);

bool_t	LEX_RemoveByName(lex_t *lex, const wchar_t *name);
bool_t	LEX_RemoveByValue(lex_t *lex, size_t value);

bool_t	LEX_Insert(lex_t *lex, const wchar_t *input);
bool_t	LEX_GenerateTransTable(lex_t *lex);
void	LEX_Clear(lex_t *lex);




bool_t LEX_Match(lex_t *lex, lexMatch_t *match, lexToken_t *tok);







AR_NAMESPACE_END


















#endif
