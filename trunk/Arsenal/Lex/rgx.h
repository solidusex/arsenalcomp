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

#ifndef __REGEX_REGEX_IN_H__
#define __REGEX_REGEX_IN_H__


#include "../Common/common.h"
#include "match.h"

AR_NAMESPACE_BEGIN


/*NameSet*/
#define	AR_RGX_MAXNAME			256


typedef struct __rgx_name_tag			rgxName_t;
typedef struct __rgx_name_set_tag		rgxNameSet_t;

typedef struct __rgx_node_tag			rgxNode_t;

struct __rgx_name_tag
{
		wchar_t			name[AR_RGX_MAXNAME];
		rgxNode_t		*node;
};

struct __rgx_name_set_tag
{
		rgxName_t		*name;
		size_t			count;
		size_t			cap;
};


void					RGX_InitNameSet(rgxNameSet_t	*set);
void					RGX_UnInitNameSet(rgxNameSet_t	*set);
bool_t					RGX_InsertToNameSet(rgxNameSet_t	*set, const wchar_t	*name, rgxNode_t *node);
bool_t					RGX_RemoveFromNameSet(rgxNameSet_t	*set, const wchar_t	*name);
const rgxNode_t*		RGX_FindFromNameSet(const rgxNameSet_t	*set, const wchar_t *name);
void					RGX_ClearNameSet(rgxNameSet_t *set);


/*NameSet end*/


/*CharSet*/
typedef struct __rgx_char_range_tag		rgxCharRange_t;
typedef struct __rgx_charset_tag		rgxCharSet_t;

struct __rgx_char_range_tag
{
		wchar_t									beg;
		wchar_t									end;
		rgxCharRange_t							*next;
};

struct __rgx_charset_tag
{
		bool_t					is_neg;
		rgxCharRange_t			*range;
};


void RGX_InitCharSet(rgxCharSet_t *cset);
void RGX_UnInitCharSet(rgxCharSet_t *cset);
void RGX_InsertRangeToCharSet(rgxCharSet_t *cset, const rgxCharRange_t *new_range);
void RGX_ReverseNegativeCharSet(rgxCharSet_t *cset);
void RGX_CopyCharSet(rgxCharSet_t *dest, const rgxCharSet_t *sour);
/*CharSetEnd*/



/*rgx node*/

typedef enum
{
		RGX_BEGIN_T,
		RGX_END_T,
		RGX_CSET_T,

		RGX_CAT_T,
		RGX_BRANCH_T,

		RGX_STAR_T,
		RGX_QUEST_T,
		RGX_PLUS_T,
		
		RGX_LOOKAHEAD_T,
		
		RGX_FINAL_T
}rgxNodeType_t;


struct __rgx_node_tag
{
		rgxNodeType_t		type;
		rgxNode_t			*left;
		rgxNode_t			*right;
		
		union{
				struct {
						wchar_t	beg;
						wchar_t end;
				}range;
				bool_t					negative_lookahead;
				bool_t					non_greedy;
				size_t					final_val;
		};
};



rgxNode_t*		RGX_CreateNode(rgxNodeType_t type);

rgxNode_t*		RGX_CopyNode(const rgxNode_t *node);

void			RGX_DestroyNode(rgxNode_t *node);

void			RGX_InsertToNode(rgxNode_t *root, rgxNode_t *node);


void			RGX_ToString(const rgxNode_t *node, arString_t *str);

void			RGX_CorrectTree(rgxNode_t *root);



/*rgx node end*/



/*parser*/
typedef struct __rgx_result_tag	rgxResult_t;
typedef struct __rgx_error_tag	rgxError_t;

struct __rgx_error_tag
{
		const wchar_t	*pos;
};


struct __rgx_result_tag
{
		rgxNode_t*		node;
		const wchar_t	*next;
		rgxError_t		err;
};


rgxResult_t	RGX_ParseExpr(const wchar_t *expr, const rgxNameSet_t *name_set);

/*parser end*/




/*rgx instruction*/


typedef enum
{
		RGX_NOP_I,
		RGX_CHAR_I,
		RGX_BEGIN_I,
		RGX_END_I,
		RGX_LOOKAHEAD_BEG_I,
		RGX_LOOKAHEAD_END_I,
		RGX_JMP_I,
		RGX_BRANCH_I,
		RGX_MATCH_I
}rgxInsType_t;

static const wchar_t *RGX_INS_NAME[] = 
{
		L"nop",
		L"char",
		L"match_begin",
		L"match_end",
		L"lookahead_begin",
		L"lookahead_end",
		L"jmp",
		L"branch",
		L"match"
};

typedef struct __regex_instruction_tag	rgxIns_t;

struct __regex_instruction_tag
{
		rgxInsType_t	opcode;
		
		rgxIns_t		*left;
		rgxIns_t		*right;

		int_t			mark;

		union{
				int_t			final;
				struct {
						wchar_t	beg;
						wchar_t end;
				}range;
				
				struct {
						bool_t			negative;
				}lookahead;
		};
};



typedef struct __regex_program_tag		rgxProg_t;



struct __regex_program_tag
{
		rgxIns_t		*start;
		size_t			count;
		
		rgxIns_t		*pc;
		int_t			mark;
};


void			RGX_InitProg(rgxProg_t *prog);
void			RGX_UnInitProg(rgxProg_t *prog);
void			RGX_Compile(rgxProg_t *prog, const rgxNode_t *tree);

void			RGX_PringProg(const rgxProg_t *prog, arString_t *str);



/*rgx instruction end*/



typedef struct __thread_tag
{
		rgxIns_t		*pc;
		const wchar_t	*sp;
		size_t			line;
		size_t			col;
}rgxThread_t;



typedef struct __thd_list_tag
{
		rgxThread_t		*lst;
		size_t			count;
		size_t			cap;
}rgxThreadList_t;

rgxThread_t		RGX_BuildThread(rgxIns_t *pc, const wchar_t *sp, size_t x, size_t y);
void			RGX_InitThreadList(rgxThreadList_t *lst);
void			RGX_UnInitThreadList(rgxThreadList_t *lst);
void			RGX_InsertToThreadList(rgxThreadList_t *lst, rgxThread_t thd);
void			RGX_SwapThreadList(rgxThreadList_t *l, rgxThreadList_t *r);
void			RGX_ClearThreadList(rgxThreadList_t *l);



bool_t RGX_Match(rgxProg_t *prog, lexMatch_t *match, lexToken_t *tok, rgxThreadList_t *curr, rgxThreadList_t *next);





AR_NAMESPACE_END


#endif
