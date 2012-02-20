/* * Copyright (c) 2011 by Solidus * This file is part of Arsenal library tools source code. * * Permission to use, copy, modify, distribute and sell this software * and its documentation for any purpose is hereby granted without fee, * provided that the above copyright notice appear in all copies and * that both that copyright notice and this permission notice appear * in supporting documentation.It is provided "as is" without express  * or implied warranty. * */#import "ActionView.h"@implementation ActionView-(void)resetFont{		NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];				NSString *name;		NSInteger size;						name = [defaults stringForKey : PREFERENCE_FONT_CONSOLE_NAME];		size = (NSInteger)[defaults doubleForKey : PREFERENCE_FONT_CONSOLE_SIZE];				[font release];		font = [NSFont fontWithName : name							   size : size				];				[font retain];						NSLayoutManager *layout = [[NSLayoutManager alloc] init];				[self setRowHeight: 		 [layout defaultLineHeightForFont : font ] + 2		 ];						[self setFont:font];		[self reloadData];				[layout release];}- (void)setFont:(NSFont *)aFont;{		NSArray *tableColumns;		unsigned int columnIndex;				tableColumns = [self tableColumns];		columnIndex = [tableColumns count];				while (columnIndex--)		{				[ [ (NSTableColumn *)[tableColumns objectAtIndex:columnIndex] dataCell] 				 setFont:aFont				 ];		}}-(void)handleFontChanged : (NSNotification*)note{		DLog(@"ActionView::handleFontChanged for notification %@", note);		[self resetFont];		}-(void)awakeFromNib{				columnsData = [[NSMutableArray alloc]init];				NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];		[nc addObserver : self			   selector : @selector(handleFontChanged:)				   name : NOTIFICATION_PREFERENCE_FONT_CHANGED				 object : nil		 ];		[self resetFont];		[self clear];				[self setDataSource :  self];		}-(void)dealloc{		[self clear];		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];		[nc removeObserver : self];				[font release];		font = nil;		[columnsData release];		columnsData = nil;		[super dealloc];}-(void)clear{		[columnsData removeAllObjects];		while(YES)		{				NSArray *columns = [self tableColumns];								if([columns count] == 0)				{						break;				}												[				 self removeTableColumn  : 	 [columns objectAtIndex : 0]				 ];		}}-(NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView{				NSInteger n = [columnsData count];				if(n > 0)		{				NSArray *rows = [columnsData objectAtIndex : 0];				n = [rows count];		}				return n;}-(id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex{		int idx = [self columnWithIdentifier : [aTableColumn identifier]];				if(idx >= 0)		{				NSArray *col_data = [columnsData objectAtIndex : idx];								return [col_data objectAtIndex : rowIndex];		}else		{				return nil;		}}-(void)	drawFirstFollowView : (const psrStatusView_t*) view{		[self clear];				if(view == NULL)		{				return;		}				NSTableColumn *col_name = [[NSTableColumn alloc] initWithIdentifier : @"0"];		NSTableColumn *col_set = [[NSTableColumn alloc] initWithIdentifier : @"1"];		[col_name setEditable : NO];		[col_set setEditable : NO];				id hc = [col_name headerCell];		[hc setStringValue : @"Name"];				hc = [col_set headerCell];		[hc setStringValue : @"Set"];				NSRect rect = [self frame];						[col_name setWidth : rect.size.width / 3 * 1];		[col_set setWidth : rect.size.width / 3 * 2];				[self addTableColumn : col_name];		[self addTableColumn : col_set];				NSMutableArray *name_row = [[NSMutableArray alloc] init];		NSMutableArray *set_row = [[NSMutableArray alloc] init];				[name_row addObject : @"First Set:"];		[set_row addObject : @" "];				size_t i;		for(i = 0; i < view->first_set.count; ++i)		{				[name_row addObject : [ARUtility convertUTF32ToNSString : view->first_set.name[i]]				 ];								[set_row addObject : [ARUtility convertUTF32ToNSString : view->first_set.name_set[i]]				 ];		}				[name_row addObject : @" "];		[set_row addObject : @" "];		[name_row addObject : @" "];		[set_row addObject : @" "];				[name_row addObject : @" "];		[set_row addObject : @" "];				[name_row addObject : @"Follow Set:"];		[set_row addObject : @" "];						for(i = 0; i < view->follow_set.count; ++i)		{				[name_row addObject : [ARUtility convertUTF32ToNSString : view->follow_set.name[i]]				 ];								[set_row addObject : [ARUtility convertUTF32ToNSString : view->follow_set.name_set[i]]				 ];		}				[columnsData addObject : name_row];		[columnsData addObject : set_row];				[name_row release];		[set_row release];				[self reloadData]; }-(void)	drawLeftRecursionView : (const psrStatusView_t*) view{		[self clear];								[self clear];				if(view == NULL)		{				return;		}				NSTableColumn *col_path = [[NSTableColumn alloc] initWithIdentifier : @"0"];				[col_path setEditable : NO];				id hc = [col_path headerCell];		[hc setStringValue : @"Path"];						NSRect rect = [self frame];		[col_path setWidth : rect.size.width];				[self addTableColumn : col_path];								NSMutableArray *path_row = [[NSMutableArray alloc] init];						for(size_t i = 0; i < view->left_recursion.count; ++i)		{				[path_row addObject : [ARUtility convertUTF32ToNSString : view->left_recursion.name_set[i]]];		}						[columnsData addObject : path_row];		[path_row release];				[self reloadData]; 		}-(void)	drawLeftFactorView : (const psrStatusView_t*) view{				[self clear];				if(view == NULL)		{				return;		}				NSTableColumn *col_name = [[NSTableColumn alloc] initWithIdentifier : @"0"];		NSTableColumn *col_val = [[NSTableColumn alloc] initWithIdentifier : @"1"];				[col_name setEditable : NO];		[col_val setEditable : NO];				[[col_name headerCell] setStringValue : @"LHS"];		[[col_val headerCell] setStringValue : @"Rule"];				NSRect rect = [self frame];						[col_name setWidth : rect.size.width / 3 * 1];		[col_val setWidth : rect.size.width / 3 * 2];				[self addTableColumn : col_name];		[self addTableColumn : col_val];						NSMutableArray *name_row = [[NSMutableArray alloc] init];		NSMutableArray *val_row = [[NSMutableArray alloc] init];								const wchar_t *curr_head = NULL;		for(size_t i = 0; i < view->left_factor.count; ++i)		{				if(curr_head == NULL)				{						curr_head = view->left_factor.name[i];												[name_row addObject : [ARUtility convertUTF32ToNSString : curr_head]];				}else if(wcscmp(curr_head, view->left_factor.name[i]) != 0)				{						curr_head = view->left_factor.name[i];						[name_row addObject : [ARUtility convertUTF32ToNSString : curr_head]];				}else				{						[name_row addObject : @" "];				}				[val_row addObject : [ARUtility convertUTF32ToNSString : view->left_factor.name_set[i]]];		}		[columnsData addObject : name_row];		[columnsData addObject : val_row];				[name_row release];		[val_row release];				[self reloadData]; }-(void)	drawConflictView : (const psrConflictView_t*) view{		[self clear];				if(view == NULL)		{				return;		}				NSTableColumn *col_name = [[NSTableColumn alloc] initWithIdentifier : @"0"];		NSTableColumn *col_val = [[NSTableColumn alloc] initWithIdentifier : @"1"];				[col_name setEditable : NO];		[col_val setEditable : NO];				[[col_name headerCell] setStringValue : @"Name"];		[[col_val headerCell] setStringValue : @"Conflict"];				NSRect rect = [self frame];						[col_name setWidth : rect.size.width / 3 * 1];		[col_val setWidth : rect.size.width / 3 * 2];				[self addTableColumn : col_name];		[self addTableColumn : col_val];						NSMutableArray *name_row = [[NSMutableArray alloc] init];		NSMutableArray *val_row = [[NSMutableArray alloc] init];						for(size_t i = 0; i < view->count; ++i)		{				const psrConflictItem_t *item = view->conflict[i];				assert(item->count > 0);								NSString *name = [NSString stringWithFormat : @"%@  :  %@",								  [ARUtility convertUTF32ToNSString : item->name],								  [ARUtility convertUTF32ToNSString : item->lookahead]								  ];												for(size_t k = 0; k < item->count; ++k)				{						[name_row addObject : name];						[val_row addObject : [ARUtility convertUTF32ToNSString : item->items[k]]];												name = @" ";				}								[name_row addObject : @" "];				[val_row addObject : @" "];						}						[columnsData addObject : name_row];		[columnsData addObject : val_row];				[name_row release];		[val_row release];		[self reloadData]; }-(void) clearAction : (id)sender{		DLog(@"On ActionView::clearAction");		[self clear];}- (NSMenu *)defaultMenu {		        NSMenu *theMenu = [[[NSMenu alloc] initWithTitle:@"Action View Context Menu"] autorelease];		        [theMenu addItemWithTitle:@"Clear" action:@selector(clearAction:) keyEquivalent:@""];				        return theMenu;}-(NSMenu *)menuForEvent:(NSEvent *)theEvent {        return [self defaultMenu];  		}@end