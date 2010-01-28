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

#include "lalr.h"
#include "lr_action.h"


AR_NAMESPACE_BEGIN



/******************************************************************************aux*****************************/



const psrAction_t*		PSR_GetAction(const psrActionTable_t *tbl, size_t state, const psrSymb_t *symb)
{
		int_t idx;
		AR_ASSERT(state < tbl->row && symb->type == PSR_TERM);
		idx = PSR_BSearchFromSymbList(&tbl->term_set,symb);
		AR_ASSERT(idx != -1);
		return tbl->actions[AR_TBL_IDX_R(state,idx, tbl->col)];


}

int_t					PSR_GetState(const psrActionTable_t *tbl, size_t state, const psrSymb_t *symb)
{
		int_t idx;
		AR_ASSERT(state < tbl->goto_row && symb->type == PSR_NONTERM);
		idx = PSR_BSearchFromSymbList(&tbl->nonterm_set,symb);
		return tbl->goto_tbl[AR_TBL_IDX_R(state,idx, tbl->goto_col)];
}

const psrSymbList_t*	PSR_GetExpectedSymb(const psrActionTable_t *tbl, size_t state)
{
		AR_ASSERT(state < tbl->row);
		return &tbl->expected_set[state];
}


static psrActionTable_t* __create_table(const psrGrammar_t *gmr)
{
		psrActionTable_t *tbl;
		size_t i;
		
		const psrSymbList_t *symb_lst;
		AR_ASSERT(gmr != NULL && gmr->count > 1);

		tbl = AR_NEW0(psrActionTable_t);
		symb_lst = PSR_GetSymbList(gmr);
		
		PSR_InitSymbList(&tbl->term_set);
		PSR_InitSymbList(&tbl->nonterm_set);

		for(i = 0; i < symb_lst->count; ++i)
		{
				const psrSymb_t *curr;
				curr = symb_lst->lst[i];

				if(curr->type == PSR_TERM)
				{
						PSR_InsertToSymbList(&tbl->term_set, PSR_CopyNewSymb(curr));
				}else
				{
						PSR_InsertToSymbList(&tbl->nonterm_set, PSR_CopyNewSymb(curr));
				}
		}

		PSR_SortSymbList(&tbl->term_set);
		PSR_SortSymbList(&tbl->nonterm_set);

		return tbl;
}




static void __build_goto_table(psrActionTable_t *tbl, const lalrStateSet_t *set)
{
		size_t i,j;
		AR_ASSERT(tbl != NULL && tbl->goto_tbl == NULL && set != NULL && set->count > 0);

		tbl->goto_row = set->count;
		tbl->goto_col = tbl->nonterm_set.count;
		tbl->goto_tbl = AR_NEWARR(int_t, tbl->goto_row * tbl->goto_col);
		AR_memset(tbl->goto_tbl, (int_t)-1, tbl->goto_row * tbl->goto_col * sizeof(int_t));
		

		for(i = 0; i < tbl->goto_row; ++i)
		{
				const lalrState_t *state;
				state = set->set[i];
				
				for(j = 0; j < state->count; ++j)
				{
						if(state->actions[j].symb->type == PSR_NONTERM && state->actions[j].act_type == PSR_SHIFT)
						{
								int_t idx;
								idx = PSR_BSearchFromSymbList(&tbl->nonterm_set, state->actions[j].symb);
								AR_ASSERT(idx != -1);
								tbl->goto_tbl[AR_TBL_IDX_R(i, idx, tbl->goto_col)] = PSR_IndexOfStateSet(set, state->actions[j].to);
						}
				}
		}

}


/*�˺������ڽ�����Ϻ����*/
static void __build_expected_list(psrActionTable_t *tbl)
{
		size_t i,j;
		AR_ASSERT(tbl != NULL && tbl->expected_set == NULL && tbl->row > 0);
		
		tbl->expected_set = AR_NEWARR0(psrSymbList_t, tbl->row);

		for(i = 0; i < tbl->row; ++i)PSR_InitSymbList(&tbl->expected_set[i]);
		
		for(i = 0; i < tbl->row; ++i)
		{
				for(j = 0; j < tbl->term_set.count; ++j)
				{
						const psrAction_t	*act;
						const psrSymb_t *symb = tbl->term_set.lst[j];
						
						if(PSR_CompSymb(symb, PSR_ErrorSymb) == 0 )continue;

						act = PSR_GetAction(tbl, i, symb);
						AR_ASSERT(act != NULL);
						
						if(act->type != PSR_ERROR)
						{
								PSR_InsertToSymbList(&tbl->expected_set[i], symb);
						}
				}
		}
}

static void __destroy_expected_list(psrActionTable_t *tbl)
{
		size_t i;
		AR_ASSERT(tbl != NULL && tbl->row > 0);
		
		for(i = 0; i < tbl->row; ++i)PSR_UnInitSymbList(&tbl->expected_set[i]);
		AR_DEL(tbl->expected_set);
		tbl->expected_set = NULL;
}



void					PSR_DestroyActionTable(psrActionTable_t *tbl)
{
		size_t i;
		
		__destroy_expected_list(tbl);

		for(i = 0; i < tbl->term_set.count; ++i)
		{
				PSR_DestroySymb(tbl->term_set.lst[i]);
		}
		
		for(i = 0; i < tbl->nonterm_set.count; ++i)
		{
				PSR_DestroySymb(tbl->nonterm_set.lst[i]);
		}

		PSR_UnInitSymbList(&tbl->nonterm_set);
		PSR_UnInitSymbList(&tbl->term_set);
		AR_DEL(tbl->goto_tbl);

		for(i = 0; i < tbl->row * tbl->col; ++i)
		{
				psrAction_t *action;

				action = tbl->actions[i];

				if(action->type != PSR_ERROR)
				{
						while(action != NULL)
						{
								psrAction_t *tmp;
								tmp = action->next;
								AR_DEL(action);
								action = tmp;
						}
				}
		}
		
		AR_DEL(tbl->actions);
		AR_DEL(tbl);

}




static const psrAction_t error_action = {PSR_ERROR, 0, 0, 0, 0,0, NULL};

const psrAction_t	*PSR_ErrorAction = &error_action;



/*
���︺����lookahead�Ͳ���ʽ�������ȼ�����������Լ��ͻ���⣬�����������ǣ����ȼ��ߵİ����ȼ��͵��滻����
������ȼ���ͬ����鿴lookahead����������ϣ��ҵ�ǰ�����������ӽ������ǹ�Լ���ô���������Լ��ͻ
���滻������Ϊ��Լ������ǰ�ǹ�Լ�������ӽ�����Ҳ�ǹ�Լ���õ�һ����Լ��Լ��ͻ���������ӽ����Ĳ��ǹ�Լ�
����������ӵģ��������ء�
lookaheadΪ�ҽ��ʱ����֮�෴�����lookahead�޽���ԣ���ͬ�����ǳ�ͻ��������롣
*/

static void __copy_action(psrAction_t *l, const psrAction_t *r)
{
		l->rule_num = r->rule_num;
		l->prec = r->prec;
		l->type = r->type;
		l->delim = r->delim;
		l->reduce_count = r->reduce_count;
		l->shift_to = r->shift_to;
}



static void __insert_action_to_action_list(psrAction_t **dest, const psrAction_t *sour, const psrTermInfo_t *lookahead)
{
		psrAction_t *curr, *prev, *tmp;
		AR_ASSERT(dest != NULL && sour != NULL && lookahead != NULL);
		
		curr = *dest;
		prev = NULL;
		if(curr == NULL)
		{
				*dest = AR_NEW0(psrAction_t);
				__copy_action(*dest, sour);
				return;
		}

		/*
				���ܴ���shift�������ﲻ�ǳ�ͻ�����ֱ�ӷ��أ���Ϊ��������ͬһ�����ϵ����뵼�µ�״̬ת������ǰ���DFA����������
		*/
		if(curr->type == PSR_SHIFT && sour->type == PSR_SHIFT)return;

		if(curr->type == PSR_ACCEPT)
		{
				tmp = AR_NEW0(psrAction_t);
				__copy_action(tmp, sour);
				tmp->next = curr->next;
				curr->next = tmp;
				return;
		}
		
		
		if(sour->prec > curr->prec)
		{
				/*
						�¼�������ȼ�����ԭ��򲻱���Ϊ�ǳ�ͻ����˲��뵽ͷ��㣬curr����Ϊhead���
				*/
				__copy_action(curr, sour);
				return;
				
		}else if(sour->prec < curr->prec)
		{
				/*
						�¼�������ȼ�����ԭ��򲻱���Ϊ�ǳ�ͻ���Ҳ����κ��޸ģ���������
				*/
				return;
		}else
		{
				/*
						��ͬ���ȼ�������£�
				*/
				if(lookahead->assoc == PSR_ASSOC_NOASSOC)
				{

						tmp = AR_NEW0(psrAction_t);
						__copy_action(tmp, sour);
						
						if(curr->type != PSR_SHIFT)
						{
								/*�������shift�����µ�action����ǰ��*/
								tmp->next = *dest;
								*dest = tmp;
						}else
						{
								/*�����shift����ѡ��curr��ǰ*/
								tmp->next = curr->next;
								curr->next = tmp;
						}

				}else if(lookahead->assoc == PSR_ASSOC_LEFT)
				{
						/*���ϣ����ȼ���ͬ������ѡ��Լ*/
						if(curr->type == PSR_SHIFT && sour->type == PSR_REDUCE)/*shift-reduce��ͻ��������*/
						{
								__copy_action(curr, sour);
						
						}else if(curr->type == PSR_REDUCE && sour->type != PSR_REDUCE)
						{
								/*���ϣ�ֻҪcurrΪ��Լ��sour���ǹ�Լ�򲻱䶯*/
								return;
						}else/*��ͻ*/
						{
								/*��ط�һ���ǹ�Լ��Լ��ͻ*/
								AR_ASSERT(curr->type == PSR_REDUCE && sour->type == PSR_REDUCE);
								tmp = AR_NEW0(psrAction_t);
								__copy_action(tmp, sour);
								tmp->next = curr->next;
								curr->next = tmp;
						}
				}else if(lookahead->assoc == PSR_ASSOC_RIGHT)
				{
						/*�ҽ�ϣ����ȼ���ͬ������ѡ����*/
						if(curr->type == PSR_REDUCE && sour->type == PSR_SHIFT)
						{
								__copy_action(curr, sour);
						}else if(curr->type == PSR_SHIFT && sour->type != PSR_SHIFT)
						{
								/*�ҽ�ϣ�ֻҪcurrΪ���룬sour���������򲻱䶫*/
								return;
						}else/*��ͻ*/
						{
								/*��ط�һ���ǹ�Լ��Լ��ͻ*/
								AR_ASSERT(curr->type == PSR_REDUCE && sour->type == PSR_REDUCE);
								tmp = AR_NEW0(psrAction_t);
								__copy_action(tmp, sour);
								tmp->next = curr->next;
								curr->next = tmp;
						}
				}
		}
}

/*********************************************************************************************************************************/



psrActionTable_t* PSR_CreateActionTable_SLR(const psrGrammar_t *grammar)
{
		psrActionTable_t *tbl;
		lalrState_t	*start;
		lalrStateSet_t set;
		psrSymbMap_t first, follow;

		size_t i;
		tbl = __create_table(grammar);
		
		PSR_InitStateSet(&set);
		start = PSR_Create_LALR_State(grammar);
		PSR_CollectState(&set, start);
		__build_goto_table(tbl, &set);


		PSR_InitSymbMap(&first);
		PSR_InitSymbMap(&follow);
		PSR_CalcFirstSet(grammar, &first);
		PSR_CalcFollowSet(grammar, &follow, &first);
		
		tbl->row = set.count;
		tbl->col = tbl->term_set.count;

		tbl->actions = AR_NEWARR0(psrAction_t*, tbl->row * tbl->col);



		for(i = 0; i < tbl->row; ++i)
		{
				lalrState_t *state;
				lalrConfigNode_t *node;
				state = set.set[i];
				
				for(node = state->all_config->head; node != NULL; node = node->next)
				{
						const lalrConfig_t *config;
						const psrSymb_t	  *body;
						const psrRule_t	  *rule;
						size_t			rule_num;
						psrAction_t		  action;
						
						config = node->config;

						rule = config->rule;
						rule_num = (size_t)PSR_IndexOfGrammar(grammar, rule);
						AR_ASSERT(rule_num < grammar->count);

						body = PSR_IndexOfSymbList(&rule->body, config->delim);
						AR_memset(&action, 0, sizeof(action));
						
						action.rule_num = rule_num;
						action.delim = config->delim;
						action.prec = PSR_GetRulePrecAssocInfo(grammar, rule)->prec;
						action.reduce_count = 0;
						action.shift_to = 0;
						action.next = NULL;
						

						if(body == NULL)
						{
								size_t x;
								action.reduce_count = rule->body.count;
								
								


								if(PSR_CompSymb(rule->head, PSR_StartSymb) == 0)/*����״̬*/
								{
										action.type = PSR_ACCEPT;
								}else
								{
										action.type = PSR_REDUCE;
								}

								{
										
										const psrMapRec_t		*rec;
										rec = PSR_GetSymbolFromSymbMap(&follow, rule->head);
										AR_ASSERT(rec != NULL);
										
										for(x = 0; x < rec->lst.count; ++x)
										{
												int_t idx;
												const psrSymb_t *symb_tmp1 = rec->lst.lst[x];
												idx = PSR_FindFromSymbList(&tbl->term_set, symb_tmp1);
												AR_ASSERT(idx != -1);
												
												__insert_action_to_action_list(&tbl->actions[AR_TBL_IDX_R(i,idx, tbl->col)], &action,PSR_GetTermSymbInfo(grammar, symb_tmp1));
										}
								}
						}else
						{
								if(body->type == PSR_TERM)
								{
										const lalrState_t *trans_to;
										int_t trans_to_idx,idx;

										action.type = PSR_SHIFT;
										trans_to = PSR_GetTransTo(state, body);
										AR_ASSERT(trans_to != NULL);
										trans_to_idx = PSR_IndexOfStateSet(&set, trans_to);
										AR_ASSERT(trans_to_idx != -1);
										action.shift_to = (size_t)trans_to_idx;
										
										idx = PSR_FindFromSymbList(&tbl->term_set, body);
										__insert_action_to_action_list(&tbl->actions[AR_TBL_IDX_R(i,idx, tbl->col)], &action, PSR_GetTermSymbInfo(grammar, body));
								}
								
						}

				}
		}




		PSR_UnInitSymbMap(&first);
		PSR_UnInitSymbMap(&follow);

		PSR_DestroyState_ALL(start);
		PSR_UnInitStateSet(&set);

		for(i = 0; i < tbl->row * tbl->col; ++i)
		{
				if(tbl->actions[i] == NULL)
				{
						tbl->actions[i] = (psrAction_t*)PSR_ErrorAction;
				}
		}

		__build_expected_list(tbl);
		return tbl;
}

psrActionTable_t* PSR_CreateActionTable_LALR(const psrGrammar_t *grammar)
{
		psrActionTable_t		*tbl;
		lalrState_t				*start;
		lalrStateSet_t			set;
		size_t i;
		tbl = __create_table(grammar);
		
		PSR_InitStateSet(&set);
		start = PSR_Create_LALR_State(grammar);
		PSR_CollectState(&set, start);
		__build_goto_table(tbl, &set);

		tbl->row = set.count;
		tbl->col = tbl->term_set.count;

		tbl->actions = AR_NEWARR0(psrAction_t*, tbl->row * tbl->col);


		for(i = 0; i < tbl->row; ++i)
		{
				lalrState_t *state;
				lalrConfigNode_t *node;
				state = set.set[i];

				for(node = state->all_config->head; node != NULL; node = node->next)
				{
						const lalrConfig_t *config;
						const psrSymb_t	  *body;
						const psrRule_t	  *rule;
						size_t			  rule_num;
						psrAction_t		  action;
					
						config = node->config;
						rule = config->rule;

						rule_num = PSR_IndexOfGrammar(grammar, rule);
						body = PSR_IndexOfSymbList(&rule->body, config->delim);
						AR_memset(&action, 0, sizeof(action));

						action.rule_num = rule_num;
						action.delim = config->delim;
						action.prec = PSR_GetRulePrecAssocInfo(grammar, rule)->prec;
						action.reduce_count = 0;
						action.shift_to = 0;
						action.next = NULL;
						
						if(body == NULL)
						{
								size_t x;
								
								action.reduce_count = rule->body.count;

								if(PSR_CompSymb(rule->head, PSR_StartSymb) == 0)/*����״̬*/
								{
										action.type = PSR_ACCEPT;
								}else
								{
										action.type = PSR_REDUCE;
								}
								{
										
										for(x = 0; x < config->follow_set.count; ++x)
										{
												int_t idx;
												const psrSymb_t *lookahead_symb = config->follow_set.lst[x];
												idx = PSR_FindFromSymbList(&tbl->term_set, lookahead_symb);

												AR_ASSERT(idx != -1);
												__insert_action_to_action_list(&tbl->actions[AR_TBL_IDX_R(i,idx, tbl->col)], &action, PSR_GetTermSymbInfo(grammar,lookahead_symb));
										}
										
								}
						}else
						{
								if(body->type == PSR_TERM)
								{
										const lalrState_t *trans_to;
										int_t trans_to_idx,idx;

										action.type = PSR_SHIFT;
										trans_to = PSR_GetTransTo(state, body);
										AR_ASSERT(trans_to != NULL);
										trans_to_idx = PSR_IndexOfStateSet(&set, trans_to);
										AR_ASSERT(trans_to_idx != -1);
										action.shift_to = (size_t)trans_to_idx;
										
										idx = PSR_FindFromSymbList(&tbl->term_set, body);
										__insert_action_to_action_list(&tbl->actions[AR_TBL_IDX_R(i,idx, tbl->col)], &action, PSR_GetTermSymbInfo(grammar,body));
								}
								
						}
				}
		}

		
		PSR_DestroyState_ALL(start);
		PSR_UnInitStateSet(&set);

		for(i = 0; i < tbl->row * tbl->col; ++i)
		{
				if(tbl->actions[i] == NULL)
				{
						tbl->actions[i] = (psrAction_t*)PSR_ErrorAction;
				}
		}
		__build_expected_list(tbl);
		return tbl;
}


/********************************************************************************************************************************************/


#if(0)

static AR_INLINE void __dump_actions(const psrAction_t *act, arString_t *str)
{
		AR_ASSERT(act != NULL && str != NULL);
		
		AR_AppendString(str, L"{");

		while(act)
		{
				AR_AppendString(str, L"{");
				AR_AppendFormatString(str, L"%" AR_INT_FMT64 L"d", (uint64_t)act->type);
				AR_AppendString(str, L",");
				AR_AppendFormatString(str, L"%" AR_INT_FMT64 L"d", (uint64_t)act->shift_to);
				AR_AppendString(str, L",");
				AR_AppendFormatString(str, L"%" AR_INT_FMT64 L"d", (uint64_t)act->rule_num);
				AR_AppendString(str, L",");
				AR_AppendFormatString(str, L"%" AR_INT_FMT64 L"d", (uint64_t)act->delim);
				AR_AppendString(str, L",");
				AR_AppendFormatString(str, L"%" AR_INT_FMT64 L"d", (uint64_t)act->prec);
				AR_AppendString(str, L",");
				AR_AppendFormatString(str, L"%" AR_INT_FMT64 L"d", (uint64_t)act->reduce_count);
				AR_AppendString(str, L"}");

				if(act->next)
				{
						AR_AppendString(str, L",");
				}

				act = act->next;
		}

		AR_AppendString(str, L"}");
}


static AR_INLINE void __dump_action_tbl(const psrAction_t **act, size_t row, size_t col, arString_t *str)
{
		size_t i;
		AR_ASSERT(act != NULL && str  != NULL && row > 0 && col > 0);

		AR_AppendFormatString(str, L"action : %" AR_INT_FMT64 L"d" L" ," L"%" AR_INT_FMT64 L"d" L" ,", (uint64_t)row, (uint64_t)col);
		
		for(i = 0; i < row * col; ++i)
		{
				__dump_actions(act[i], str);

				if(i < row * col -1)
				{
						AR_AppendString(str, L",");
				}
		}

		AR_AppendString(str, L";\r\n");
}


static AR_INLINE void __dump_goto_tbl(const int_t *goto_tbl, size_t row, size_t col, arString_t *str)
{
		size_t i;
		AR_ASSERT(goto_tbl != NULL && str  != NULL && row > 0 && col > 0);


		AR_AppendFormatString(str, L"goto : %" AR_INT_FMT64 L"d" L" ," L"%" AR_INT_FMT64 L"d" L" , {", (uint64_t)row, (uint64_t)col);
		
		for(i = 0; i < row * col; ++i)
		{
				AR_AppendFormatString(str, L"%" AR_INT_FMT64 L"d ", (int64_t)goto_tbl[i]);

				if(i < row * col -1)
				{
						AR_AppendString(str, L",");
				}
		}

		AR_AppendString(str, L"} ;\r\n");
}


static AR_INLINE void __dump_term_list(const psrSymbList_t *lst, bool_t is_term, arString_t *str)
{
		size_t i;
		
		AR_ASSERT(lst != NULL);

		if(is_term)
		{
				AR_AppendFormatString(str, L"term : ");
		}else
		{
				AR_AppendFormatString(str, L"nonterm : ");
		}

		for(i = 0; i < lst->count; ++i)
		{
				AR_AppendFormatString(str, L"\"%ls\"", lst->lst[i]->name);

				if(i <lst->count - 1)
				{
						AR_AppendString(str, L",");
				}
		}
		AR_AppendFormatString(str, L";\r\n");
}



void					PSR_ActionTableToString(psrActionTable_t *tbl, arString_t *str)
{
		AR_ASSERT(tbl != NULL && str != NULL);
		AR_ClearString(str);

		__dump_term_list(&tbl->term_set, true, str);
		__dump_term_list(&tbl->nonterm_set, false, str);
		__dump_action_tbl(tbl->actions, tbl->row, tbl->col, str);
		__dump_goto_tbl(tbl->goto_tbl, tbl->goto_row, tbl->goto_col, str);
}




psrActionTable_t*		PSR_RestoreActionTable(const wchar_t *txt)
{
		AR_ASSERT(txt);
		
}

#endif


AR_NAMESPACE_END

