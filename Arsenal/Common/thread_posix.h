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
#include "common.h"





AR_NAMESPACE_BEGIN

#if (OS_TYPE == OS_MAC_OS_X || OS_TYPE == OS_IOS)

		#include "thread_posix_apple.h"

#else
		#include "thread_posix_posix.h"
#endif

/**********************************************************************************************************************************end*/

#define RESOLUTION		1000000LL

uint_64_t		AR_GetTime_Microseconds()
{
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return (uint_64_t)tv.tv_sec * (uint_64_t)RESOLUTION + (uint_64_t)tv.tv_usec;
}





AR_NAMESPACE_END


