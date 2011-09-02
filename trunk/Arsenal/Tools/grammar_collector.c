
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

#include "grammar_config.h"



AR_NAMESPACE_BEGIN

typedef enum
{
		CFG_STR_NAME,
		CFG_STR_TOKEN,
		CFG_STR_TOKEN_VALUE,
		CFG_STR_PREC
}cfgStrType_t;

typedef struct __config_parser_data_tag
{
		bool_t					has_error;
		cfgReport_t				*report;
		arStringTable_t			*name;
		arStringTable_t			*token;
		arStringTable_t			*token_val;
		arStringTable_t			*prec;
}cfgParserData_t;

static	AR_INLINE		void	__init_parser_data(cfgParserData_t		*psr_data, cfgReport_t	*report)
{
		AR_ASSERT(psr_data != NULL && report != NULL);

		AR_memset(psr_data, 0, sizeof(*psr_data));
		psr_data->has_error = false;
		psr_data->report = report;
		psr_data->name = AR_CreateStrTable(0);
		psr_data->token = AR_CreateStrTable(0);
		psr_data->token_val = AR_CreateStrTable(0);
		psr_data->prec  = AR_CreateStrTable(0);
}

static AR_INLINE		void	__uninit_parser_data(cfgParserData_t	*psr_data)
{
		AR_ASSERT(psr_data != NULL);

		AR_DestroyStrTable(psr_data->name);
		AR_DestroyStrTable(psr_data->token);
		AR_DestroyStrTable(psr_data->token_val);
		AR_DestroyStrTable(psr_data->prec);
		psr_data->has_error = false;
		AR_memset(psr_data, 0, sizeof(*psr_data));
}




static AR_INLINE		arStringTable_t*		__get_parser_data_tbl(cfgParserData_t	*psr_data, cfgStrType_t t)
{
		switch(t)
		{
		case CFG_STR_NAME:
				return psr_data->name;
		case CFG_STR_TOKEN:
				return psr_data->token;
		case CFG_STR_TOKEN_VALUE:
				return psr_data->token_val;
		case CFG_STR_PREC:
				return psr_data->prec;
		default:
				AR_ASSERT(false);
				return NULL;

		}
}


static AR_INLINE		void	__insert_str_to_parser_data(cfgParserData_t	*psr_data, const wchar_t *str, cfgStrType_t t)
{
		arStringTable_t	*tbl;
		AR_ASSERT(psr_data != NULL && str != NULL);
		tbl = __get_parser_data_tbl(psr_data, t);
		AR_GetString(tbl, str);
}


static AR_INLINE		bool_t	__has_str_in_parser_data(cfgParserData_t	*psr_data, const wchar_t *str, cfgStrType_t t)
{
		arStringTable_t	*tbl;
		AR_ASSERT(psr_data != NULL && str != NULL);
		tbl = __get_parser_data_tbl(psr_data, t);
		return AR_HasString(tbl, str);
}


static AR_INLINE		void	__check_name_from_parser_data(cfgParserData_t *psr_data, const wchar_t *name, size_t line, cfgStrType_t t)
{

		AR_ASSERT(psr_data != NULL && name != NULL);

		if(__has_str_in_parser_data(psr_data, name, t))
		{
				cfgReportInfo_t	msg;
				wchar_t	buf[1024];

				msg.warning.line = line;
				switch(t)
				{
				case CFG_STR_NAME:
						AR_swprintf(buf, 1024, L"Duplicate name definition %ls\r\n", name);
						break;
				case CFG_STR_TOKEN:
						AR_swprintf(buf, 1024, L"Duplicate token definition %ls\r\n", name);
						break;
				case CFG_STR_TOKEN_VALUE:
						AR_swprintf(buf, 1024, L"Duplicate token value definition %ls\r\n", name);
						break;
				case CFG_STR_PREC:
						AR_swprintf(buf, 1024, L"Duplicate prec definition %ls\r\n", name);
						break;
				default:
						AR_ASSERT(false);
				}
				
				msg.warning.msg = buf;
				msg.type = CFG_REPORT_WARNING_SYNTAX_T;
				psr_data->report->report_func(&msg, psr_data->report->report_ctx);
		}else
		{
				__insert_str_to_parser_data(psr_data, name, t);
		}

}


/*********************************************************************************************************************************************/




typedef enum
{
		CFG_EMPTY_T,
		CFG_LEXEME_T,
		CFG_NAME_T,
		CFG_TOKEN_T,
		CFG_PREC_T,
		CFG_PREDEF_T,
		CFG_START_T,
		CFG_RULE_T,
		CFG_NODE_LIST_T,
		CFG_CONFIG_T,
		CFG_ERROR_T
}cfgNodeType_t;

struct __cfg_node_tag
{
		union{
				cfgLexeme_t		lexeme;
				cfgName_t		name;
				cfgNodeList_t	lst;
				cfgPrec_t		prec;
				cfgToken_t		token;
				cfgRule_t		rule;
				cfgConfig_t		config;
				cfgStart_t		start;
				cfgPreDef_t		predef;
		};
		
		cfgNodeType_t	type;
		cfgNode_t		*next;
};


static const wchar_t *__cfg_lex_name[] =
{
		//L"	delim		= 	[\\f\\v\\t\\r\\n ]+",
		L"	delim		= 	[\\x{000B}\\x{0020}\\x{00A0}\\x{2028}\\x{2029} \\f\\v\\t\\r\\n]+",
		L"	letter		= 	[A-Z_a-z]",
		L"	digit		=	[0-9]",
		L"	number		=	0|[1-9]{digit}*",
		L"	name		=	{letter}({letter}|{digit})*",
		L"	lexeme		=	{name}|(\\\"([^\\\"\\n\\r])+\\\")|('([^'\\n\\r])+')",
		L"	comment		= 	/\\*([^\\*]|\\*+[^\\*/])*\\*+/",
		L"	comment_line	= //[^\\n\\r]*(\\n|$|\\r)",
		L"  skip_lexem		= {delim}",
		L"	comment_lexem	= {comment}|{comment_line}",
		L"  key_lookahead   = {skip_lexem}+|\"{\""
};




typedef struct  __cfg_lex_pattern_tag
{
		size_t			val;
		const wchar_t	*regex;
		bool_t			is_skip;
		size_t			prec;
}cfgLexPattern_t;


static const cfgLexPattern_t	__cfg_pattern[] =
{
		{CFG_LEXVAL_EOI,	L"$", false,2},
		{CFG_LEXVAL_DELIM, L"{skip_lexem}+", true, 1},
		
		{CFG_LEXVAL_COMMENT, L"{comment_lexem}", false, 1},

		{CFG_LEXVAL_ASSOC,	L"\"%\"(\"left\"|\"right\"|\"nonassoc\")(?={key_lookahead})", false,1},

		{CFG_LEXVAL_CODE,	L"\"%code\"(?={key_lookahead})", false,0},
		{CFG_LEXVAL_VALUE,	L"\"%value\"(?={key_lookahead})", false,0},
		
		{CFG_LEXVAL_SKIP,	L"\"%skip\"(?={key_lookahead})", false,0},
		{CFG_LEXVAL_START,	L"\"%start\"(?={key_lookahead})", false,0},
		{CFG_LEXVAL_NAME,	L"\"%name\"(?={key_lookahead})", false,0},
		{CFG_LEXVAL_TOKEN,	L"\"%token\"(?={key_lookahead})", false,0},
		{CFG_LEXVAL_PREC,	L"\"%prec\"(?={key_lookahead})", false,0},

		{CFG_LEXVAL_ACTION, L"\"%action\"(?={skip_lexem}+)", false, 0},
		{CFG_LEXVAL_ACTION_INS, L"\\{:[^\\u0]*?:\\}", false, 0},

		{CFG_LEXVAL_LEXEME,	L"{lexeme}", false,0},
		{CFG_LEXVAL_NUMBER,	L"{number}", false,0},

		{CFG_LEXVAL_DOT,		L"\".\"",	false,1},
		{CFG_LEXVAL_COMMA,		L"\",\"",	false,1},

		{CFG_LEXVAL_COLON,		L"\":\"",	false,1},
		{CFG_LEXVAL_SEMI,		L"\";\"",	false,1},

		{CFG_LEXVAL_OR,		L"\"|\"",	false,1}
};













typedef struct	__cfg_term_info_tag
{
		cfgLexValue_t	val;
		const wchar_t	*name;
}cfgTermInfo_t;


static const cfgTermInfo_t	__cfg_term[] =
{
		{CFG_LEXVAL_CODE, L"%code"},
		{CFG_LEXVAL_VALUE, L"%value"},
		{CFG_LEXVAL_SKIP, L"%skip"},
		{CFG_LEXVAL_START, L"%start"},
		{CFG_LEXVAL_NAME, L"%name"},
		{CFG_LEXVAL_TOKEN, L"%token"},
		{CFG_LEXVAL_PREC, L"%prec"},
		{CFG_LEXVAL_ACTION, L"%action"},
		{CFG_LEXVAL_ACTION_INS, L"action_ins"},
		{CFG_LEXVAL_ASSOC, L"assoc"},
		{CFG_LEXVAL_LEXEME, L"lexeme"},
		{CFG_LEXVAL_NUMBER, L"number"},

		{CFG_LEXVAL_DOT, L"."},
		{CFG_LEXVAL_COMMA, L","},
		{CFG_LEXVAL_COLON, L":"},
		{CFG_LEXVAL_SEMI, L";"},
		{CFG_LEXVAL_OR,   L"|"},
		{CFG_LEXVAL_FAKE_EOI, L"#"}
};

/*
typedef struct __cfg_prec_tag
{
		size_t	line;

		psrAssocType_t	assoc;
		size_t			prec_level;

		size_t			*prec_tok_val;
		wchar_t			**prec_tok_set;
		size_t			count;
		size_t			cap;
}cfgPrec_t;
*/

static void CFG_InitPrec(cfgPrec_t *prec)
{
		AR_ASSERT(prec != NULL);
		AR_memset(prec, 0, sizeof(*prec));
}


static void CFG_UnInitPrec(cfgPrec_t *prec)
{
		size_t i;
		AR_ASSERT(prec != NULL);

		for(i = 0; i < prec->count; ++i)
		{
				AR_ASSERT(prec->prec_tok_set[i] != NULL);
				AR_DEL(prec->prec_tok_set[i]);
		}

		if(prec->prec_tok_set) AR_DEL(prec->prec_tok_set);
		if(prec->prec_tok_val) AR_DEL(prec->prec_tok_val);
		AR_memset(prec, 0, sizeof(*prec));
}

static void CFG_InsertTokenToPrec(cfgPrec_t *prec, const wchar_t *prec_tok, size_t prec_tok_val)
{
		AR_ASSERT(prec != NULL && prec_tok != NULL);

		if(prec->count == prec->cap)
		{
				prec->cap = (prec->cap + 4)*2;
				prec->prec_tok_set = AR_REALLOC(wchar_t*, prec->prec_tok_set, prec->cap);
				prec->prec_tok_val = AR_REALLOC(size_t, prec->prec_tok_val, prec->cap);
		}

		prec->prec_tok_set[prec->count] = AR_wcsdup(prec_tok);
		prec->prec_tok_val[prec->count] = prec_tok_val;
		prec->count++;
}


static void CFG_CopyPrec(cfgPrec_t *dest, const cfgPrec_t *sour)
{
		size_t i;
		CFG_UnInitPrec(dest);

		dest->assoc = sour->assoc;
		dest->prec_level = sour->prec_level;
		dest->line = sour->line;
		for(i = 0; i < sour->count; ++i)
		{
				CFG_InsertTokenToPrec(dest, sour->prec_tok_set[i], sour->prec_tok_val[i]);
		}
}

/*******************************************************************/
static void CFG_InitNodeList(cfgNodeList_t *lst)
{
		AR_ASSERT(lst != NULL);
		AR_memset(lst, 0, sizeof(*lst));
}


static void CFG_UnInitNodeList(cfgNodeList_t *lst)
{
		if(lst && lst->lst)
		{
				AR_DEL(lst->lst);
				AR_memset(lst, 0, sizeof(*lst));
		}
}

static void CFG_InsertToNodeList(cfgNodeList_t *lst, cfgNode_t *node)
{
		AR_ASSERT(lst != NULL && node != NULL);


		if(lst->count == lst->cap)
		{
				lst->cap = (lst->cap + 4)*2;
				lst->lst = AR_REALLOC(cfgNode_t*, lst->lst, lst->cap);
				
				/*lst->lst = (cfgNode_t**)AR_realloc(lst->lst, sizeof(cfgNode_t*) * lst->cap);*/
		}
		lst->lst[lst->count++] = node;
}




static void CFG_InitConfig(cfgConfig_t *cfg, cfgNodeList_t *name, cfgNodeList_t *token, cfgNodeList_t *prec, cfgNodeList_t *rule, cfgStart_t *start_rule, cfgNodeList_t *pre_def)
{
		size_t i;
		size_t tmp_tok_val = PARSER_MIN_TOKENVAL + 1;

		AR_ASSERT(cfg != NULL);
		
		AR_memset(cfg, 0, sizeof(*cfg));

		if(name)
		{
				/*cfg->name_cnt = name->count;*/
				cfg->name = name->count > 0 ? AR_NEWARR0(cfgName_t, name->count) : NULL;

				for(i = 0; i < name->count; ++i)
				{
						if(name->lst[i]->type == CFG_ERROR_T)
						{
								cfg->has_error = true;
						}else
						{
								cfg->name[cfg->name_cnt].line = name->lst[i]->name.line;
								cfg->name[cfg->name_cnt].name = AR_wcsdup(name->lst[i]->name.name);
								cfg->name[cfg->name_cnt].regex = AR_wcsdup(name->lst[i]->name.regex);
								cfg->name_cnt++;
						}
				}
		}


		if(token)
		{
				size_t k;


				/*cfg->tok_cnt = token->count + 1;*/

				/*cfg->tok = cfg->tok_cnt > 0 ? AR_NEWARR0(cfgToken_t, cfg->tok_cnt + 1) : NULL;*/

				cfg->tok = AR_NEWARR0(cfgToken_t, token->count + 1);

				for(i = 0; i < token->count; ++i)
				{
						if(token->lst[i]->type == CFG_ERROR_T)
						{
								cfg->has_error = true;
						}else
						{
								cfg->tok[cfg->tok_cnt] = token->lst[i]->token;
								cfg->tok[cfg->tok_cnt].name = token->lst[i]->token.name != NULL ? AR_wcsdup(token->lst[i]->token.name) : NULL;
								cfg->tok[cfg->tok_cnt].regex = AR_wcsdup(token->lst[i]->token.regex);
								cfg->tok[cfg->tok_cnt].action_line = token->lst[i]->token.action_line;
								cfg->tok[cfg->tok_cnt].action_name = token->lst[i]->token.action_name != NULL ? AR_wcsdup(token->lst[i]->token.action_name) : NULL;
								cfg->tok[cfg->tok_cnt].action_ins = token->lst[i]->token.action_ins != NULL ? AR_wcsdup(token->lst[i]->token.action_ins) : NULL;

								if(token->lst[i]->token.code_name == NULL)
								{
										cfg->tok[cfg->tok_cnt].code_name = NULL;
								}else
								{
										cfg->tok[cfg->tok_cnt].code_name = AR_wcsdup(token->lst[i]->token.code_name);
										cfg->tok[cfg->tok_cnt].is_assigned_code_name = true;
								}

								cfg->tok_cnt++;
						}
				}

				for(i = 0; i < cfg->tok_cnt ; ++i)
				{
						if(cfg->tok[i].tokval == 0)
						{
RE_CHECK_POINT:
								for(k = 0; k < cfg->tok_cnt; ++k)
								{
										if(cfg->tok[k].tokval == tmp_tok_val)
										{
												tmp_tok_val++;
												k = 0;
												goto RE_CHECK_POINT;
										}
								}
								cfg->tok[i].tokval = tmp_tok_val++;
						}
				}


				/*CFG_LEXVAL_EOI*/
				cfg->tok[cfg->tok_cnt ].is_skip = false;
				cfg->tok[cfg->tok_cnt ].lex_prec = 2;
				cfg->tok[cfg->tok_cnt ].line = 0;
				cfg->tok[cfg->tok_cnt ].tokval = 0;
				cfg->tok[cfg->tok_cnt ].name = AR_wcsdup(L"CFG_LEXVAL_EOI");
				cfg->tok[cfg->tok_cnt ].regex = AR_wcsdup(L"$");

				cfg->tok_cnt++;


				for(i = 0; i < cfg->tok_cnt ; ++i)
				{
						if(cfg->tok[i].code_name == NULL)
						{
								cfg->tok[i].code_name = AR_vtow(L"%Id", cfg->tok[i].tokval);
						}
				}


		}


		if(prec)
		{
				/*cfg->prec_cnt = prec->count;*/
				cfg->prec = prec->count > 0 ? AR_NEWARR0(cfgPrec_t, prec->count) : NULL;

				for(i = 0; i < prec->count; ++i)
				{

						if(prec->lst[i]->type == CFG_ERROR_T)
						{
								cfg->has_error = true;

						}else
						{
								size_t k;
								cfgPrec_t *dest = &cfg->prec[cfg->prec_cnt];
								CFG_InitPrec(dest);

								CFG_CopyPrec(dest, &prec->lst[i]->prec);

								for(k = 0; k < dest->count; ++k)
								{
										size_t x;
										for(x = 0; x < cfg->tok_cnt; ++x)
										{
												if(cfg->tok[x].is_skip)continue;

												AR_ASSERT(cfg->tok[x].name != NULL);
												if(AR_wcscmp(dest->prec_tok_set[k], cfg->tok[x].name) == 0)
												{
														dest->prec_tok_val[k] = cfg->tok[x].tokval;
														break;
												}
										}

										if(dest->prec_tok_val[k] == 0)
										{
												dest->prec_tok_val[k] = tmp_tok_val++;
										}

								}

								dest->prec_level = i + 1;
								cfg->prec_cnt++;
						}
				}
		}


		if(rule)
		{
				/*cfg->rule_cnt = rule->count;*/

				cfg->rule = rule->count > 0 ? AR_NEWARR0(cfgRule_t, rule->count) : NULL;


				for(i = 0; i < rule->count; ++i)
				{
						if(rule->lst[i]->type == CFG_ERROR_T)
						{
								cfg->has_error = true;
						}else
						{
								cfg->rule[cfg->rule_cnt].line = rule->lst[i]->rule.line;

								cfg->rule[cfg->rule_cnt].lhs = AR_wcsdup(rule->lst[i]->rule.lhs);
								cfg->rule[cfg->rule_cnt].rhs = AR_wcsdup(rule->lst[i]->rule.rhs);

								if(rule->lst[i]->rule.prec_tok)
								{
										cfg->rule[cfg->rule_cnt].prec_tok = AR_wcsdup(rule->lst[i]->rule.prec_tok);
								}
										
								cfg->rule[cfg->rule_cnt].action_line = rule->lst[i]->rule.action_line;

								if(rule->lst[i]->rule.action_name)
								{
										cfg->rule[cfg->rule_cnt].action_name = AR_wcsdup(rule->lst[i]->rule.action_name);
								}

								if(rule->lst[i]->rule.action_ins)
								{
										cfg->rule[cfg->rule_cnt].action_ins = AR_wcsdup(rule->lst[i]->rule.action_ins);
								}

								cfg->rule_cnt++;
						}
				}

		}


		if(start_rule != NULL)
		{
				cfg->start.line = start_rule->line;
				cfg->start.start_rule = AR_wcsdup(start_rule->start_rule);

		}else
		{
				cfg->start.start_rule = NULL;
				cfg->start.line = 0;
		}


		if(pre_def)
		{
				cfg->pre_def = pre_def->count > 0 ? AR_NEWARR0(cfgPreDef_t, pre_def->count) : NULL;
				cfg->predef_cnt = pre_def->count;

				for(i = 0; i < pre_def->count; ++i)
				{
						AR_ASSERT(pre_def->lst[i]->type == CFG_PREDEF_T);
						cfg->pre_def[i].name = AR_wcsdup(pre_def->lst[i]->predef.name);
						cfg->pre_def[i].code = AR_wcsdup(pre_def->lst[i]->predef.code);
						cfg->pre_def[i].line = pre_def->lst[i]->predef.line;
				}
		}

}


static void CFG_UnInitConfig(cfgConfig_t *cfg)
{
		size_t i;
		AR_ASSERT(cfg != NULL);

		for(i = 0; i < cfg->name_cnt; ++i)
		{
				AR_DEL(cfg->name[i].name);
				AR_DEL(cfg->name[i].regex);

		}

		if(cfg->name)AR_DEL(cfg->name);


		for(i = 0; i < cfg->tok_cnt; ++i)
		{
				if(cfg->tok[i].code_name)AR_DEL(cfg->tok[i].code_name);
				if(cfg->tok[i].name)AR_DEL(cfg->tok[i].name);
				if(cfg->tok[i].regex)AR_DEL(cfg->tok[i].regex);
				if(cfg->tok[i].action_name)AR_DEL(cfg->tok[i].action_name);
				if(cfg->tok[i].action_ins)AR_DEL(cfg->tok[i].action_ins);
				
		}

		if(cfg->tok)AR_DEL(cfg->tok);


		for(i = 0; i < cfg->prec_cnt; ++i)
		{
				CFG_UnInitPrec(&cfg->prec[i]);
				/*if(cfg->prec[i].prec_tok)AR_DEL(cfg->prec[i].prec_tok);*/
		}

		if(cfg->prec)AR_DEL(cfg->prec);

		if(cfg->start.start_rule)
		{
				AR_DEL(cfg->start.start_rule);
		}

		for(i = 0; i < cfg->rule_cnt; ++i)
		{
				AR_DEL(cfg->rule[i].lhs);
				AR_DEL(cfg->rule[i].rhs);

				if(cfg->rule[i].prec_tok)AR_DEL(cfg->rule[i].prec_tok);
				if(cfg->rule[i].action_name)AR_DEL(cfg->rule[i].action_name);
				if(cfg->rule[i].action_ins)AR_DEL(cfg->rule[i].action_ins);
		}

		if(cfg->rule)AR_DEL(cfg->rule);


		for(i = 0; i < cfg->predef_cnt; ++i)
		{
				if(cfg->pre_def[i].name)AR_DEL(cfg->pre_def[i].name);
				if(cfg->pre_def[i].code)AR_DEL(cfg->pre_def[i].code);
		}

		if(cfg->pre_def)AR_DEL(cfg->pre_def);

}

#if defined(AR_DEBUG)

		#define CFG_INIT_CFG_NODE_COUNT			0
#else
		#define CFG_INIT_CFG_NODE_COUNT			1000
#endif

static cfgNode_t		*__g_node_list = NULL;
static arSpinLock_t		__g_list_lock;

static cfgNode_t*		__get_cfg_node()
{
		cfgNode_t *res = NULL;

		if(__g_node_list == NULL)
		{
				res = AR_NEW(cfgNode_t);

		}else
		{
				AR_LockSpinLock(&__g_list_lock);

				if(__g_node_list == NULL)
				{
						res = AR_NEW(cfgNode_t);
				}else
				{
						res = __g_node_list;
						__g_node_list = __g_node_list->next;
				}

				AR_UnLockSpinLock(&__g_list_lock);
		}

		AR_memset(res, 0, sizeof(*res));
		return res;
}

static void		__put_cfg_node(cfgNode_t *node)
{
		AR_ASSERT(node != NULL);

		AR_LockSpinLock(&__g_list_lock);
		node->next = __g_node_list;
		__g_node_list = node;

		AR_UnLockSpinLock(&__g_list_lock);

}


static void __init_cfg_node_list()
{
		size_t i;
		AR_InitSpinLock(&__g_list_lock);
		__g_node_list	= NULL;

		for(i = 0; i < CFG_INIT_CFG_NODE_COUNT; ++i)
		{
				cfgNode_t *n = AR_NEW0(cfgNode_t);
				__put_cfg_node(n);
		}
}

static void __uninit_cfg_node_list()
{
		size_t count = 0;
		AR_UnInitSpinLock(&__g_list_lock);

		while(__g_node_list)
		{
				cfgNode_t		*tmp = __g_node_list->next;
				AR_DEL(__g_node_list);
				__g_node_list = tmp;
				count++;
		}


		{
				wchar_t buf[1024];
				AR_swprintf(buf, 1024, L"Total consume cfgNode_t == %Iu", count);
				AR_LOG(L"%ls\r\n", buf);
		}

}




static cfgNode_t* CFG_CreateNode(cfgNodeType_t type)
{
		cfgNode_t		*node;

		node = __get_cfg_node();

		node->type = type;
		node->next = NULL;

		if(node->type == CFG_NODE_LIST_T)
		{
				CFG_InitNodeList(&node->lst);
		}else if(node->type == CFG_PREC_T)
		{
				CFG_InitPrec(&node->prec);
		}
		return node;
}


static void CFG_DestroyNode(cfgNode_t *node)
{
		if(node)
		{

				switch(node->type)
				{
				case CFG_EMPTY_T:
				{
						break;
				}
				case CFG_LEXEME_T:
				{
						if(node->lexeme.lexeme)
						{
								AR_DEL(node->lexeme.lexeme);
						}
						break;
				}
				case CFG_NAME_T:
				{
						if(node->name.name)AR_DEL(node->name.name);
						if(node->name.regex)AR_DEL(node->name.regex);
						break;
				}
				case CFG_TOKEN_T:
				{
						if(node->token.name)AR_DEL(node->token.name);
						if(node->token.regex)AR_DEL(node->token.regex);
						if(node->token.code_name)AR_DEL(node->token.code_name);

						if(node->token.action_name)AR_DEL(node->token.action_name);
						if(node->token.action_ins)AR_DEL(node->token.action_ins);

						break;
				}
				case CFG_PREC_T:
				{
						CFG_UnInitPrec(&node->prec);
						break;
				}
				case CFG_START_T:
				{
						if(node->start.start_rule)
						{
								AR_DEL(node->start.start_rule);
						}
						break;
				}
				case CFG_RULE_T:
				{
						if(node->rule.action_name)
						{
								AR_DEL(node->rule.action_name);
						}

						if(node->rule.action_ins)
						{
								AR_DEL(node->rule.action_ins);
						}

						if(node->rule.lhs)AR_DEL(node->rule.lhs);
						if(node->rule.rhs)AR_DEL(node->rule.rhs);
						if(node->rule.prec_tok)AR_DEL(node->rule.prec_tok);

						break;
				}
				case CFG_NODE_LIST_T:
				{
						size_t i;
						for(i = 0; i < node->lst.count; ++i)
						{
								CFG_DestroyNode(node->lst.lst[i]);
						}
						CFG_UnInitNodeList(&node->lst);
						break;
				}
				case CFG_CONFIG_T:
				{
						CFG_UnInitConfig(&node->config);
						break;
				}
				case CFG_ERROR_T:
				{
						break;
				}
				case CFG_PREDEF_T:
				{
						if(node->predef.name)AR_DEL(node->predef.name);
						if(node->predef.code)AR_DEL(node->predef.code);
						break;
				}
				default:
				{
						AR_CHECK(false, L"Arsenal internal error : %hs\r\n", AR_FUNC_NAME);
				}
				}

				/*
				AR_DEL(node);
				*/
				__put_cfg_node(node);

		}
}







static psrNode_t* AR_STDCALL __build_leaf(const psrToken_t *tok,  void *ctx)
{
		cfgNode_t *node;
		cfgParserData_t	*parser_ctx;
		AR_ASSERT(tok->str_cnt > 0 && ctx != NULL);
		
		parser_ctx = (cfgParserData_t*)ctx;

		node = CFG_CreateNode(CFG_LEXEME_T);

		node->lexeme.lex_val = (cfgLexValue_t)tok->term_val;

		if(tok->term_val == CFG_LEXVAL_LEXEME && (tok->str[0] == L'"' || tok->str[0] == L'\''))
		{
				wchar_t *tmp, *p;
				AR_ASSERT(tok->str[tok->str_cnt-1] == L'"' || tok->str[tok->str_cnt-1] == L'\'');

				tmp = AR_wcsndup(tok->str + 1, tok->str_cnt-2);
				
				p = (wchar_t*)AR_wcstrim_space(tmp);
				p = AR_wcstrim_right_space(p);
				
				node->lexeme.lexeme = AR_wcsdup(p);

				if(AR_wcslen(node->lexeme.lexeme) == 0)
				{
						cfgReportInfo_t info;
						wchar_t buf[128];
						
						AR_swprintf(buf, 128, L"Empty lexeme : line = %Id", tok->line);

						info.type = CFG_REPORT_ERROR_LEX_T;
						info.lex_error.msg = buf;
						info.lex_error.tok = tok;
						parser_ctx->report->report_func(&info, parser_ctx->report->report_ctx);
						parser_ctx->has_error = true;
				}
				
				AR_DEL(tmp);

		}else if(tok->term_val == CFG_LEXVAL_ACTION_INS)
		{
				wchar_t *buf;
				
				AR_ASSERT(tok->str_cnt >= 4);
				buf = AR_wcsndup(tok->str, tok->str_cnt);
				
				AR_ASSERT(buf[0] == L'{' && buf[1] == L':');
				AR_ASSERT(buf[tok->str_cnt-1] == L'}' && buf[tok->str_cnt-2] == L':');

				buf[1] = L' ';
				buf[tok->str_cnt-2] = L' ';
				
				

				node->lexeme.lexeme = buf;
		}else if(tok->str_cnt > 0)
		{
				node->lexeme.lexeme = AR_wcsndup(tok->str, tok->str_cnt);
		}

		node->lexeme.line = tok->line;
		return (psrNode_t*)node;
}





/*
{ L"action_decl			:		%action lexeme action_ins",				__handle_action_decl,0},
{ L"action_decl			:		%action lexeme",						__handle_action_decl},
{ L"action_decl			:				",										NULL},
*/
static psrNode_t*		AR_STDCALL __handle_action_decl(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;		
		
		AR_ASSERT(count == 2 || count == 3);
		AR_ASSERT(ns[0]->type == CFG_LEXEME_T && ns[1]->type == CFG_LEXEME_T);

		AR_UNUSED(name);
		AR_UNUSED(ctx);
		
		res = CFG_CreateNode(CFG_NODE_LIST_T);
		
		CFG_InsertToNodeList(&res->lst, ns[1]);
		ns[1] = NULL;
		
		if(count == 3)
		{
				AR_ASSERT(ns[2] && ns[2]->type == CFG_LEXEME_T);
				CFG_InsertToNodeList(&res->lst, ns[2]);
				ns[2] = NULL;
		}
		return res;
}



/*
{ L"term_list			:		term_list term",								NULL},
{ L"term_list			:		term",											NULL},
*/

static psrNode_t*		AR_STDCALL __handle_term_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;

		AR_ASSERT((count == 1 || count == 2) && nodes != NULL);
		AR_UNUSED(name);
		AR_UNUSED(ctx);
		if(count == 1)
		{
				res = CFG_CreateNode(CFG_NODE_LIST_T);

				AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T);

				CFG_InsertToNodeList(&res->lst, ns[0]);
				ns[0] = NULL;
		}else
		{
				AR_ASSERT(ns[0] && ns[0]->type == CFG_NODE_LIST_T && ns[1] && ns[1]->type == CFG_LEXEME_T);

				res = ns[0];

				CFG_InsertToNodeList(&res->lst, ns[1]);

				ns[0] = ns[1] = NULL;
		}

		return res;
}




/*
{ L"rhs					:		term_list prec_decl action_decl",				__handle_rhs,0},
{ L"rhs					:		. prec_decl action_decl",						__handle_rhs,0},
*/


static psrNode_t*		AR_STDCALL __handle_rhs(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		size_t i;
		AR_ASSERT(count == 3 && nodes != NULL);
		AR_UNUSED(name);
		AR_UNUSED(ctx);

		AR_UNUSED(count);

		res = CFG_CreateNode(CFG_RULE_T);
		res->rule.lhs = NULL;
		res->rule.line = 0;
		res->rule.prec_tok = NULL;
		res->rule.rhs = AR_NEWARR0(wchar_t, 2048);
		res->rule.action_line = 0;
		res->rule.action_name = NULL;
		res->rule.action_ins = NULL;

		/*AR_ASSERT(ns[0] && ns[0]->type == CFG_NODE_LIST_T && ns[0]->lst.count > 0);*/
		AR_ASSERT(ns[0]);
		AR_ASSERT(ns[1] ? ns[1]->type == CFG_LEXEME_T : true);
		AR_ASSERT(ns[2] ? ns[2]->type == CFG_NODE_LIST_T : true);


		if(ns[0]->type == CFG_LEXEME_T && ns[0]->lexeme.lex_val == CFG_LEXVAL_DOT)
		{
				AR_wcscat((wchar_t*)res->rule.rhs, L" ");
				res->rule.line = ns[0]->lexeme.line;

		}else if(ns[0]->type == CFG_NODE_LIST_T)
		{
				for(i = 0; i < ns[0]->lst.count; ++i)
				{
						cfgNode_t *tmp = ns[0]->lst.lst[i];

						AR_ASSERT(tmp->type == CFG_LEXEME_T && tmp->lexeme.lex_val == (size_t)CFG_LEXVAL_LEXEME);

						AR_wcscat((wchar_t*)res->rule.rhs, tmp->lexeme.lexeme);
						AR_wcscat((wchar_t*)res->rule.rhs, L" ");

						if(i == 0)
						{
								res->rule.line = tmp->lexeme.line;
						}
				}

				
		}else
		{
				AR_ASSERT(false);
		}


		if(ns[1])
		{
				res->rule.prec_tok = ns[1]->lexeme.lexeme;
				ns[1]->lexeme.lexeme = NULL;
		}

		if(ns[2])
		{
				AR_ASSERT(ns[2]->lst.count == 1 || ns[2]->lst.count == 2);
				
				res->rule.action_line = ns[2]->lst.lst[0]->lexeme.line;
				res->rule.action_name = ns[2]->lst.lst[0]->lexeme.lexeme;
				ns[2]->lst.lst[0]->lexeme.lexeme = NULL;
				
				if(ns[2]->lst.count == 2)
				{
						res->rule.action_ins = ns[2]->lst.lst[1]->lexeme.lexeme;
						ns[2]->lst.lst[1]->lexeme.lexeme = NULL;
				}
		}

/*
		CFG_DestroyNode(ns[0]);
		CFG_DestroyNode(ns[1]);
		CFG_DestroyNode(ns[2]);
		ns[0] = ns[1] = ns[2] = NULL;
*/
		return res;

}




/*
{ L"rhs_list			: 		rhs_list  | rhs ",								NULL},
{ L"rhs_list			: 		rhs ",											NULL},
*/

static psrNode_t*		AR_STDCALL __handle_rhs_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;

		AR_ASSERT((count == 1 || count == 3) && nodes != NULL);
		AR_UNUSED(name);
		AR_UNUSED(ctx);
		if(count == 3)
		{
				AR_ASSERT(ns[0] && ns[0]->type == CFG_NODE_LIST_T && ns[1]->type == CFG_LEXEME_T && ns[1]->lexeme.lex_val == (size_t)CFG_LEXVAL_OR && ns[2] && ns[2]->type == CFG_RULE_T);

				res = ns[0];

				CFG_InsertToNodeList(&res->lst, ns[2]);

				CFG_DestroyNode(ns[1]);

				ns[0] = ns[1] = ns[2] = NULL;
		}else
		{
				AR_ASSERT(ns[0]->type == CFG_RULE_T);
				res = CFG_CreateNode(CFG_NODE_LIST_T);
				CFG_InsertToNodeList(&res->lst, ns[0]);
				ns[0] = NULL;
		}

		return res;


}



/*
{ L"rule_def			: 		lexeme : rhs_list ",							__handle_rule_def},
*/

static psrNode_t*		AR_STDCALL __handle_rule_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		size_t i;
		AR_ASSERT(count == 3);

		AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T && ns[1] && ns[1]->type == CFG_LEXEME_T && ns[2] && ns[2]->type == CFG_NODE_LIST_T);
		AR_ASSERT(ns[0]->lexeme.lexeme != NULL);

		AR_UNUSED(ctx);
		AR_UNUSED(name);
		AR_UNUSED(count);


		for(i = 0; i < ns[2]->lst.count; ++i)
		{
				cfgNode_t *tmp = ns[2]->lst.lst[i];

				AR_ASSERT(tmp->type == CFG_RULE_T);
				tmp->rule.lhs = AR_wcsdup(ns[0]->lexeme.lexeme);
				tmp->rule.line = ns[0]->lexeme.line;
		}

		res = ns[2];
		ns[2] = NULL;

		return res;
}





/*{ L"prec_def			:  		assoc lexeme ",								__handle_prec_def},*/
static psrNode_t*		AR_STDCALL __handle_prec_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		wchar_t			c;
		size_t			i;
		AR_ASSERT(count == 2);

		AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T &&  ns[0]->lexeme.lex_val == CFG_LEXVAL_ASSOC && ns[1] && ns[1]->type == CFG_NODE_LIST_T);
		AR_UNUSED(name);
		AR_UNUSED(count);

		res = CFG_CreateNode(CFG_PREC_T);
		res->prec.line = ns[0]->lexeme.line;

		c = ns[0]->lexeme.lexeme[1];

		if(c == L'n')
		{
				res->prec.assoc	= PARSER_ASSOC_NONASSOC;

		}else if(c == L'l')
		{
				res->prec.assoc	= PARSER_ASSOC_LEFT;

		}else if(c == L'r')
		{
				res->prec.assoc	= PARSER_ASSOC_RIGHT;
		}else
		{
				AR_ASSERT(false);
				AR_CHECK(false, L"Arsenal Tools Internal Error In : %hs\r\n", AR_FUNC_NAME);
		}

		for(i = 0; i < ns[1]->lst.count; ++i)
		{
				const cfgNode_t *node = ns[1]->lst.lst[i];
				AR_ASSERT(node->type == CFG_LEXEME_T);
				CFG_InsertTokenToPrec(&res->prec, node->lexeme.lexeme, 0);
				
				__check_name_from_parser_data((cfgParserData_t*)ctx, node->lexeme.lexeme, node->lexeme.line, CFG_STR_PREC);

		}
		return res;

}



/*
{ L"token_def			:  		%token %skip token_val_prec : lexeme token_val_prec token_value action_decl",	__handle_token_def,0},
{L"token_def			:  		%token lexeme token_val_prec : lexeme token_val_prec token_value action_decl",	__handle_token_def,0},
*/

static psrNode_t*		AR_STDCALL __handle_token_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;

		AR_ASSERT(count == 8);

		/*AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T && ns[1] && ns[1]->type == CFG_LEXEME_T && ns[2] && ns[2]->type == CFG_LEXEME_T && ns[4] && ns[4]->type == CFG_LEXEME_T);*/

		AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T
				&& ns[1] && ns[1]->type == CFG_LEXEME_T
				&& ns[3] && ns[3]->type == CFG_LEXEME_T
				&& ns[4] && ns[4]->type == CFG_LEXEME_T
				);

		AR_ASSERT((ns[2] == NULL) || (ns[2]->type == CFG_LEXEME_T));
		AR_ASSERT((ns[5] == NULL) || (ns[5]->type == CFG_LEXEME_T));

		AR_ASSERT((ns[6] == NULL) || (ns[6]->type == CFG_LEXEME_T));
		AR_ASSERT((ns[7] == NULL) || (ns[7]->type == CFG_NODE_LIST_T));

		AR_UNUSED(name);
		AR_UNUSED(count);

		res = CFG_CreateNode(CFG_TOKEN_T);

		res->token.line = ns[1]->lexeme.line;

		if(ns[1]->lexeme.lex_val == CFG_LEXVAL_SKIP)
		{
				res->token.is_skip = true;
				res->token.name = NULL;
		}else
		{
				__check_name_from_parser_data((cfgParserData_t*)ctx, ns[1]->lexeme.lexeme, ns[1]->lexeme.line, CFG_STR_TOKEN);

				res->token.is_skip = false;
				res->token.name = ns[1]->lexeme.lexeme;
				ns[1]->lexeme.lexeme = NULL;
		}

		res->token.regex = ns[4]->lexeme.lexeme;
		ns[4]->lexeme.lexeme = NULL;

		res->token.lex_prec = 0;

		if(ns[2])
		{
				AR_wtou(ns[2]->lexeme.lexeme, (uint_t*)&res->token.tokval, 10);
		}else
		{
				res->token.tokval = 0;
		}

		if(ns[5])
		{
				AR_wtou(ns[5]->lexeme.lexeme, (uint_t*)&res->token.lex_prec, 10);
		}

		if(ns[6])
		{
				res->token.code_name = AR_wcsdup(ns[6]->lexeme.lexeme);
				__check_name_from_parser_data((cfgParserData_t*)ctx, ns[6]->lexeme.lexeme, ns[6]->lexeme.line, CFG_STR_TOKEN_VALUE);
		}


		if(ns[7])
		{
				AR_ASSERT(ns[7]->lst.count == 1 || ns[7]->lst.count == 2);
				
				res->token.action_line = ns[7]->lst.lst[0]->lexeme.line;
				res->token.action_name = ns[7]->lst.lst[0]->lexeme.lexeme;
				ns[7]->lst.lst[0]->lexeme.lexeme = NULL;
				
				if(ns[7]->lst.count == 2)
				{
						res->token.action_ins = ns[7]->lst.lst[1]->lexeme.lexeme;
						ns[7]->lst.lst[1]->lexeme.lexeme = NULL;
				}
		}

#if(0)
		if(ns[6])
		{
				AR_ASSERT(ns[6]->lst.count > 0);
				res->token.code_name = AR_wcsdup(ns[6]->lst.lst[0]->lexeme.lexeme);

				__check_name_from_parser_data((cfgParserData_t*)ctx, ns[6]->lst.lst[0]->lexeme.lexeme, ns[6]->lst.lst[0]->lexeme.line, CFG_STR_TOKEN_VALUE);
		}
#endif

		

		return res;

}





/*
{ L"name_def 			:		%name lexeme : lexeme  ",								__handle_name_def},
*/

static psrNode_t*		AR_STDCALL __handle_name_def(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgParserData_t			*psr_data;
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;

		AR_ASSERT(count == 4);

		AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T
				&& ns[1] && ns[1]->type == CFG_LEXEME_T
				&& ns[2] && ns[2]->type == CFG_LEXEME_T
				&& ns[3] && ns[3]->type == CFG_LEXEME_T
				);

		AR_UNUSED(name);
		AR_UNUSED(count);

		res = CFG_CreateNode(CFG_NAME_T);




		res->name.line = ns[1]->lexeme.line;
		res->name.name = ns[1]->lexeme.lexeme;
		ns[1]->lexeme.lexeme = NULL;

		res->name.regex = ns[3]->lexeme.lexeme;
		ns[3]->lexeme.lexeme = NULL;

		psr_data = (cfgParserData_t*)ctx;

		__check_name_from_parser_data(psr_data, res->name.name, res->name.line, CFG_STR_NAME);
		
		return res;
}


/*
{ L"item				:		name_def ;",	__handle_item},
{ L"item				:		token_def ;",	__handle_item},
{ L"item				:		prec_def ;",	__handle_item},
{ L"item				:		rule_def ;",	__handle_item},
{ L"item				:		error ; ",	__handle_item},
{ L"item				:		; ",	__handle_item},
{ L"item				:		pre_code_decl ;",	 __handle_item,0},

*/

static psrNode_t*		AR_STDCALL __handle_item(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res = NULL;

		AR_ASSERT(count == 1 || count == 2);
		AR_UNUSED(ctx);
		AR_UNUSED(name);

		if(count == 2)
		{
				if(ns[0] == NULL)
				{
						res = CFG_CreateNode(CFG_ERROR_T);
				}else
				{
						res = ns[0];
						ns[0] = NULL;
				}

		}else
		{
				res = CFG_CreateNode(CFG_EMPTY_T);
		}
		return (psrNode_t*)res;
}

/*
{ L"item_list			:		item_list item",	__handle_item_list},
{ L"item_list			:		",	NULL},
*/

static psrNode_t*		AR_STDCALL __handle_item_list(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;

		AR_ASSERT(count == 2);

		AR_ASSERT((ns[0] == NULL) || (ns[0] && ns[0]->type == CFG_NODE_LIST_T));
		AR_ASSERT(ns[1]);
		AR_UNUSED(ctx);
		AR_UNUSED(name);
		AR_UNUSED(count);

		res = ns[0];
		if(res == NULL)
		{
				res = CFG_CreateNode(CFG_NODE_LIST_T);
		}
		ns[0] = NULL;

		if(ns[1]->type == CFG_NODE_LIST_T)
		{
				size_t i;
				for(i = 0; i < ns[1]->lst.count; ++i)
				{
						CFG_InsertToNodeList(&res->lst, ns[1]->lst.lst[i]);
						ns[1]->lst.lst[i] = NULL;
				}

		}else
		{
				CFG_InsertToNodeList(&res->lst, ns[1]);
				ns[1] = NULL;
		}


		return (psrNode_t*)res;
}

/*
{ L"start_def			:  		%start lexeme",							__handle_start_def,0},
*/

static psrNode_t*		AR_STDCALL __handle_start_def(psrNode_t **nodes, size_t count, const wchar_t * name_tmp, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;

		AR_ASSERT(count == 2);

		AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T && ns[0]->lexeme.lex_val == CFG_LEXVAL_START);
		AR_ASSERT(ns[1] && ns[1]->type == CFG_LEXEME_T);
		
		AR_UNUSED(ctx);
		AR_UNUSED(name_tmp);
		AR_UNUSED(count);

		res = CFG_CreateNode(CFG_START_T);
		res->start.line = ns[1]->lexeme.line;
		res->start.start_rule = AR_wcsdup(ns[1]->lexeme.lexeme);
		return res;

}

#if(0)
/*
{ L"pre_code_decl		:		action_ins",				__handle_pre_def,0},
*/
static psrNode_t*		AR_STDCALL __handle_pre_def(psrNode_t **nodes, size_t count, const wchar_t * name_tmp, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		size_t			len;
		AR_ASSERT(count == 1);

		AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T && ns[0]->lexeme.lex_val == CFG_LEXVAL_ACTION_INS);


		res = CFG_CreateNode(CFG_PREDEF_T);
		res->predef.line = ns[0]->lexeme.line;
		
		len = AR_wcslen(ns[0]->lexeme.lexeme);
		AR_ASSERT(len >= 2);
		
		if(len >= 2)
		{
				res->predef.code = AR_wcsndup(ns[0]->lexeme.lexeme+1, len - 2);
		}else
		{
				res->predef.code = AR_wcsdup(L"");
		}

		return res;

}
#endif


/*
{ L"pre_code_decl		:		%code pre_code_name action_ins",		__handle_pre_def,0},
*/
static psrNode_t*		AR_STDCALL __handle_pre_def(psrNode_t **nodes, size_t count, const wchar_t * name_tmp, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res;
		size_t			len;
		AR_ASSERT(count == 3);

		AR_ASSERT(ns[0] && ns[0]->type == CFG_LEXEME_T && ns[0]->lexeme.lex_val == CFG_LEXVAL_CODE);
		AR_ASSERT(ns[2] && ns[2]->type == CFG_LEXEME_T && ns[2]->lexeme.lex_val == CFG_LEXVAL_ACTION_INS);

		AR_UNUSED(ctx);
		AR_UNUSED(name_tmp);
		AR_UNUSED(count);

		res = CFG_CreateNode(CFG_PREDEF_T);
		res->predef.line = ns[0]->lexeme.line;
		
		len = AR_wcslen(ns[2]->lexeme.lexeme);
		AR_ASSERT(len >= 2);
		
		if(len >= 2)
		{
				AR_ASSERT(ns[2]->lexeme.lexeme[0] == L'{' && ns[2]->lexeme.lexeme[len-1] == L'}');
				res->predef.code = AR_wcsndup(ns[2]->lexeme.lexeme+1, len - 2);
		}else
		{
				res->predef.code = AR_wcsdup(L"");
		}

		if(ns[1] == NULL)
		{
				res->predef.name = AR_wcsdup(L"");
		}else
		{
				res->predef.name = AR_wcsdup(ns[1]->lexeme.lexeme);
		}

		return res;

}




/*
{ L"program				:		item_list #",			__handle_program, 0},
{ L"program				:		item_list error #",		__handle_program, 0},
*/
static psrNode_t*		AR_STDCALL __handle_program(psrNode_t **nodes, size_t count, const wchar_t * name_tmp, void *ctx)
{
		cfgNode_t		**ns = (cfgNode_t**)nodes;
		cfgNode_t		*res = NULL;
		size_t	i;
		bool_t			has_err = false;
		cfgNodeList_t name, token, prec, rule, error, empty, predef;
		cfgStart_t		*start_rule = NULL;
		cfgParserData_t	*parser_ctx;
		

		AR_ASSERT(count == 2 || count == 3);
		AR_ASSERT((ns[0] == NULL) || (ns[0] && ns[0]->type == CFG_NODE_LIST_T));

		parser_ctx = (cfgParserData_t*)ctx;

		AR_ASSERT(parser_ctx != NULL);


		AR_UNUSED(name_tmp);

		if(count == 2)
		{
				AR_ASSERT(ns[1]->type == CFG_LEXEME_T && ns[1]->lexeme.lex_val == CFG_LEXVAL_FAKE_EOI);
		}else
		{
				AR_ASSERT(ns[1] == NULL && ns[2]->type == CFG_LEXEME_T && ns[2]->lexeme.lex_val == CFG_LEXVAL_FAKE_EOI);
				has_err = true;
		}

		CFG_InitNodeList(&name);
		CFG_InitNodeList(&token);
		CFG_InitNodeList(&prec);
		CFG_InitNodeList(&rule);
		CFG_InitNodeList(&error);
		CFG_InitNodeList(&empty);
		CFG_InitNodeList(&predef);
		 

		for(i = 0; ns[0] && i < ns[0]->lst.count; ++i)
		{
				cfgNode_t *node = ns[0]->lst.lst[i];
				AR_ASSERT(node);

				switch(node->type)
				{
				case CFG_EMPTY_T:
				{
						CFG_InsertToNodeList(&empty, node);
						break;
				}
				case CFG_NAME_T:
				{
						CFG_InsertToNodeList(&name, node);
						break;
				}
				case CFG_TOKEN_T:
				{
						CFG_InsertToNodeList(&token, node);
						break;
				}
				case CFG_PREC_T:
				{
						CFG_InsertToNodeList(&prec, node);
						break;
				}
				case CFG_START_T:
				{
						start_rule = &node->start;
						break;
				}
				case CFG_RULE_T:
				{
						CFG_InsertToNodeList(&rule, node);
						break;
				}
				case CFG_ERROR_T:
				{
						CFG_InsertToNodeList(&error, node);
						break;
				}
				case CFG_PREDEF_T:
				{
						CFG_InsertToNodeList(&predef, node);
						break;
				}
				case CFG_NODE_LIST_T:
				case CFG_CONFIG_T:
				case CFG_LEXEME_T:
				default:
				{
						AR_ASSERT(false);
						AR_CHECK(false, L"Arsenal Tools Internal Error In : %hs\r\n", AR_FUNC_NAME);
				}
				}
		}

		res = CFG_CreateNode(CFG_CONFIG_T);
		CFG_InitConfig(&res->config, &name, &token, &prec, &rule, start_rule, &predef);

		if(parser_ctx->has_error || error.count > 0 || has_err)
		{
				res->config.has_error = true;
		}

		CFG_UnInitNodeList(&name);
		CFG_UnInitNodeList(&token);
		CFG_UnInitNodeList(&prec);
		CFG_UnInitNodeList(&rule);
		CFG_UnInitNodeList(&error);
		CFG_UnInitNodeList(&empty);
		CFG_UnInitNodeList(&predef);
		return (psrNode_t*)res;
}



typedef struct __cfg_rule_tag
{
		const wchar_t *rule;
		psrRuleFunc_t handler;
		size_t			auto_ret;
}cfgRuleDef_t;



static const cfgRuleDef_t	__cfg_rule[] =
{


		{ L"item_list			:		item_list item",	__handle_item_list, 0},
		{ L"item_list			:		",	NULL, 0},
		
		{ L"item				:		pre_code_decl ;",	 __handle_item,0},
		{ L"item				:		start_def ;",	__handle_item, 0},
		{ L"item				:		name_def ;",	__handle_item, 0},
		{ L"item				:		token_def ;",	__handle_item,0},
		{ L"item				:		prec_def ;",	__handle_item,0},
		{ L"item				:		rule_def ;",	__handle_item,0},
		{ L"item				:		error ; ",	__handle_item,0},
		{ L"item				:		; ",	__handle_item,0},

		{ L"name_def 			:		%name lexeme : lexeme ",								__handle_name_def,0},

		{ L"token_def			:  		%token %skip token_val_prec : lexeme token_val_prec token_value action_decl",	__handle_token_def,0},
		{ L"token_def			:  		%token lexeme token_val_prec : lexeme token_val_prec token_value action_decl",	__handle_token_def,0},

		{ L"token_val_prec 		:		, number",												NULL,1},
		{ L"token_val_prec 		:		",														NULL,0},

		{ L"token_value			:  		%value lexeme",	NULL,1},
		{ L"token_value			:  		",	NULL,0},


		{ L"prec_def			:  		assoc term_list ",								__handle_prec_def,0},


		{ L"rule_def			: 		lexeme : rhs_list ",							__handle_rule_def,0},


		{ L"rhs_list			: 		rhs_list  | rhs ",								__handle_rhs_list,0},
		{ L"rhs_list			: 		rhs ",											__handle_rhs_list,0},
		{ L"rhs					:		term_list prec_decl action_decl",				__handle_rhs,0},
		{ L"rhs					:		. prec_decl action_decl",						__handle_rhs,0},

		{ L"term_list			:		term_list lexeme",								__handle_term_list,0},
		{ L"term_list			:		lexeme",										__handle_term_list,0},




		{ L"prec_decl			:		%prec lexeme",									NULL,1},

		{ L"prec_decl			:		",												NULL,0},
		
		{ L"action_decl			:		%action lexeme action_ins",				__handle_action_decl,0},
		{ L"action_decl			:		%action lexeme",						__handle_action_decl,0},
		{ L"action_decl			:		",										NULL,0},

		{ L"start_def			:  		%start lexeme",							__handle_start_def,0},

		
		{ L"pre_code_decl		:		%code pre_code_name action_ins",		__handle_pre_def,0},
		{ L"pre_code_name		:		lexeme",								NULL,0},
		{ L"pre_code_name		:		",										NULL,0},


		{ L"program				:		item_list #",			__handle_program, 0},
		{ L"program				:		item_list error #",		__handle_program, 0}

};




static lex_t* __build_lex()
{
		lex_t *lex;
		size_t	i;

		lex = Lex_Create();

		AR_ASSERT(lex != NULL);

		for(i = 0; i < AR_NELEMS(__cfg_lex_name); ++i)
		{
				if(!Lex_Insert(lex, __cfg_lex_name[i]))
				{
						AR_CHECK(false, L"Arsenal internal error : %hs\r\n", AR_FUNC_NAME);

				}
		}

		for(i = 0; i < AR_NELEMS(__cfg_pattern); ++i)
		{
				lexAction_t action;
				action.is_skip = __cfg_pattern[i].is_skip;
				action.priority = __cfg_pattern[i].prec;
				action.value = (size_t)__cfg_pattern[i].val;

				
				if(!Lex_InsertRule(lex, __cfg_pattern[i].regex, &action))
				{
						AR_CHECK(false, L"Arsenal internal error : %hs\r\n", AR_FUNC_NAME);

				}
		}


		if(!Lex_GenerateTransTable(lex))
		{
				AR_CHECK(false, L"Arsenal internal error : %hs\r\n", AR_FUNC_NAME);
		}

		return lex;
}


static psrGrammar_t*	__build_grammar(psrHandler_t *handler)
{
		psrGrammar_t	*gmr;
		size_t i;
		AR_ASSERT(handler != NULL);

		gmr = Parser_CreateGrammar(handler);

		if(gmr == NULL)return NULL;

		for(i = 0; i < AR_NELEMS(__cfg_term); ++i)
		{
				if(!Parser_InsertTerm(gmr, __cfg_term[i].name, (size_t)__cfg_term[i].val, PARSER_ASSOC_NONASSOC, 0, __build_leaf))
				{
						AR_CHECK(false, L"Arsenal internal error : %hs\r\n", AR_FUNC_NAME);
				}
		}

		for(i = 0; i < AR_NELEMS(__cfg_rule); ++i)
		{
				if(!Parser_InsertRuleByStr(gmr, __cfg_rule[i].rule, NULL, __cfg_rule[i].handler, __cfg_rule[i].auto_ret))
				{
						AR_CHECK(false, L"Arsenal internal error : %hs\r\n", AR_FUNC_NAME);
				}

		}

		if(!Parser_SetStartRule(gmr, L"program"))
		{
				AR_CHECK(false, L"Arsenal internal error : %hs\r\n", AR_FUNC_NAME);
				return NULL;
		}

		if(!Parser_CheckIsValidGrammar(gmr, NULL))
		{
				AR_CHECK(false, L"Arsenal internal error : %hs\r\n", AR_FUNC_NAME);
		}

		return gmr;
}



static const parser_t*		__build_parser(const psrGrammar_t *gmr)
{
		const parser_t *parser;
		AR_ASSERT(gmr && Parser_CheckIsValidGrammar(gmr, NULL));

		parser = Parser_CreateParser(gmr, PARSER_SLR);
		AR_ASSERT(parser && Parser_CountParserConflict(parser) == 0);
		return parser;
}








static void	AR_STDCALL cfg_on_error(int_t level, const wchar_t *msg, void *ctx)
{
		cfgReport_t				*report = NULL;
		cfgReportInfo_t			info;
		AR_ASSERT(msg != NULL && ctx != NULL);

		report = ((cfgParserData_t*)ctx)->report;
		
		AR_memset(&info, 0, sizeof(info));

		info.type = CFG_REPORT_ERROR_T;
		info.error.err_level = level;
		info.error.err_msg = msg;
		report->report_func(&info, report->report_ctx);
}


void	AR_STDCALL cfg_on_print(const wchar_t *msg, void *ctx)
{
		cfgReport_t				*report = NULL;
		cfgReportInfo_t			info;
		AR_ASSERT(msg != NULL && ctx != NULL);

		report = ((cfgParserData_t*)ctx)->report;

		AR_memset(&info, 0, sizeof(info));

		info.type = CFG_REPORT_MESSAGE_T;
		info.std_msg.message = msg;
		report->report_func(&info, report->report_ctx);
}




static void	AR_STDCALL cfg_free(psrNode_t *node, void *ctx)
{
		AR_UNUSED(ctx);
		CFG_DestroyNode((cfgNode_t*)node);
}




static void		AR_STDCALL cfg_error(const psrToken_t *tok, const size_t expected[], size_t count, void *ctx)
{
		cfgReport_t				*report = NULL;
		cfgReportInfo_t			info;

		arString_t		*str;
		wchar_t			*buf;
		size_t			i;

/*
		if(tok->term_val == CFG_LEXVAL_FAKE_EOI)
		{
				return;
		}
*/

		AR_ASSERT(tok != NULL && ctx != NULL && expected != NULL && count > 0);

		report = ((cfgParserData_t*)ctx)->report;
		AR_memset(&info, 0, sizeof(info));

		/******************************************************************************************/
		if(tok->str_cnt == 0 || tok->term_val == CFG_LEXVAL_FAKE_EOI)
		{
				buf = AR_wcsdup(L"%CFG_LEXVAL_EOI");
		}else
		{
				buf = AR_wcsndup(tok->str, tok->str_cnt);
		}
		
		str = AR_CreateString();

		AR_AppendFormatString(str
						, L"Invalid Token \"%ls\" in (%Id : %Id)\r\n\r\n"
						, buf
						, tok->line
						, tok->col
						);
		AR_DEL(buf);
		AR_AppendFormatString(str, L"Expected Term :\r\n");

		for(i = 0; i < count; ++i)
		{
				const wchar_t *term = NULL;
				size_t x;
				for(x = 0; x < sizeof(__cfg_term) / sizeof(__cfg_term[0]); ++x)
				{
						if((size_t)(__cfg_term[x].val) == expected[i])
						{
								term = __cfg_term[x].name;
						}
				}

				AR_ASSERT(term != NULL);
				AR_AppendFormatString(str, L"\"%ls\" ", term);
		}
		AR_AppendFormatString(str, L"\r\n\r\n");
		/******************************************************************************************/
		info.syntax_error.msg = AR_GetStrString(str);
		info.syntax_error.tok = tok;
		info.type = CFG_REPORT_ERROR_SYNTAX_T;
		report->report_func(&info, report->report_ctx);
		AR_DestroyString(str);
}



static void AR_STDCALL __default_report_func(const cfgReportInfo_t *report, void *context)
{
		AR_UNUSED(report);
		AR_UNUSED(context);
}


static cfgReport_t		__g_def_report = 
{
		__default_report_func,
		NULL
};


static arIOCtx_t	__def_io_ctx = 
{		
		cfg_on_error,
		cfg_on_print,
		NULL
};

static psrHandler_t		__def_handler_ctx = 
{
		cfg_error,
		cfg_free,
};


static arSpinLock_t				__g_lock;
static lex_t					*__g_lex;
static psrGrammar_t				*__g_grammar = NULL;
static const parser_t			*__g_parser = NULL;


static void __init_parser_tag()
{
		psrHandler_t	psr_handler;
		AR_InitSpinLock(&__g_lock);
		
		psr_handler.error_f = cfg_error;
		psr_handler.free_f = cfg_free;

		__g_lex		= __build_lex();
		__g_grammar = __build_grammar(&psr_handler);
		__g_parser	= __build_parser(__g_grammar);
}


static void __uninit_parser_tag()
{
		Parser_DestroyParser(__g_parser);
		Parser_DestroyGrammar(__g_grammar);
		Lex_Destroy(__g_lex);
		__g_lex = NULL;
		__g_parser = NULL;
		__g_grammar = NULL;
		AR_UnInitSpinLock(&__g_lock);
		
}

static lexMatch_t*		__create_lex_match()
{
		lexMatch_t		*match;

		AR_LockSpinLock(&__g_lock);
		match = Lex_CreateMatch(__g_lex);
		AR_UnLockSpinLock(&__g_lock);

		return match;
}

static void				__destroy_lex_match(lexMatch_t *match)
{
		AR_ASSERT(match != NULL);
		Lex_DestroyMatch(match);
}

static psrContext_t*	__create_parser_context(void *ctx)
{
		psrContext_t	*parser_context = NULL;
		AR_LockSpinLock(&__g_lock);
		parser_context = Parser_CreateContext(__g_parser, ctx);
		AR_UnLockSpinLock(&__g_lock);
		return parser_context;
}

static void				__destroy_parser_context(psrContext_t *parser_context)
{
		AR_ASSERT(parser_context != NULL);
		Parser_DestroyContext(parser_context);
}





const cfgConfig_t*	CFG_CollectGrammarConfig(const wchar_t *gmr_txt, cfgReport_t *report)
{

		bool_t is_ok, has_error;
		
		psrContext_t	*parser_context;
		cfgParserData_t	psr_data;
		lexMatch_t		*match;

		lexToken_t		tok;
		psrToken_t		term;
		
		cfgNode_t		*result = NULL;

		arIOCtx_t		io_ctx ;
		

		AR_ASSERT(gmr_txt != NULL);

		if(report == NULL)report = &__g_def_report;

		io_ctx.on_error = cfg_on_error;
		io_ctx.on_print = cfg_on_print;
		io_ctx.ctx		= (void*)report;

		
		
		match		   = __create_lex_match();

		__init_parser_data(&psr_data, report);
		parser_context = __create_parser_context((void*)&psr_data);
		
		Lex_ResetInput(match, gmr_txt);

		is_ok = true;
		has_error = false;


		while(is_ok)
		{
				is_ok = Lex_Match(match, &tok);

				if(!is_ok)
				{
						cfgReportInfo_t	info;
						psrToken_t		tmp_tok;
						arString_t		*str;
						size_t n;
						size_t	line, col;
						const wchar_t *tok = NULL;

						AR_memset(&info, 0, sizeof(info));
						n = AR_wcslen(Lex_GetNextInput(match));
						tok = AR_wcsndup(Lex_GetNextInput(match), n > 5 ? 5 : n);

						str = AR_CreateString();
						
						Lex_MatchGetCoordinate(match, &line, &col);
						AR_AppendFormatString(str, L"Invalid Token %ls...(%Id : %Id)\r\n", tok, line, col);

						if(tok)AR_DEL(tok);

						info.type = CFG_REPORT_ERROR_LEX_T;
						info.lex_error.msg = AR_GetStrString(str);

						tmp_tok.term_val = 0;
						tmp_tok.str_cnt = 0;
						tmp_tok.str =  Lex_GetNextInput(match);

						Lex_MatchGetCoordinate(match, &line, &col);
						tmp_tok.line = line;
						tmp_tok.col = col;

						info.lex_error.tok = &tmp_tok;
						
						report->report_func(&info, report->report_ctx);
						
						AR_DestroyString(str);

						AR_ASSERT(*Lex_GetNextInput(match) != L'\0');
						Lex_Skip(match);
						Lex_ClearError(match);
						has_error = true;
						is_ok = true;
						continue;
				}

				if(tok.value == CFG_LEXVAL_COMMENT)
				{
						continue;
				}

				PARSER_TOTERMTOK(&tok, &term);

				/*
						����һ���򵥿���䣬�Ա㲻���� abc CFG_LEXVAL_EOI��������£��޷�����������
				*/
				if(term.term_val == CFG_LEXVAL_EOI)
				{
						psrToken_t end;
						end.col = term.col;
						end.line = term.line;
						end.str = L"#";
						end.str_cnt = 1;
						end.term_val = CFG_LEXVAL_FAKE_EOI;

						if(!Parser_AddToken(parser_context, &end))
						{
								AR_CHECK(false, L"Arsenal internal error : %hs\r\n", AR_FUNC_NAME);
						}
				}

				is_ok = Parser_AddToken(parser_context, &term);
				
				if(tok.value == CFG_LEXVAL_EOI)break;
		}		

		if(is_ok)
		{
				result = (cfgNode_t*)Parser_GetResult(parser_context);
				AR_ASSERT(result->type == CFG_CONFIG_T);

				if(result && !result->config.has_error)
				{
						result->config.has_error = has_error;
				}
		}

		
		__destroy_parser_context(parser_context);
		__destroy_lex_match(match);
		__uninit_parser_data(&psr_data);
		
		return (cfgConfig_t*)result;

}

void			CFG_DestroyGrammarConfig(const cfgConfig_t *cfg)
{
		CFG_DestroyNode((cfgNode_t*)cfg);

}


/***************************************************************************/

cfgLexicalSet_t*		__create_lexical_set()
{
		return AR_NEW0(cfgLexicalSet_t);
}

void					__destroy_lexical_set(cfgLexicalSet_t *lx_set)
{
		AR_ASSERT(lx_set != NULL);

		if(lx_set->token_set)
		{
				AR_DEL(lx_set->token_set);
		}

		if(lx_set)
		{
				AR_DEL(lx_set);
		}
}

static void __insert_to_lexical_set(cfgLexicalSet_t *lx_set, const lexToken_t *tok)
{
		AR_ASSERT(lx_set != NULL && tok != NULL);

		if(lx_set->cnt == lx_set->cap)
		{
				lx_set->cap = (lx_set->cap + 4)*2;
				lx_set->token_set = AR_REALLOC(lexToken_t, lx_set->token_set, lx_set->cap);
		}

		lx_set->token_set[lx_set->cnt] = *tok;
		lx_set->cnt++;
}

const cfgLexicalSet_t*		CFG_CollectLexicalSet(const wchar_t *gmr_txt)
{
		lexMatch_t		*match;
		cfgLexicalSet_t	*lx_set;
		bool_t is_ok;
		lexToken_t		tok;
		AR_ASSERT(gmr_txt != NULL);

		match = __create_lex_match();
		lx_set = __create_lexical_set();
		
		Lex_ResetInput(match, gmr_txt);


		is_ok = true;


		while(is_ok)
		{
				is_ok = Lex_Match(match, &tok);

				if(!is_ok)
				{
						lx_set->has_error = true;
						AR_ASSERT(*Lex_GetNextInput(match) != L'\0');
						Lex_Skip(match);
						Lex_ClearError(match);
						is_ok = true;
						continue;
				}
				
				__insert_to_lexical_set(lx_set, &tok);

				if(tok.value == CFG_LEXVAL_EOI)break;
		}		
		__destroy_lex_match(match);

		if(lx_set->cnt == 0)
		{
				__destroy_lexical_set(lx_set);
				lx_set = NULL;
		}

		return lx_set;
}

void						CFG_DestroyLexicalSet(const cfgLexicalSet_t *lexical_set)
{
		if(lexical_set)
		{
				__destroy_lexical_set((cfgLexicalSet_t*)lexical_set);
				lexical_set = NULL;
		}
}


void			CFG_Init()
{
		__init_cfg_node_list();
		__init_parser_tag();
}

void			CFG_UnInit()
{
		__uninit_parser_tag();
		__uninit_cfg_node_list();
}



AR_NAMESPACE_END
