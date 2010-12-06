
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


#include "parser_in.h"
#include "lalr.h"

AR_NAMESPACE_BEGIN



static	void	__calc_lr0_closure(lalrConfigList_t *all_config, const psrGrammar_t *grammar, const psrSymbMap_t *first_set, bool_t lr0)
{

		lalrConfigNode_t		*node;
		AR_ASSERT(all_config != NULL && grammar != NULL && first_set != NULL);

		AR_ASSERT(all_config->count > 0);
		for(node = all_config->head; node != NULL; node = node->next)
		{
				const psrRule_t *rule;
				size_t rule_num;
				size_t delim;
				const psrSymb_t *symb;

				rule_num = node->config->rule_num;
				delim = node->config->delim;
				rule = Parser_GetRuleFromGrammar(grammar, rule_num);

				if(delim >= rule->body.count)continue;

				/*
				A -> a . B C;
				�� body[delim] == B������ B -> . a b c;װ�뱾��
				*/
				symb = rule->body.lst[delim];

				if(symb->type == PARSER_NONTERM)
				{
						size_t i;
						
						for(i = 0; i < grammar->count; ++i)
						{
								const psrRule_t *inner_rule;
								size_t			inner_rule_num;
								inner_rule = grammar->rules[i];
								inner_rule_num = i;
								/*����ÿһ���﷨*/

								if(Parser_CompSymb(symb, inner_rule->head) == 0)
								{
										/*
										�����ǰlhs��symb��ͬ���򽫵�ǰ�﷨���� B -> . a b c�����all_config
										*/
										size_t k;
										lalrConfig_t *new_config;
										new_config = Parser_FindFromConfigList(all_config, inner_rule_num, 0);
										if(new_config == NULL)
										{
												new_config = Parser_InsertToConfigListByValue(all_config, inner_rule_num, 0);
										}
										AR_ASSERT(new_config != NULL);

										/*lr0ģʽ�������㴫�����Լ�follow set*/
										if(lr0)continue;
										
										/*
												S : A . B C D
												  ;

												body[delim] == B;
												body[k] == C 
										*/
										for(k = delim + 1; k < rule->body.count; ++k)
										{
												const psrSymb_t *sp = rule->body.lst[k];
												AR_ASSERT(sp != NULL);

												if(sp->type == PARSER_TERM)
												{
														/*
														���spΪ�ս���������������new_config��follow_set�У�֮��ѭ����ֹ����Ϊ��
														�����﷨�����в�����sp֮����ս�����뵽new_config��
														*/
														Parser_InsertToSymbList_Unique(&new_config->follow_set, sp);
														break;
												}else
												{
														/*
														���spΪ���ս��������first_set���뵽new_config�У�������ս���ɵ����մ���
														��ѭ������������ѭ����ֹ
														*/
														const psrMapRec_t *rec;
														size_t x;
														rec = Parser_GetSymbolFromSymbMap(first_set, sp);

														for(x = 0; x < rec->lst.count; ++x)
														{
																Parser_InsertToSymbList_Unique(&new_config->follow_set, rec->lst.lst[x]);
														}

														if(!rec->can_empty)
														{
																break;
														}
												}
										}
												
										/*
										����node->configΪA -> a . B C D;
										��C D���ɵ����մ��� ����A��follow_set���ᴫ����������
										*/
										if(k == rule->body.count)
										{
												Parser_InsertToConfigList(node->config->forward, new_config);
										}
								}
						}
				}

		}

}



static	void	__build_goto(lalrState_t *start, const psrGrammar_t *grammar, lalrStateSet_t *set, const psrSymbMap_t *first_set, bool_t lr0);


static	lalrState_t* __build_state(lalrConfigList_t *basis, const psrGrammar_t *grammar, lalrStateSet_t *set, const psrSymbMap_t *first_set, bool_t lr0)
{
		lalrState_t		*new_state;
		AR_ASSERT(basis != NULL && basis->count > 0 && grammar != NULL && set != NULL && first_set != NULL);
		
		Parser_SortConfigList(basis);
		new_state = Parser_FindStateByBasis(set, basis);

		if(new_state)
		{
				if(!lr0)
				{
						lalrConfigNode_t *l, *r;
						/*���µĺ������backward list copy���Ѵ��ڵ�״̬�������*/
						for(l = basis->head, r = new_state->basis->head; l != NULL && r != NULL; l = l->next, r = r->next)
						{
								Parser_UnionConfigList(r->config->backward, l->config->backward);
						}
				}
				
				/*�������ú�����*/
				Parser_DestroyConfigList(basis, true);
		}else
		{
				lalrConfigList_t *all_config;

				all_config = Parser_CreateConfigList();
				Parser_CopyConfigList(all_config, basis);

				__calc_lr0_closure(all_config, grammar, first_set, lr0);
				Parser_SortConfigList(all_config);
				
				new_state = Parser_CreateState();
				new_state->basis = basis;
				new_state->all_config = all_config;
				Parser_InsertToStateSet(set, new_state);
				
				__build_goto(new_state, grammar, set, first_set, lr0);
		}

		return new_state;
}


static	void	__build_goto(lalrState_t *start, const psrGrammar_t *grammar, lalrStateSet_t *set, const psrSymbMap_t *first_set, bool_t lr0)
{
		lalrConfigNode_t		*node;
		AR_ASSERT(start != NULL && grammar != NULL && set != NULL && first_set != NULL);
		AR_ASSERT(start->basis != NULL && start->all_config != NULL);

		for(node = start->all_config->head; node != NULL; node = node->next)
		{
				node->config->is_completed = false;
		}


		/*����״̬(�)start��ÿһ����*/
		for(node = start->all_config->head; node != NULL; node = node->next)
		{
				lalrState_t				*new_state;
				lalrConfigList_t		*goto_list;
				const psrSymb_t			*symb;
				const psrRule_t			*rule;
				lalrConfigNode_t		*inner_node;
				
				rule = Parser_GetRuleFromGrammar(grammar, node->config->rule_num);

				if(node->config->is_completed)continue;
				if(node->config->delim >= rule->body.count)continue;

				/*A -> a. B c ;
				���ʱ delim == 1,��body[delim] == B;��symb== BΪ��һ��ת��
				*/
				symb = rule->body.lst[node->config->delim]; 

				new_state = NULL;
				goto_list = Parser_CreateConfigList();
				/*����Ϊ��״̬(�)start���������ڷ���symb�ϵ�ת��*/
				for(inner_node = start->all_config->head; inner_node != NULL; inner_node = inner_node->next)
				{
						const psrSymb_t			*bsp;
						lalrConfig_t			*new_config;
						const psrRule_t			*inner_rule;
						
						inner_rule = Parser_GetRuleFromGrammar(grammar, inner_node->config->rule_num);

						if(inner_node->config->is_completed)continue;/*����������ټ���*/

						AR_ASSERT(inner_node->config->delim <= inner_rule->body.count);
						if(inner_node->config->delim == inner_rule->body.count)continue;

						bsp = inner_rule->body.lst[inner_node->config->delim];
						
						if(Parser_CompSymb(symb, bsp) != 0)continue;
						inner_node->config->is_completed = true;

						new_config = Parser_FindFromConfigList(goto_list, inner_node->config->rule_num, inner_node->config->delim + 1);

						if(new_config == NULL)
						{
								new_config = Parser_InsertToConfigListByValue(goto_list, inner_node->config->rule_num, inner_node->config->delim + 1);
						}
						
						if(!lr0)
						{
								/*
										A  :   ( A ) | a
										   ;

										s0 :	[A'		: . A, $]				config0
												[A		: . ( A ), $]			
												[A		: . a, $]
										
										s1 :    [A'		: A . $ ]				//config 1

										s2 :	[A		:	( . A )		, $]	//config 2
												[A		: . ( A )		, )]
												[A		: . a			, )]

										s3 :	[A		:	a .			, $]	//config 3
										
										config0��$��Ҫ���ݵ�config1
										
								*/

								/*
										��¼����inner_node->config ��������symb��ת�Ƶ�new_config,
								���磺s0 -> [A'		: . A, $] ��������Aת�Ƶ�  s1 :    [A'		: A . $ ] 

								*/
								Parser_InsertToConfigList(new_config->backward, inner_node->config);
						}
				}

				new_state = __build_state(goto_list, grammar, set, first_set, lr0);
				
				AR_ASSERT(new_state != NULL);
				
				Parser_InsertAction(start, new_state, symb, node->config);
		}
}



static void __build_slr_actions(lalrStateSet_t *set, const psrGrammar_t *grammar, const psrSymbMap_t *follow_set)
{
		size_t i;
		
		
		AR_ASSERT(set != NULL && set->count > 0 && follow_set != NULL);

		
		for(i = 0; i < set->count; ++i)
		{
				lalrConfigNode_t *node;
				lalrState_t *state = set->set[i];

				for(node = state->all_config->head; node != NULL; node = node->next)
				{
						const psrRule_t *rule = Parser_GetRuleFromGrammar(grammar, node->config->rule_num);
						
						if(node->config->delim == rule->body.count)
						{
								size_t x;
								const psrSymbList_t *lst = &(Parser_GetSymbolFromSymbMap(follow_set, rule->head)->lst);

								for(x = 0; x < lst->count; ++x)
								{
										
										lalrAction_t *action = Parser_InsertAction(state, NULL, lst->lst[x], node->config);

										if(Parser_CompSymb(rule->head, PARSER_StartSymb) == 0)
										{
												action->act_type = LALR_ACT_ACCEPT;
										}
								}
						}
				}
		}

		
}



lalrState_t*	Parser_Create_LR0_State(const psrGrammar_t *grammar)
{
		psrSymbMap_t			first_set, follow_set;
		lalrState_t				*start;
		lalrConfigList_t		*basis;
		lalrConfig_t			*first_cfg;
		lalrStateSet_t			set;
		AR_ASSERT(grammar != NULL);
		
		
		Parser_InitSymbMap(&first_set);
		Parser_InitSymbMap(&follow_set);
		Parser_CalcFirstSet(grammar, &first_set);
		Parser_CalcFollowSet(grammar, &follow_set, &first_set);

		Parser_InitStateSet(&set);

		basis = Parser_CreateConfigList();

		first_cfg = Parser_InsertToConfigListByValue(basis, 0, 0);
		
		Parser_InsertToSymbList(&first_cfg->follow_set, PARSER_EOISymb);

		start = __build_state(basis, grammar, &set, &first_set, true);

		__build_slr_actions(&set, grammar, &follow_set);

		Parser_UnInitSymbMap(&follow_set);
		Parser_UnInitSymbMap(&first_set);
		Parser_UnInitStateSet(&set);

		return start;
}







static void __build_propagation_links(lalrStateSet_t *set)
{
		size_t i;
		bool_t changed;
		
		AR_ASSERT(set != NULL);
		
		for(i = 0; i < set->count; ++i)
		{
				lalrConfigNode_t *node;
				lalrState_t *state = set->set[i];

				for(node = state->all_config->head; node != NULL; node = node->next)
				{
						lalrConfigNode_t		*bp;
						lalrConfig_t			*config;
						config = node->config;
						for(bp = node->config->backward->head; bp != NULL; bp = bp->next)
						{
								lalrConfig_t			*other = bp->config;
								Parser_InsertToConfigList(other->forward, config);
						}
				}
		}


		for(i = 0; i < set->count; ++i)
		{
				lalrConfigNode_t *node;
				lalrState_t *state = set->set[i];

				for(node = state->all_config->head; node != NULL; node = node->next)
				{
						node->config->is_completed = false;
				}
		}
		
		/*��ʱ���һ�������·�ʽ�Ż��˺�����
		
		�˺��������Ż�������ÿ��symbol��Ӧ������һ��ID����0->N��֮��ÿ��config���ڴ洢follow_set�ֶΣ����洢һ��bit���飬
		��ʱ������Ȧѭ����Parser_InsertToSymbList_Unique��O(N)����O(1)���Ҵ洢Ҳ�����Խ���
		*/
		do{
				changed = false;

				for(i = 0; i < set->count; ++i)
				{
						lalrConfigNode_t *node;
						lalrState_t *state = set->set[i];

						for(node = state->all_config->head; node != NULL; node = node->next)
						{
								lalrConfigNode_t *fp;
								
								if(node->config->is_completed)continue;

								for(fp = node->config->forward->head; fp != NULL; fp = fp->next)
								{
										size_t x;
										lalrConfig_t *next_config = fp->config;
										
										for(x = 0; x < node->config->follow_set.count; ++x)
										{
												const psrSymb_t *symb = node->config->follow_set.lst[x];
												if(Parser_InsertToSymbList_Unique(&next_config->follow_set, symb))
												{
														changed = true;
														next_config->is_completed = false;
												}
										}
								}

								node->config->is_completed = true;
						}
				}
		}while(changed);
}



static void __build_actions(lalrStateSet_t *set, const psrGrammar_t *grammar)
{
		size_t i;
		AR_ASSERT(set != NULL && set->count > 0);

		for(i = 0; i < set->count; ++i)
		{
				lalrConfigNode_t *node;
				lalrState_t *state = set->set[i];

				for(node = state->all_config->head; node != NULL; node = node->next)
				{
						psrRule_t *rule = Parser_GetRuleFromGrammar(grammar, node->config->rule_num);
						/*
						node->config->delim == rule->body.count�����˴���Ϊ��Լ״̬
						*/

						if(node->config->delim == rule->body.count)
						{
								size_t x;
								for(x = 0; x < node->config->follow_set.count; ++x)
								{
										lalrAction_t *action = Parser_InsertAction(state, NULL, node->config->follow_set.lst[x], node->config);

										if(Parser_CompSymb(rule->head, PARSER_StartSymb) == 0)
										{
												action->act_type = LALR_ACT_ACCEPT;
										}
								}
						}
				}
		}
}


lalrState_t*	Parser_Create_LALR_State(const psrGrammar_t *grammar)
{

		psrSymbMap_t			first_set;
		lalrState_t				*start;
		lalrConfigList_t		*basis;
		lalrConfig_t			*first_cfg;
		lalrStateSet_t			set;
		AR_ASSERT(grammar != NULL);
		
		
		Parser_InitSymbMap(&first_set);
		Parser_CalcFirstSet(grammar, &first_set);
		Parser_InitStateSet(&set);

		basis = Parser_CreateConfigList();

		first_cfg = Parser_InsertToConfigListByValue(basis, 0,  0);
		
		Parser_InsertToSymbList(&first_cfg->follow_set, PARSER_EOISymb);

		start = __build_state(basis, grammar, &set, &first_set, false);

		__build_propagation_links(&set);
		__build_actions(&set, grammar);
		Parser_UnInitSymbMap(&first_set);
		Parser_UnInitStateSet(&set);

		return start;
}


AR_NAMESPACE_END
