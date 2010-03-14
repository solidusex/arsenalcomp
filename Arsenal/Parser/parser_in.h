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
#ifndef __PARSER_IN__H__
#define __PARSER_IN__H__

#include "parser.h"

AR_NAMESPACE_BEGIN



/*typedef struct __parser_symbol_tag psrSymb_t;*/

/*����Ϊ�ս��*/
extern const struct __parser_symbol_tag	*PSR_EOISymb;   /*��ʾ�����β����*/
extern const struct __parser_symbol_tag	*PSR_ErrorSymb;/*����Ĵ��������ʽ����ս����*/
extern const struct __parser_symbol_tag	*PSR_DefPrecSymb;/*�������ս���Ĳ���ʽ��δָ��prec_tok�Ĳ���ʽ����ص��ս��*/

/*���¶�Ϊ���ս��*/
extern const struct __parser_symbol_tag	*PSR_StartSymb;/*��һ�����ţ���ת��Ϊ %Start -> gmr->head[0]*/


/*����parser������صķ��Ŷ������º������䣬�˹���Ϊ���ŶԱ��ṩ��O(1)������(ֻ��Ա�ָ��)*/

#define PSR_STRTBL_BUCKET		(1536)


const wchar_t*	PSR_AllocString(const wchar_t *str);
const wchar_t*	PSR_AllocStringN(const wchar_t *str, size_t n);

const wchar_t*  PSR_AllocStringInt(int64_t num, size_t radix);
const wchar_t*  PSR_AllocStringUInt(uint64_t num, size_t radix);





AR_NAMESPACE_END

#endif

