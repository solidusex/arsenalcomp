/* * Copyright (c) 2011 by Solidus * This file is part of Arsenal library tools source code. * * Permission to use, copy, modify, distribute and sell this software * and its documentation for any purpose is hereby granted without fee, * provided that the above copyright notice appear in all copies and * that both that copyright notice and this permission notice appear * in supporting documentation.It is provided "as is" without express  * or implied warranty. * */#import "OutputView.h"@implementation OutputView-(void)resetFont{		NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];				NSString *name;		NSInteger size;						name = [defaults stringForKey : PREFERENCE_FONT_CONSOLE_NAME];		size = (NSInteger)[defaults doubleForKey : PREFERENCE_FONT_CONSOLE_SIZE];				[font release];		font = [NSFont fontWithName : name							   size : size				];				[font retain];				[self setFont : font];}-(void)handleFontChanged : (NSNotification*)note{		DLog(@"OutputView::handleFontChanged for notification %@", note);		[self resetFont];		}-(void)onDoubleClicked : (id)sender{		DLog(@"On OutputView::onDoubleClicked");				NSInteger idx = [self selectedRow];				if(idx >= 0 && idx < [msgArray count])		{				ConsoleMessage *msg = [msgArray objectAtIndex : idx];				assert(msg != nil);								DLog(@"OutputView Sending Notification : %@", NOTIFICATION_OUTPUT_INVOKE_RELOCATE_TEXT);								NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];				[nc postNotificationName :	NOTIFICATION_OUTPUT_INVOKE_RELOCATE_TEXT 								  object :  self								userInfo : [NSDictionary dictionaryWithObject : msg 																	   forKey : @"UserInfo"											]				 ];										}}-(void)awakeFromNib{		msgArray = [[NSMutableArray alloc] init];		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];		[nc addObserver : self			   selector : @selector(handleFontChanged:)				   name : NOTIFICATION_PREFERENCE_FONT_CHANGED				 object : nil		 ];				[self resetFont];				[self setDataSource :  self];				[self setDoubleAction : @selector(onDoubleClicked:)];		}-(void)dealloc{		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];		[nc removeObserver : self];				[font release];		font = nil;		[msgArray  release];		msgArray = nil;				[super dealloc];}-(void)append : (ConsoleMessage*)msg{		assert(msg != nil);		[msgArray addObject : msg];}-(void)clear{		[msgArray removeAllObjects];		[self reloadData];}-(NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView{		NSInteger n = [msgArray count];		return n;}-(id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex{		ConsoleMessage *msg = [msgArray objectAtIndex : rowIndex];		return msg.msg;		}-(void) clearOutput : (id)sender{		DLog(@"On OutputView::clearOutput");		[self clear];}-(void) copyOutput : (id)sender{		DLog(@"On OutputView::copyOutput not implement");		}- (NSMenu *)defaultMenu {		        NSMenu *theMenu = [[[NSMenu alloc] initWithTitle:@"Output View Context Menu"] autorelease];				[theMenu addItemWithTitle:@"Copy" action:@selector(copyOutput:) keyEquivalent:@""];        [theMenu addItemWithTitle:@"Clear" action:@selector(clearOutput:) keyEquivalent:@""];						        return theMenu;}-(NSMenu *)menuForEvent:(NSEvent *)theEvent {        return [self defaultMenu];  		}@end