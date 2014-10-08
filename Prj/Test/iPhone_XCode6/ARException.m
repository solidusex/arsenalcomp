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


#import "ARException.h"


@implementation ARException


+(id)exceptionWithReason : (NSString*)reason
{
		Class c = [self class];
		ARException *expt = [[c alloc] initWithReason : reason];
		[expt autorelease];
		return expt;
}

-(id)init
{
		return [self initWithReason : @"Unknow exception"];
}

-(id)initWithReason : (NSString*)reason
{
		NSString *name = [NSString stringWithFormat : @"%@", [self class]];
		
		
		self = [super initWithName: name
						   reason : reason 
						  userInfo:	nil
				];
		
		return self;
}

-(NSString*)what
{
		return [self reason];
}


@end






IMPL_EXCEPTION(ARRuntimeException)
IMPL_EXCEPTION(ARSystemException)
IMPL_EXCEPTION(ARStringConvertException)	

