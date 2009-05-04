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

#ifndef __PARSER_H__
#define __PARSER_H__

#include "..\Common\common.h"
#include "..\Lex\lex.h"

AR_NAMESPACE_BEGIN







typedef void			psrNode_t;
typedef lexToken_t		psrToken_t;

typedef psrNode_t*		(AR_STDCALL *psrTermFunc_t)(const psrToken_t *tok,void *ctx);
typedef psrNode_t*		(AR_STDCALL *psrRuleFunc_t)(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx);

typedef void			(AR_STDCALL *psrFreeFunc_t)(psrNode_t *node, void *ctx);

typedef void			(AR_STDCALL *psrErrorFunc_t)(const psrToken_t *tok, const wchar_t *expected[], size_t count, void *ctx);




/**********************************��������������PSR�庯��֮ǰ���ã�������Ϊδ����***************************/

void	PSR_Init();
void	PSR_UnInit();



#define PSR_EOI_TOKVAL			0/*end of input���ŵĴʷ�ֵ*/
#define PSR_EPSILON_TOKVAL		1
#define PSR_LALR_TOKVAL			2
#define PSR_ERROR_TOKVAL		3
#define PSR_DEFPREC_TOKVAL		4

#define PSR_MIN_TOKENVAL		256		/*��С�ս��value*/


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
		PSR_LR0,
		PSR_SLR = PSR_LR0,
		PSR_LR1,
		PSR_LALR
}psrLRItemType_t;

typedef psrLRItemType_t psrModeType_t;






typedef struct __parser_context_tag
{
		psrFreeFunc_t	free_f;
		psrErrorFunc_t	error_f;
		void			*ctx;
}psrCtx_t;


typedef struct __parser_grammar_tag		psrGrammar_t;
typedef struct __parser_action_tag		psrActionTable_t;
typedef struct __parser_symbol_tag		psrSymb_t;
typedef struct __parser_stack_tag		psrStack_t;
typedef struct __parser_node_stack		psrNodeStack_t;
typedef struct __term_table_tag			psrTermInfoTbl_t;
typedef struct __expected_message_tag	psrExpectedMsg_t;

typedef struct __parser_tag
{
		psrGrammar_t			*grammar;
		psrActionTable_t		*tbl;
		psrStack_t				*state_stack;
		psrNodeStack_t			*node_stack;
		psrTermInfoTbl_t		*term_tbl;
		
		bool					is_repair;
		bool					is_accepted;
		
		psrExpectedMsg_t		*msg_set;
		size_t					msg_count;
		
		psrCtx_t				user;
}parser_t;



parser_t* PSR_CreateParser(const psrGrammar_t *grammar, psrModeType_t type, const psrCtx_t *ctx);
 
void	  PSR_DestroyParser(parser_t *parser);

void	  PSR_Clear(parser_t *parser);

/*
		���ڲ�����һ��������ķ������Ե�EOI�����ӵ�stack��ʱ��ֻ���ܳ��ִ�����߳�Ϊ����״̬��EOI��Զ���ᱻSHIFT��parser��
*/

bool PSR_AddToken(parser_t *parser, const psrToken_t *tok);


psrNode_t* PSR_GetResult(parser_t *parser);/*��״̬Ϊaccepted֮��ſ��Ե���*/

bool	PSR_IsAccepted(const parser_t *parser);

bool	PSR_IsInError(const parser_t *parser);

void	PSR_ClearError(parser_t *parser);


AR_NAMESPACE_END




#endif
