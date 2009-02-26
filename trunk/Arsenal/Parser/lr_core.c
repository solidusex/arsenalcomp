#include "lr.h"


/********************************************************LR0�Զ���*******************************************************/





static psrLRItem_t* __lr0_closure(const psrLRItem_t *itemset, const psrGrammar_t *grammar)
{
		psrLRItem_t *closure;
		const psrLRItem_t *item;
		bool_t changed;
		AR_ASSERT(grammar != NULL);
		
		if(itemset == NULL)return NULL;

		closure = PSR_CopyNewLRItemList(itemset);
		
		changed = True;
		while(changed)
		{
				changed = False;

		
				for(item = closure; item != NULL; item = item->next)
				{
						const psrSymb_t *head, *body;
						head = PSR_GetRuleHeadByRuleID(grammar, item->rule_id);
						body = PSR_GetRuleBodyByRuleID(grammar, item->rule_id);
						AR_ASSERT(head != NULL && body != NULL);
						
						body = PSR_IndexOfSymbList(body, item->delim);
						
						if(body == NULL)continue;/*��Լ״̬*/
						
						if(!body->non_term)
						{
								if(PSR_IsNullable(body))
								{
										psrLRItem_t new_item;
										new_item = *item;
										new_item.delim = item->delim + 1;
										new_item.next = NULL;
										new_item.symb = NULL;

										if(PSR_InsertLRItem_Unique(&closure, &new_item))
										{
												changed = True;
										}
								}
						}else
						{
								size_t i;
								for(i = 0; i < grammar->count; ++i)
								{
										if(PSR_CompSymb(body, grammar->head[i]) == 0)
										{
												psrLRItem_t new_item;
												AR_MSET0(&new_item, sizeof(new_item));
												new_item.rule_id = grammar->head[i]->id;
												new_item.delim = 0;
												

												
												if(PSR_IsNullable(grammar->body[i]))
												{
														new_item.delim += 1;
														AR_ASSERT(grammar->body[i]->next == NULL || !PSR_IsNullable(grammar->body[i]->next));
												}

												if(PSR_InsertLRItem_Unique(&closure, &new_item))
												{
														changed = True;
												}
										}
								}
						}
				}
		}
		
		return closure;

}


static psrLRItem_t* __lr0_goto(const psrLRItem_t *itemset, const psrSymb_t *symb, const psrGrammar_t *grammar)
{
		psrLRItem_t *dest, *result;
		psrLRItem_t	 new_item;
		const psrLRItem_t *curr;
		
		dest = NULL;
		for(curr = itemset; curr != NULL; curr = curr->next)
		{
				const psrSymb_t *head, *next_symb;

				head = PSR_GetRuleHeadByRuleID(grammar, curr->rule_id);
				next_symb = PSR_GetRuleBodyByRuleID(grammar, curr->rule_id);
				
				AR_ASSERT(head != NULL && next_symb != NULL);

				next_symb = PSR_IndexOfSymbList(next_symb, curr->delim);

				if(next_symb == NULL)continue;

				if(PSR_CompSymb(next_symb, symb) == 0)
				{
						new_item = *curr;
						new_item.next = NULL;
						new_item.symb = NULL;
						new_item.delim += 1;
						PSR_InsertLRItem_Unique(&dest, &new_item);
				}
		}
		result = __lr0_closure(dest, grammar);
		PSR_DestroyLRItemList(dest);
		return result;
}

lrItemsCollection_t* LR0_CreateItemsCollection(const psrGrammar_t *grammar)
{
		lrItemsCollection_t* clt;
		psrLRItem_t first, *tmp;
		bool_t changed;

		clt = AR_NEW0(lrItemsCollection_t);
		
		AR_MSET0(&first, sizeof(first));
		first.rule_id = grammar->head[0]->id;
		first.delim = 0;
		
		tmp = __lr0_closure(&first, grammar);

		LR_InsertToItemsCollection_Unique(clt, tmp);
		
		changed = True;
		while(changed)
		{
				size_t i;
				changed = False;
				for(i = 0; i < clt->count; ++i)
				{
						const psrLRItem_t *items;
						const psrSymb_t	  *symbol;
						
						items = clt->items[i];
						
						for(symbol = grammar->symb_list; symbol != NULL; symbol = symbol->next)
						{
								psrLRItem_t *new_items; 
								
								new_items = __lr0_goto(items, symbol, grammar);
								
								if(new_items == NULL)continue;

								
								
								if(LR_InsertToItemsCollection_Unique(clt, new_items))
								{
										size_t new_state;
										changed = True;
										new_state = clt->count-1;
										LR_InsertToGotoMap(&clt->goto_map[i], symbol, new_state);
								}else
								{
										int idx;
										idx = LR_FindItemsFromCollection(clt, new_items);
										LR_InsertToGotoMap(&clt->goto_map[i], symbol, (size_t)idx);
								}
								
								PSR_DestroyLRItemList(new_items);
						}
				}
		}
		
		return clt;
}




/***********************************************LR(1)�Զ���****************************************************/
/*���������follow_body��first���ϣ����follow_body��ÿ�����Ŷ������գ���lookahead���뵽first�����У�����ͷ���follow_body��first����*/
static psrSymb_t*  __lr1_calc_first(const psrSymb_t *follow_body, const psrSymb_t *lookahead, const psrGrammar_t *gmr)
{
		const psrSymb_t *curr;
		psrSymb_t		*first;
		bool_t has_epsilon;
		AR_ASSERT(lookahead != NULL && gmr != NULL);
		AR_ASSERT(!lookahead->non_term);
		
		first = NULL;
		
		has_epsilon = True;
		for(curr = follow_body; curr; curr = curr->next)
		{
				const psrSymb_t *fset = PSR_GetFirstSetBySymb(gmr, curr);
				has_epsilon = False;
				for(fset; fset; fset = fset->next)
				{
						if(PSR_CompSymb(fset, PSR_EpsilonSymb) == 0)
						{
								has_epsilon = True;
						}else
						{
								PSR_InsertSymb_Unique(&first, fset);
						}
				}
				
				if(!has_epsilon)
				{
						return first;
				}
		}
		
		if(has_epsilon)
		{
				PSR_InsertSymb_Unique(&first, lookahead);
		}
		return first;
}


static psrLRItem_t* __lr1_closure(const psrLRItem_t *itemset, const psrGrammar_t *grammar)
{
		psrLRItem_t *closure;
		const psrLRItem_t *item;
		bool_t changed;
		AR_ASSERT(grammar != NULL);
		
		if(itemset == NULL)return NULL;

		closure = PSR_CopyNewLRItemList(itemset);
		
		changed = True;
		while(changed)
		{
				changed = False;

		
				for(item = closure; item != NULL; item = item->next)
				{
						
						const psrSymb_t *head, *body;
						head = PSR_GetRuleHeadByRuleID(grammar, item->rule_id);
						body = PSR_GetRuleBodyByRuleID(grammar, item->rule_id);
						AR_ASSERT(head != NULL && body != NULL);
						
						body = PSR_IndexOfSymbList(body, item->delim);
						
						if(body == NULL)continue;/*��Լ״̬*/

						AR_ASSERT(item->symb != NULL && !PSR_IsNullable(item->symb));

						if(!body->non_term)
						{
								/*
										�����ǰ��Ϊ(A->.%Epsilon��a)
										��ǰlookahead����Ϊʲô������
										(A->%Epsilon., a)��������Ϊ��������հ����������Ҫ��
										��ת��ɾ���������delimֱ��+=1����lookahead�����ճ�����
								*/

								if(PSR_IsNullable(body))
								{
										psrLRItem_t new_item;
										new_item = *item;
										new_item.delim += 1;
										new_item.symb = PSR_CopyNewSymb(item->symb);
										/*
										    ������ܽ�new_item�嵽��ǰitem��ǰ��ȥ���������޹ؽ�Ҫ����Ϊÿ�γɹ��Ĳ��붼�����¼���������
										*/
										if(PSR_InsertLRItem_Unique(&closure, &new_item))
										{
												changed = True;
										}
										PSR_DestroySymb(new_item.symb);
								}
						}else
						{
						
								size_t i;
								for(i = 0; i < grammar->count; ++i)
								{
										if(PSR_CompSymb(body, grammar->head[i]) == 0)
										{

												psrSymb_t *firstset;
												const psrSymb_t *curr_first;

												firstset = __lr1_calc_first(body->next, item->symb, grammar);

												AR_ASSERT(firstset != NULL);

												for(curr_first = firstset; curr_first; curr_first = curr_first->next)
												{

														psrLRItem_t new_item;
														AR_MSET0(&new_item, sizeof(new_item));
														new_item.rule_id = grammar->head[i]->id;
														new_item.delim = 0;
														new_item.symb = PSR_CopyNewSymb(curr_first);
														new_item.next = NULL;

														if(PSR_IsNullable(grammar->body[i]))
														{
																/*ͬ��ע��*/
																new_item.delim += 1;
																AR_ASSERT(grammar->body[i]->next == NULL || !PSR_IsNullable(grammar->body[i]->next));
														}

														if(PSR_InsertLRItem_Unique(&closure, &new_item))
														{
																changed = True;
														}
														PSR_DestroySymb(new_item.symb);
												}
												PSR_DestroySymbList(firstset);
										}
								}
						}
				}
		}

		return closure;

}


/*�����ʵ��lr0ûɶ������*/
static psrLRItem_t* __lr1_goto(const psrLRItem_t *itemset, const psrSymb_t *symb, const psrGrammar_t *grammar)
{
		
		psrLRItem_t *dest, *result;
		psrLRItem_t	 new_item;
		const psrLRItem_t *curr;
		
		dest = NULL;
		for(curr = itemset; curr != NULL; curr = curr->next)
		{
				const psrSymb_t *head, *next_symb;

				head = PSR_GetRuleHeadByRuleID(grammar, curr->rule_id);
				next_symb = PSR_GetRuleBodyByRuleID(grammar, curr->rule_id);
				
				AR_ASSERT(head != NULL && next_symb != NULL);

				next_symb = PSR_IndexOfSymbList(next_symb, curr->delim);

				if(next_symb == NULL)continue;

				if(PSR_CompSymb(next_symb, symb) == 0)
				{
						new_item = *curr;
						new_item.next = NULL;
						new_item.symb = PSR_CopyNewSymb(curr->symb);
						new_item.delim += 1;
						PSR_InsertLRItem_Unique(&dest, &new_item);
						PSR_DestroySymb(new_item.symb);
				}
		}
		result = __lr1_closure(dest, grammar);
		PSR_DestroyLRItemList(dest);
		return result;
}



lrItemsCollection_t* LR1_CreateItemsCollection(const psrGrammar_t *grammar)
{
		lrItemsCollection_t* clt;
		psrLRItem_t first, *tmp;
		bool_t changed;

		clt = AR_NEW0(lrItemsCollection_t);
		
		AR_MSET0(&first, sizeof(first));
		first.rule_id = grammar->head[0]->id;
		first.delim = 0;
		first.symb = (psrSymb_t*)PSR_EOISymb;
		
		tmp = __lr1_closure(&first, grammar);

		LR_InsertToItemsCollection_Unique(clt, tmp);
		
		changed = True;
		while(changed)
		{
				size_t i;
				changed = False;
				for(i = 0; i < clt->count; ++i)
				{
						const psrLRItem_t *items;
						const psrSymb_t	  *symbol;
						
						items = clt->items[i];
						
						for(symbol = grammar->symb_list; symbol != NULL; symbol = symbol->next)
						{
								psrLRItem_t *new_items; 
								new_items = __lr1_goto(items, symbol, grammar);
								if(new_items == NULL)continue;

								if(LR_InsertToItemsCollection_Unique(clt, new_items))
								{
										size_t new_state;
										changed = True;
										new_state = clt->count-1;
										LR_InsertToGotoMap(&clt->goto_map[i], symbol, new_state);
								}else
								{
										int idx;
										idx = LR_FindItemsFromCollection(clt, new_items);
										LR_InsertToGotoMap(&clt->goto_map[i], symbol, (size_t)idx);

								}
								PSR_DestroyLRItemList(new_items);
						}
				}
		}
		
		return clt;
}




/************************************************LALR�Զ���***************************************************************/
/*
bool_t __lalr_insert_symb_to_lalritem(psrLRItem_t *itemset, const psrLRItem_t *key, const psrSymb_t *symb)
{
		psrLRItem_t *curr;
		for(curr = itemset; curr; curr = curr->next)
		{
				if(curr->rule_id == key->rule_id && curr->delim == key->delim)
				{
						return PSR_InsertSymb_Unique(&curr->symb, symb);
				}
		}
		AR_ASSERT(0);
		return False;
}
*/
/*����һ����Լ�������input�ϵ�ת��*/
static bool_t __insert_to_lalr_collection(size_t state, const psrSymb_t *input, lrItemsCollection_t *clt, const psrLRItem_t *key, const psrSymb_t *symb)
{
		int idx;
		psrLRItem_t *item;
		idx = LR_FindStateFromGotoMap(&clt->goto_map[state], input);
		AR_ASSERT(idx != -1);
		
		/*idxΪ�state��input�ϵ�ת��*/
		for(item = clt->items[idx]; item; item = item->next)
		{
				if(item->rule_id == key->rule_id && item->delim == key->delim)
				{
						/*���ĳ��item��key��LR0�������Ϣ��ͬ����lookahead�������ӵ�������
						*/
						break;
				}
		}
		
		if(item != NULL)
		{
				return PSR_InsertSymb_Unique(&item->symb, symb);
		}else
		{
				AR_ASSERT(0);
				return False;
		}
}


/*
�Ҹо��ü򵥵ļ�¼������������߼������ȣ����Ĺ����Ǽ����lr0�state,��symb�ϵ�ת�Ƶ������ɵ���Щ���ţ��Լ������״̬�Ƿ�̳���state�µ�lookahead���ţ�
ע�⣬��Ϊlr1�ıհ���ϸ��¼��ÿһ������ʽ(rule_id)������ķ��ţ�lalrֻ�ǽ����ǻ㼯��һ��������Ҳ���ʽ����˼��A->a|b;Ϊ��������ʽ����


����㷨��ϸ���ǣ���һ�������ռ����еķ���PSR_LALRSymb,��LR1��{rule_id, delim, PSR_LALRSymb}��LR1�հ������LALRSymb������ıհ���Ҵ˱հ���delim + 1Ϊstate��symb�ϵ�LR0ת��
�򽫷������lalr lookahead�����б�copy��ת�ƺ�����lookahead�б������֤�������������ת���ϴ����Է����ɵķ��ţ��򽫴˷��Ų��뵽ת�ƺ�����lookahead�б���

*/

static bool_t calc_symbol_transfer(size_t state, lrItemsCollection_t *clt, const psrSymb_t *symb,  const psrGrammar_t *grammar)
{
		psrLRItem_t *item;
		bool_t res;
		res = False;
		/*����state��������*/
		for(item = clt->items[state]; item; item = item->next)
		{
				psrLRItem_t tmp;
				psrLRItem_t *closure;
				const psrLRItem_t *curr;
				tmp = *item;
				tmp.next = NULL; 
				tmp.symb = (psrSymb_t*)PSR_LALRSymb;
				closure = __lr1_closure(&tmp, grammar);/*��ÿ������lr1�հ�*/

				for(curr = closure; curr; curr = curr->next)
				{
						const psrSymb_t *body;
						body = PSR_GetRuleBodyByRuleID(grammar,curr->rule_id);
						body = PSR_IndexOfSymbList(body, curr->delim);

						if(body == NULL)continue;/*��ת����*/


						if(PSR_CompSymb(body, symb) == 0)/*����õ��ıհ��е�ĳ���������symb�ϵ�ת�ƣ�������������Է����ɵķ���*/
						{
								
								psrLRItem_t new_item;
								new_item = *curr;
								new_item.delim += 1;
								new_item.next = NULL;
								new_item.symb = NULL;
								
								if(PSR_CompSymb(curr->symb, PSR_LALRSymb) != 0)/*������Է����ս��������뵽��Ӧ�����lookahead�б���*/
								{
										if(__insert_to_lalr_collection(state, symb, clt, &new_item, curr->symb))
										{
												res = True;
										}
								}else/*����Ҫ��������ʼ�item)�е�lookahead�б��еķ���copy����state��symb��ת���е������Ӧ����*/
								{
										const psrSymb_t *symb_tmp;
										
										for(symb_tmp = item->symb; symb_tmp; symb_tmp = symb_tmp->next)
										{
												if(__insert_to_lalr_collection(state, symb, clt, &new_item, symb_tmp))
												{
														res = True;
												}
										}
								}
						}
				}
				PSR_DestroyLRItemList(closure);
		}
		return res;
}





/*�ظ�����calc_symbol_transfer�� ֱ��û��������ż���*/
lrItemsCollection_t* LALR_CreateItemsCollection(const psrGrammar_t *grammar)
{
		lrItemsCollection_t *result;
		size_t state;
		bool_t changed;
		psrLRItem_t *item;
		
		result = LR0_CreateItemsCollection(grammar);
		
		LR_ClearNonKernelItems(result);
		
		
		
		
		for(item = result->items[0]; item; item = item->next)
		{
				if(item->rule_id == 0 && item->next == 0)/* %Start->xxx�е�lookahead(End of input)������ǰ�����ȥ*/ 
				{
						item->symb = PSR_CopyNewSymb(PSR_EOISymb);
						break;
				}
		}

		

		changed = True;

		while(changed)/*���ϵļ��㲢�����Է����ɵķ��ţ�ֱ��û�пɼ����*/
		{
				changed = False;
#if(0)
				for(state = 0; state < result->count; ++state)
				{
						const psrSymb_t *symb;
						for(symb = grammar->symb_list; symb; symb = symb->next)
						{
								if(calc_symbol_transfer(state, result, symb, grammar))
								{
										changed = True;
								}
						}

				}
#endif
				/*�����Ǹ��汾ÿ��ѭ���ж���������з��ż���һ��lr1�հ������������ͦ�˷ѵģ�����������Ժ�úú��Ż�*/
				for(state = 0; state < result->count; ++state)
				{		
						size_t i;
						for(i = 0; i < result->goto_map[state].count; i++)
						{
								if(calc_symbol_transfer(state, result, &result->goto_map[state].symb[i], grammar))
								{
										changed = True;
								}
						}
				}
		}


		
		

		return result;
}

