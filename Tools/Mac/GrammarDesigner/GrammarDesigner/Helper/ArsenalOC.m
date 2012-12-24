/*
 * Copyright (c) 2011 by Solidus
 * This file is part of Arsenal library tools source code.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.It is provided "as is" without express 
 * or implied warranty.
 *
 */


#import "ArsenalOC.h"
#import "ARUtility.h"


static void	AR_STDCALL __default_error_func(ar_int_t level, const wchar_t *msg, void *ctx)
{
		assert(ctx != NULL && msg != NULL);
		NSObject<ArsenalContext> *context = (NSObject<ArsenalContext>*)ctx;


		[context onError : level 
					 msg : [ARUtility convertUTF32ToNSString : msg]
		 ];
		
}


static void	AR_STDCALL __default_print_func(const wchar_t *msg, void *ctx)
{
		assert(ctx != NULL && msg != NULL);
		
		NSObject<ArsenalContext> *context = (NSObject<ArsenalContext>*)ctx;
		
		[context onPrint : [ARUtility convertUTF32ToNSString : msg]];
}


static ArsenalOC *__g_instance = nil;






@implementation ArsenalOC


-(id)init
{
		assert(NO);
}

-(id)initWithContext : (NSObject<ArsenalContext>*)ctx
{
		assert(ctx != nil);
		
		self = [super init];
		
		if(self)
		{
				context = ctx;
				[context retain];
				
				arInit_t init;
				init.global_io_ctx.on_error = __default_error_func;
				init.global_io_ctx.on_print = __default_print_func;
				init.global_io_ctx.ctx = (void*)context;
                init.backtrace.gen_backtrace = NULL;
                init.backtrace.gen_backtrace_sym = NULL;
                
				Arsenal_Init(&init);
		}
		return self;
}

-(void)dealloc
{
		Arsenal_UnInit();
		[super dealloc];
}


+(void) initializeArsenalOC : (NSObject<ArsenalContext>*)ctx
{
		@synchronized(self)
		{
				if(__g_instance == nil)
				{
						__g_instance = [[self alloc] initWithContext : ctx];
				}
		}
}


+(void)			uninitializeArsenalOC
{
		
		@synchronized(self)
		{
				
				if(__g_instance != nil)
				{
						[__g_instance release];
						__g_instance = nil;
				}
		}
}






@end




