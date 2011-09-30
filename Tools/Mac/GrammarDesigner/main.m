/* * Copyright (c) 2011 by Solidus * This file is part of Arsenal library tools source code. * * Permission to use, copy, modify, distribute and sell this software * and its documentation for any purpose is hereby granted without fee, * provided that the above copyright notice appear in all copies and * that both that copyright notice and this permission notice appear * in supporting documentation.It is provided "as is" without express  * or implied warranty. * */#import <Cocoa/Cocoa.h>#import "stdheader.h"@interface DefaultARContext		:		NSObject<ArsenalContext>-(void)onPrint	:		(NSString*) msg;-(void)onError	:		(NSInteger)level  msg : (NSString*)msg;@end@implementation DefaultARContext-(void)onPrint	:		(NSString*) msg{		assert(msg);		DLog(@"%@", msg);}-(void)onError	:		(NSInteger)level  msg : (NSString*)msg{		assert(msg);		DLog(@"Level == %d : msg == %@", (int)level, msg);				}@endint main(int argc, char *argv[]){		NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];								DefaultARContext *ctx = [[DefaultARContext alloc] init];		[ArsenalOC initializeArsenalOC : ctx];		[ctx release];				int ret = NSApplicationMain(argc, (const char **) argv);		[ArsenalOC uninitializeArsenalOC];				[pool drain];				return ret;}