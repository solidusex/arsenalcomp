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
#include "symbol.h"



AR_NAMESPACE_BEGIN


static volatile arSpinLock_t		__tbl_lock;
static arStringTable_t *__tbl = NULL;

const psrSymb_t	*PSR_EpsilonSymb = NULL;/*��ʾ��*/
const psrSymb_t	*PSR_EOISymb = NULL;   /*��ʾ�����β����*/
const psrSymb_t	*PSR_LALRSymb= NULL; /*��ʾΪ���������ַ�ֵ�����е��ַ�������LALR�﷨������ʱʹ��*/
const psrSymb_t	*PSR_ErrorSymb= NULL;/*����Ĵ��������ʽ����ս����*/
const psrSymb_t	*PSR_DefPrecSymb = NULL;/*�ṩĬ��prec��assoc���Ե��ս��*/

/*���¶�Ϊ���ս��*/
const psrSymb_t	*PSR_StartSymb = NULL;


/******************************************************************************************************************************************/

void	PSR_Init()
{
		AR_InitSpinLock(&__tbl_lock);
		__tbl		= AR_CreateStrTable(PSR_STRTBL_BUCKET);

		PSR_EpsilonSymb =		PSR_CreateSymb(L"%EPSILON", PSR_TERM);
		PSR_EOISymb		=		PSR_CreateSymb(L"%EOI", PSR_TERM);
		PSR_LALRSymb	=		PSR_CreateSymb(L"%LALR", PSR_TERM);
		PSR_ErrorSymb	=		PSR_CreateSymb(L"error", PSR_TERM);
		
		PSR_DefPrecSymb	=		PSR_CreateSymb(L"%PREC_ASSOC", PSR_TERM);

		PSR_StartSymb	=		PSR_CreateSymb(L"%Start", PSR_NONTERM);

		
}

void	PSR_UnInit()
{
		PSR_DestroySymb((psrSymb_t*)PSR_EpsilonSymb);
		PSR_DestroySymb((psrSymb_t*)PSR_EOISymb);
		PSR_DestroySymb((psrSymb_t*)PSR_LALRSymb);
		PSR_DestroySymb((psrSymb_t*)PSR_ErrorSymb);
		PSR_DestroySymb((psrSymb_t*)PSR_DefPrecSymb);
		PSR_DestroySymb((psrSymb_t*)PSR_StartSymb);

		AR_DestroyStrTable(__tbl);
		AR_UnInitSpinLock(&__tbl_lock);
		__tbl = NULL;
}



/***********************************************StringTable************************************/

const wchar_t*	PSR_AllocString(const wchar_t *str)
{
		const wchar_t *res;
		AR_LockSpinLock(&__tbl_lock);
		res = AR_GetString(__tbl, str);
		AR_UnLockSpinLock(&__tbl_lock);
		return res;
}

const wchar_t*	PSR_AllocStringN(const wchar_t *str, size_t n)
{
		const wchar_t *res;
		AR_LockSpinLock(&__tbl_lock);
		res = AR_GetStringN(__tbl, str,n);
		AR_UnLockSpinLock(&__tbl_lock);
		return res;
}


const wchar_t*  PSR_AllocStringInt(int64_t num, size_t radix)
{
		const wchar_t *res;
		AR_LockSpinLock(&__tbl_lock);
		res = AR_GetStringInt(__tbl, num, radix);
		AR_UnLockSpinLock(&__tbl_lock);
		return res;
}

const wchar_t*  PSR_AllocStringUInt(uint64_t num, size_t radix)
{
		const wchar_t *res;
		AR_LockSpinLock(&__tbl_lock);
		res = AR_GetStringUInt(__tbl, num, radix);
		AR_UnLockSpinLock(&__tbl_lock);
		return res;
}


AR_NAMESPACE_END