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
#ifndef __PARSER_IN_H__
#define __PARSER_IN_H__


#include "parser.h"





typedef struct __parser_symbol_tag psrSymb_t;

/*
���з����������ⶨ�壬�����Զ�ΪNO_ASSOC�����ϱ�ʾ�ڷ�����ͻʱ������ѡ��ͬ���ȼ��Ĺ�Լ�����ҽ�������෴������
expr -> id | expr '-' expr;

�� ����Ϊ expr - expr - exprʱ������޽���ԣ���Ϊ��ͻ��������'-'Ϊ���ϣ������ȹ���ǰ���expr-expr���ҽ�Ϸ�֮

*/
typedef enum 
{
		PSR_ASSO_LEFT,
		PSR_ASSO_RIGHT,
		PSR_ASSO_NOASSO
}psrAssoType_t;/*�ս�������*/

typedef struct __parser_symbol_tag
{
		wchar_t			name[PARSER_MAXNAME];
		bool_t			non_term;/*�Ƿ�Ϊ���ս��*/
		size_t			priority;/*���ȼ������Ϊ<PSR_MIN_PRIOVAL��Ϊ�����ȼ�*/
		psrAssoType_t	asso;/*�����*/
		psrSymb_t		*next;

		union{
				size_t	val;/*���ս����˵Ϊֵ������֮*/
				size_t	id;
		};
}psrSymb_t;


/*����Ϊ�ս��*/
extern const psrSymb_t	*PSR_EpsilonSymb;/*��ʾ��*/
extern const psrSymb_t	*PSR_EOISymb;   /*��ʾ�����β����*/
extern const psrSymb_t	*PSR_LALRSymb; /*��ʾΪ���������ַ�ֵ�����е��ַ�������LALR�﷨������ʱʹ��*/

/*���¶�Ϊ���ս��*/
extern const psrSymb_t	*PSR_StartSymb;/*��һ�����ţ���ת��Ϊ %Start -> gmr->head[0]*/

psrSymb_t* PSR_CopyNewSymb(const psrSymb_t *sour);
psrSymb_t* PSR_DestroySymb(psrSymb_t *sour);
void	PSR_DestroySymbList(psrSymb_t *list);

psrSymb_t* PSR_FindSymb(psrSymb_t *start, const psrSymb_t *key);
void	PSR_InsertSymb(psrSymb_t **psymbs, const psrSymb_t *item);
bool_t PSR_InsertSymb_Unique(psrSymb_t **psymbs, const psrSymb_t *item);

psrSymb_t* PSR_CopySymbList(const psrSymb_t *sour);



int PSR_CompSymb(const psrSymb_t *left, const psrSymb_t *right);
bool_t PSR_IsNullable(const psrSymb_t *symb);

psrError_t PSR_SetSymb(psrSymb_t *symb, const wchar_t *name, bool_t is_nt, size_t prio, psrAssoType_t asso, size_t val);

/***********************for lr itemset*****************************************/

int				PSR_CompSymbList(const psrSymb_t *left, const psrSymb_t *right);
bool_t			PSR_InsertSymbByOrder_Unique(psrSymb_t **psymbs, const psrSymb_t *item);
psrSymb_t*		PSR_CopyNewSymbList(const psrSymb_t *sour);
		
size_t			PSR_CountSymbList(const psrSymb_t *lst);
const psrSymb_t* PSR_IndexOfSymbList(const psrSymb_t *lst, size_t index);

















/***********************************************************************************************************************/

typedef struct __parser_grammar_tag
{
		psrSymb_t		**head;/*���ս����ɵĲ���ʽ��ͷ���ϣ�����A->a|b��õ� head[1] = <A> && head[2] == <A>*/
		psrSymb_t		**body;
		
		size_t			count;
		size_t			cap;
		
		psrSymb_t		**nonterm_set;/*���ս������, ����A->a|b��õ� nonterm_set[1] = <A>*/
		psrSymb_t		**first;/*first����,first[i]Ϊnonterm_set[i]�ķ��ս����first����*/
		psrSymb_t		**follow;/*follow���ϣ�ͬ��*/
		size_t			non_count;

		psrSymb_t		**term_set;/*�ս������*/
		size_t			term_count;
		size_t			term_cap;
		
		psrSymb_t		*symb_list;
}psrGrammar_t;



psrGrammar_t*	PSR_BuildGrammar(const wchar_t *input);

void			PSR_DestroyGrammar(psrGrammar_t* grammar);

const psrSymb_t* PSR_GetFirstSetBySymb(const psrGrammar_t *grammar,const psrSymb_t *symb);

const psrSymb_t* PSR_GetFollowSetBySymb(const psrGrammar_t *grammar,const psrSymb_t *symb);

const psrSymb_t* PSR_GetRuleHeadByRuleID(const psrGrammar_t *grammar,size_t id);
const psrSymb_t* PSR_GetRuleBodyByRuleID(const psrGrammar_t *grammar,size_t id);
const psrSymb_t* PSR_GetTermSymbByValue(const psrGrammar_t *grammar,size_t value);

/***************************************LR Item && ItemSet****************************************************************/

typedef struct __parser_lr_item_tag psrLRItem_t;


typedef struct __parser_lr_item_tag
{
		size_t			rule_id;
		size_t			delim;
		
		psrSymb_t		*symb;/*���ڹ淶LR1����LALR1��ʹ��,�淶LR1����Զֻ��һ����LALR����N����N > 1������ʱ����ԶΪNULL*/
		
		psrLRItem_t		*next;
}psrLRItem_t;


int				PSR_CompLRItem(const psrLRItem_t *left, const psrLRItem_t *right);
bool_t			PSR_InsertLRItem_Unique(psrLRItem_t **dest, const psrLRItem_t *sour);
psrLRItem_t*	PSR_CopyNewLRItem(const psrLRItem_t *sour);

psrLRItem_t*	PSR_DestroyLRItem(psrLRItem_t *item);
void			PSR_DestroyLRItemList(psrLRItem_t *lst);

psrLRItem_t*	PSR_FindLRItem(psrLRItem_t *sour, const psrLRItem_t *key);
psrLRItem_t*	PSR_CopyNewLRItemList(const psrLRItem_t *sour);
int				PSR_CompLRItemList(const psrLRItem_t *left, const psrLRItem_t *right);

/*�Ƚ�left��right���������True�����left�е�����Ԫ�ض�������right��,�˺�������PSR_CompLRItem*/
bool_t			PSR_IsIncludesLRItemList(const psrLRItem_t *left, const psrLRItem_t *right);

/************************************************�﷨������*********************************************************************/

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
		size_t					priority;
		psrAction_t				*next;
		

		union{
				size_t			shift_to;
				size_t			reduce_id;
		};
		
		psrLRItem_t				item;/*������õ���*/
		size_t					reduce_count;/*��Լʱ�õ�������ʽ���Ϊ%Epsilon����countΪ0*/
}psrAction_t;




typedef struct __parser_action_tag
{
		psrSymb_t		*term_set;
		size_t			term_count;
		psrAction_t		**actions;

		size_t			row;
		size_t			col;


		psrSymb_t		*non_term_set;
		size_t			nonterm_count;

		int				*goto_tbl;
		size_t			goto_row;
		size_t			goto_col;
		
}psrActionTable_t;

void					PSR_DestroyActionTable(psrActionTable_t *tbl);
const psrAction_t*		PSR_GetAction(const psrActionTable_t *tbl, size_t state, const psrSymb_t *symb);
int						PSR_GetState(const psrActionTable_t *tbl, size_t state, const psrSymb_t *symb);



psrActionTable_t* PSR_BuildActionTable_SLR(const psrGrammar_t *grammar);
psrActionTable_t* PSR_BuildActionTable_LR1(const psrGrammar_t *grammar);
psrActionTable_t* PSR_BuildActionTable_LALR(const psrGrammar_t *grammar);




/***********************************************Parser*********************/

typedef struct __parser_tag 
{
		psrActionTable_t		*tbl;
		psrGrammar_t			*grammar;
		lex_t					*lex;

		psrLeaf_func			leaf_func;
		psrNode_func			node_func;
		psrDestroy_func			destroy_func;
}parser_t;















#endif


