/* * Copyright (c) 2011 by Solidus * This file is part of Arsenal library tools source code. * * Permission to use, copy, modify, distribute and sell this software * and its documentation for any purpose is hereby granted without fee, * provided that the above copyright notice appear in all copies and * that both that copyright notice and this permission notice appear * in supporting documentation.It is provided "as is" without express  * or implied warranty. * */#import "InputChecker.h"#import "GrammarDesignerDocument.h"@implementation InputSyntaxTreeView-(void)resetFont{		NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];				NSString *name;		NSInteger size;						name = [defaults stringForKey : PREFERENCE_FONT_TREEVIEW_NAME];		size = (NSInteger)[defaults doubleForKey : PREFERENCE_FONT_TREEVIEW_SIZE];				[font release];		font = [NSFont fontWithName : name							   size : size				];				[font retain];				[self setFont : font];}-(void)handleFontChanged : (NSNotification*)note{		DLog(@"InputSyntaxTreeView::handleFontChanged for notification %@", note);		[self resetFont];		}-(void)setHasValidParser : (BOOL)valid{		hasValidParser = valid;}-(void)awakeFromNib{		hasValidParser = NO;		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];		[nc addObserver : self			   selector : @selector(handleFontChanged:)				   name : NOTIFICATION_PREFERENCE_FONT_CHANGED				 object : nil		 ];				[self resetFont];						}-(void)dealloc{		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];		[nc removeObserver : self];				[font release];		font = nil;		[super dealloc];}-(void) expandTree :(id)sender{		NSLog(@"On expandTree");		}-(void) collapseTree :(id)sender{		NSLog(@"On collapseTree");}-(void) clearTree :(id)sender{		NSLog(@"On clearTree");}- (NSMenu *)defaultMenu {		        NSMenu *theMenu = [[[NSMenu alloc] initWithTitle:@"SyntaxTreeView Context Menu"] autorelease];		        [theMenu insertItemWithTitle:@"Expand" action:@selector(expandTree:) keyEquivalent:@"" atIndex:0];		[theMenu insertItemWithTitle:@"Collapse" action:@selector(collapseTree:) keyEquivalent:@"" atIndex:1];			[theMenu insertItem : [NSMenuItem separatorItem]					atIndex : 2		 ];		[theMenu insertItemWithTitle:@"Clear" action:@selector(clearTree:) keyEquivalent:@"" atIndex:3];		        return theMenu;}-(NSMenu *)menuForEvent:(NSEvent *)theEvent {        return [self defaultMenu];  }@end