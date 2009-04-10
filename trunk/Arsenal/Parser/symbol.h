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
#ifndef __PARSER_SYMBOL_H__
#define __PARSER_SYMBOL_H__

#include "parser.h"

AR_NAMESPACE_BEGIN



/*
���з����������ⶨ�壬�����Զ�ΪNO_ASSOC�����ϱ�ʾ�ڷ�����ͻʱ������ѡ��ͬ���ȼ��Ĺ�Լ�����ҽ�������෴������
expr -> id | expr '-' expr;

�� ����Ϊ expr - expr - exprʱ������޽���ԣ���Ϊ��ͻ��������'-'Ϊ���ϣ������ȹ���ǰ���expr-expr���ҽ�Ϸ�֮

*/

typedef enum 
{
		PSR_ASSOC_NOASSOC,
		PSR_ASSOC_LEFT,
		PSR_ASSOC_RIGHT
}psrAssocType_t;/*�ս�������*/

typedef enum
{
		PSR_TERM,
		PSR_NONTERM
}psrSymbType_t;

typedef struct __parser_symbol_tag psrSymb_t;

typedef struct __parser_symbol_tag
{
		const wchar_t	*name;
		psrSymbType_t	type;
		size_t			prec;/*���ȼ������Ϊ0�������ȼ������з���Ĭ�϶���0*/
		psrAssocType_t	assoc;/*�����*/
		
		int_t			ref_count;
		uint_t			hash_code;

		uint_t			val;/*������ս��������Ϊ�ս��ID������Ϊ0*/

}psrSymb_t;


const psrSymb_t*		PSR_CreateSymb(const wchar_t *name, psrSymbType_t t, size_t prec, psrAssocType_t assoc, uint_t val);
const psrSymb_t*		PSR_CopyNewSymb(const psrSymb_t *sour);
void					PSR_DestroySymb(const psrSymb_t *symb);
int_t					PSR_CompSymb(const psrSymb_t *l, const psrSymb_t *r);


/***************************************symbol_list***************************************************/

typedef struct __parser_symbol_list_tag
{
		const psrSymb_t **lst;
		size_t			count;
		size_t			cap;	
}psrSymbList_t;

void				PSR_InitSymbList(psrSymbList_t *symb_lst);
void				PSR_UnInitSymbList(psrSymbList_t *symb_lst);
void				PSR_ClearSymbList(psrSymbList_t *symb_lst);

void				PSR_CopySymbList(psrSymbList_t *dest, const psrSymbList_t *sour);

void				PSR_InsertToSymbList(psrSymbList_t *symb_lst, const psrSymb_t *symb);
const psrSymb_t*	PSR_IndexOfSymbList(const psrSymbList_t *symb_lst, size_t idx);
int_t				PSR_FindFromSymbList(const psrSymbList_t *symb_lst, const psrSymb_t* symb);

int_t				PSR_BSearchFromSymbList(const psrSymbList_t *symb_lst, const psrSymb_t* symb);
void				PSR_SortSymbList(psrSymbList_t *symb_lst);

/***************************************symbol_table***************************************************/

typedef struct __parser_symbol_record_tag
{
		const psrSymb_t							*symb;
		struct __parser_symbol_record_tag		*next;
}psrSymbRec_t;

typedef struct __parser_symbol_table_tag
{
		psrSymbRec_t	**lst;
		size_t			bucket_size;
		size_t			item_count;
}psrSymbTbl_t;

void					PSR_InitSymbTbl(psrSymbTbl_t *tbl, size_t count);

void					PSR_UnInitSymbTbl(psrSymbTbl_t *tbl);

void					PSR_ClearSymbTbl(psrSymbTbl_t	*tbl);

bool					PSR_InsertToSymbTbl(psrSymbTbl_t *tbl, const psrSymb_t *symb);

const psrSymb_t*		PSR_FindFromSymbTbl(const psrSymbTbl_t *tbl, const psrSymb_t *symb);

void					PSR_SymbTblToSymbList(const psrSymbTbl_t *tbl,  psrSymbList_t *lst);

void					PSR_CopySymbTbl(psrSymbTbl_t *dest, const psrSymbTbl_t *sour);




typedef struct __parser_symb_iter_tag
{
		size_t				bucket;
		const psrSymbRec_t	*curr;
}psrSymbTblIter_t;


psrSymbTblIter_t		PSR_FirstFromSymbTbl(const psrSymbTbl_t *tbl);
bool					PSR_NextFromSymbTbl(const psrSymbTbl_t *tbl, psrSymbTblIter_t *iter);




/********************************************************************************************************/
void			PSR_PrintSymbol(const psrSymb_t *symb, bool detail, arString_t *str);
void			PSR_PrintSymbolList(const psrSymbList_t *lst, bool detail, arString_t *str);
void			PSR_PrintSymbolTable(const psrSymbTbl_t *tbl, bool detail, arString_t *str);

AR_NAMESPACE_END


#endif

