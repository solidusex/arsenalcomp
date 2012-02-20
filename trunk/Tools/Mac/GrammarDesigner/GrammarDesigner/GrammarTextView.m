/* * Copyright (c) 2011 by Solidus * This file is part of Arsenal library tools source code. * * Permission to use, copy, modify, distribute and sell this software * and its documentation for any purpose is hereby granted without fee, * provided that the above copyright notice appear in all copies and * that both that copyright notice and this permission notice appear * in supporting documentation.It is provided "as is" without express  * or implied warranty. * */#import "GrammarTextView.h"@implementation GrammarTextView-(void)setParserIsBuild : (BOOL)isBuild{		parserIsBuild = isBuild;}-(void)selecteLine : (size_t)line{		int location = -1;		int length = -1;		DLog(@"GrammarTextView::selectLine");				NSString *txt = [self string];				/*if(txt == nil || [txt length] == 0)		{				return;		}		 */				WideCharWrapper *wcs_txt = [ARUtility convertNSStringToUTF32 : txt];												Lex_ResetInput(line_match, [wcs_txt string]);				lexToken_t tok;		arStatus_t is_ok = AR_S_NO;								while(YES)		{				is_ok = Lex_Match(line_match, &tok);								if(is_ok != AR_S_YES)				{						break;				}								if(line == tok.line)				{						break;				}								if(tok.value == 0)				{						break;				}		}				if(is_ok == AR_S_YES && tok.value != 0 && tok.count > 0)		{				location = (int)tok.index;				length = (int)tok.count;		}		assert(is_ok == AR_S_YES);								DLog(@"select range == (%d,%d)", location, length);				if(location != -1 && length != -1)		{				[self setSelectedRange : NSMakeRange(location, length)];				[self scrollRangeToVisible : NSMakeRange(location, length)];		}}-(NSString*)getLine : (size_t)line{		wchar_t *line_msg = NULL;		NSString *ret = nil;		DLog(@"GrammarTextView::getLine");				NSString *txt = [self string];				if(txt == nil || [txt length] == 0)		 {				 return nil;		 }		 				WideCharWrapper *wcs_txt = [ARUtility convertNSStringToUTF32 : txt];																		Lex_ResetInput(line_match, [wcs_txt string]);				lexToken_t tok;		arStatus_t is_ok = AR_S_NO;								while(YES)		{				is_ok = Lex_Match(line_match, &tok);								if(is_ok != AR_S_YES)				{						break;				}								if(line == tok.line)				{						break;				}								if(tok.value == 0)				{						break;				}		}				if(is_ok && tok.value != 0 && tok.count > 0)		{				line_msg = healper_wcsndup(tok.str, tok.count);		}				assert(is_ok == AR_S_YES);						if(line_msg)		{				ret = [ARUtility convertUTF32ToNSString : line_msg];								free(line_msg);		}				return ret;}-(NSArray*)getLines{				NSMutableArray *ret = [[NSMutableArray alloc] init];		[ret autorelease];		DLog(@"GrammarTextView::getLines");				NSString *txt = [self string];				if(txt == nil || [txt length] == 0)		{				return ret;		}						WideCharWrapper *wcs_txt = [ARUtility convertNSStringToUTF32 : txt];																Lex_ResetInput(line_match, [wcs_txt string]);				lexToken_t tok;		arStatus_t is_ok = AR_S_NO;				while(YES)		{				is_ok = Lex_Match(line_match, &tok);								if(is_ok != AR_S_YES || tok.value == 0 || tok.count == 0)				{						break;				}				wchar_t *line_msg = healper_wcsndup(tok.str, tok.count);				NSString *tmp = [ARUtility convertUTF32ToNSString : line_msg];				[ret addObject : tmp];				free(line_msg);						}								return ret;		}-(void)disableLineWrap{		const float LargeNumberForText = 1.0e7;				NSScrollView *scrollView = [self enclosingScrollView];		[scrollView setHasVerticalScroller:YES];		[scrollView setHasHorizontalScroller:YES];		[scrollView setAutoresizingMask:(NSViewWidthSizable | NSViewHeightSizable)];				NSTextContainer *textContainer = [self textContainer];		[textContainer setContainerSize:NSMakeSize(LargeNumberForText, LargeNumberForText)];		[textContainer setWidthTracksTextView:NO];		[textContainer setHeightTracksTextView:NO];				[self setMaxSize:NSMakeSize(LargeNumberForText, LargeNumberForText)];		[self setHorizontallyResizable:YES];		[self setVerticallyResizable:YES];		[self setAutoresizingMask:NSViewNotSizable];}-(void)resetFont{		NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];				NSString *name;		NSInteger size;						name = [defaults stringForKey : PREFERENCE_FONT_GRAMMAR_NAME];		size = (NSInteger)[defaults doubleForKey : PREFERENCE_FONT_GRAMMAR_SIZE];				[font release];		font = [NSFont fontWithName : name							   size : size				];				[font retain];				[self setFont : font];}-(void)handleFontChanged : (NSNotification*)note{		DLog(@"GrammarTextView::handleFontChanged for notification %@", note);		[self resetFont];		}-(void)awakeFromNib{				line_lex = Lex_Create();				lexAction_t act;		act.is_skip = false;		act.priority = 0;		act.value = 1;				if(Lex_InsertRule(line_lex, L".*(\\r\\n|\\n\\r|\\r|\\n)?", &act) != AR_S_YES)		{				assert(0);		}				act.priority = 1;		act.value = 0;				if(Lex_InsertRule(line_lex, L"$", &act) != AR_S_YES)		{				assert(0);		}				line_match = Lex_CreateMatch(line_lex);								NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];		[nc addObserver : self			   selector : @selector(handleFontChanged:)				   name : NOTIFICATION_PREFERENCE_FONT_CHANGED				 object : nil		 ];						[self resetFont];				[self disableLineWrap];				[self setDelegate : self];				parserIsBuild = NO;						}-(void)dealloc{		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];		[nc removeObserver : self];				[font release];		font = nil;				Lex_DestroyMatch(line_match);		line_match = NULL;		Lex_Destroy(line_lex);		line_lex = NULL;				[super dealloc];}- (void)textDidChange:(NSNotification *)note{		DLog(@"GrammarTextView::textDidChange %@",note);}- (NSMenu *)defaultMenu {		        NSMenu *theMenu = [[[NSMenu alloc] initWithTitle:@"Grammar Text View Context Menu"] autorelease];				[theMenu addItemWithTitle:@"Input Checker" action:@selector(showInputChecker:) keyEquivalent:@""];		[theMenu addItem : [NSMenuItem separatorItem]	 ];				        [theMenu addItemWithTitle:@"Goto Declaration" action:@selector(gotoDeclaration:) keyEquivalent:@""];        [theMenu addItemWithTitle:@"Find All References" action:@selector(findAllReferences:) keyEquivalent:@""];        [theMenu addItemWithTitle:@"Build" action:@selector(buildParser:) keyEquivalent:@""];				NSMenuItem *showItem = nil;				{				NSMenu *showMenu = [[[NSMenu alloc] initWithTitle:@"Grammar Text View Context Show Menu"] autorelease];				//[showMenu addItemWithTitle:@"Action Table" action:@selector(showActionTable:) keyEquivalent:@"" ];				[showMenu addItemWithTitle:@"Conflicts" action:@selector(showConflicts:) keyEquivalent:@""];				[showMenu addItemWithTitle:@"FirstFollow" action:@selector(showFirstFollow:) keyEquivalent:@""];				[showMenu addItemWithTitle:@"Left Recursive" action:@selector(showLeftRecursive:) keyEquivalent:@""];				[showMenu addItemWithTitle:@"Left Factor" action:@selector(showLeftFactor:) keyEquivalent:@""];								showItem = [[NSMenuItem alloc] initWithTitle : @"Show"													  action : NULL											   keyEquivalent : @""							];				[showItem  setSubmenu : showMenu];					}				[theMenu addItem : showItem	 ];		[showItem release];				[theMenu addItem : [NSMenuItem separatorItem]	 ];		[theMenu addItemWithTitle:@"Cut" action:@selector(cut:) keyEquivalent:@""];		[theMenu addItemWithTitle:@"Copy" action:@selector(copy:) keyEquivalent:@""];		[theMenu addItemWithTitle:@"Paste" action:@selector(paste:) keyEquivalent:@""];				[theMenu addItem : [NSMenuItem separatorItem]	 ];				[theMenu addItemWithTitle:@"Select All" action:@selector(selectAll:) keyEquivalent:@""];		        return theMenu;}-(NSMenu *)menuForEvent:(NSEvent *)theEvent {        return [self defaultMenu];  		}-(void) findAllReferences :(id)sender{		DLog(@"On findAllReferences");				NSRange sel_range = [self selectedRange];				DLog(@"location = %d length = %d", sel_range.location, sel_range.length);				NSString *s = nil;		if(sel_range.length != 0)		{				s = [[self string] substringWithRange : sel_range];		}				if(s != nil)		{				NSString *trim_s = [s stringByTrimmingCharactersInSet : [NSCharacterSet whitespaceAndNewlineCharacterSet]];								if(trim_s != nil && [trim_s length] > 0)				{						DLog(@"GrammarTextView Sending NOTIFICATION_GRAMMARVIEW_INVOKE_FIND_ALL_REFERENCES");												NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];						[nc postNotificationName :	NOTIFICATION_GRAMMARVIEW_INVOKE_FIND_ALL_REFERENCES 										  object :  self										userInfo : [NSDictionary dictionaryWithObject : trim_s 																			   forKey : @"UserInfo"													]						 ];				}		}		}-(void) gotoDeclaration :(id)sender{		DLog(@"On gotoDeclaration");				NSRange sel_range = [self selectedRange];				DLog(@"location = %d length = %d", sel_range.location, sel_range.length);				NSString *s = nil;		if(sel_range.length != 0)		{				s = [[self string] substringWithRange : sel_range];		}				if(s != nil)		{				NSString *trim_s = [s stringByTrimmingCharactersInSet : [NSCharacterSet whitespaceAndNewlineCharacterSet]];								if(trim_s != nil && [trim_s length] > 0)				{						DLog(@"GrammarTextView Sending NOTIFICATION_GRAMMARVIEW_INVOKE_GOTO_DECLARATION");										NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];						[nc postNotificationName :	NOTIFICATION_GRAMMARVIEW_INVOKE_GOTO_DECLARATION 										  object :  self										userInfo : [NSDictionary dictionaryWithObject : trim_s 																	   forKey : @"UserInfo"											]						 ];				}		}}-(void) showInputChecker :(id)sender{		DLog(@"On showInputChecker");				DLog(@"GrammarTextView Sending NOTIFICATION_GRAMMARVIEW_INVOKE_SHOW_INPUT_CHECKER");				NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];		[nc postNotificationName :	NOTIFICATION_GRAMMARVIEW_INVOKE_SHOW_INPUT_CHECKER 						  object :  self						userInfo : nil		 ];				}-(void) buildParser :(id)sender{				DLog(@"On buildParser");						NSString *s = [self string];				if(s != nil)		{				DLog(@"GrammarTextView Sending NOTIFICATION_GRAMMARVIEW_INVOKE_BUILD_PARSER");				NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];				[nc postNotificationName :	NOTIFICATION_GRAMMARVIEW_INVOKE_BUILD_PARSER 								  object :  self								userInfo : [NSDictionary dictionaryWithObject : s 																	   forKey : @"UserInfo"											]				 ];		}				}-(void) showActionTable :(id)sender{		DLog(@"GrammarTextView Sending NOTIFICATION_GRAMMARVIEW_INVOKE_SHOW_ACTION_TABLE");		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];		[nc postNotificationName: NOTIFICATION_GRAMMARVIEW_INVOKE_SHOW_ACTION_TABLE 						 object : self		 ];}-(void) showConflicts :(id)sender{		DLog(@"GrammarTextView Sending NOTIFICATION_GRAMMARVIEW_INVOKE_SHOW_CONFLICTS");		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];		[nc postNotificationName: NOTIFICATION_GRAMMARVIEW_INVOKE_SHOW_CONFLICTS 						 object : self		 ];}-(void) showFirstFollow :(id)sender{		DLog(@"GrammarTextView Sending NOTIFICATION_GRAMMARVIEW_INVOKE_SHOW_FIRSTFOLLOW_SET");		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];		[nc postNotificationName: NOTIFICATION_GRAMMARVIEW_INVOKE_SHOW_FIRSTFOLLOW_SET 						 object : self		 ];}-(void) showLeftRecursive :(id)sender{		DLog(@"GrammarTextView Sending NOTIFICATION_GRAMMARVIEW_INVOKE_SHOW_LEFT_RECURSIVE");		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];		[nc postNotificationName: NOTIFICATION_GRAMMARVIEW_INVOKE_SHOW_LEFT_RECURSIVE 						 object : self		 ];		}-(void) showLeftFactor :(id)sender{		DLog(@"GrammarTextView Sending NOTIFICATION_GRAMMARVIEW_INVOKE_SHOW_LEFT_FACTOR");		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];		[nc postNotificationName: NOTIFICATION_GRAMMARVIEW_INVOKE_SHOW_LEFT_FACTOR 						 object : self		 ];		}-(BOOL)validateUserInterfaceItem:(id < NSValidatedUserInterfaceItem >)item{				BOOL ret = YES;		assert(item != nil);				NSMenuItem *menuItem = (NSMenuItem*)item;				NSString *s = [self string];										if([menuItem action] == @selector(findAllReferences:) || [menuItem action] == @selector(gotoDeclaration:)) 		{				NSRange sel_range = [self selectedRange];								if(sel_range.length > 0)				{						NSString *s = [[self string] substringWithRange : sel_range];						NSString *trim_s = [s stringByTrimmingCharactersInSet : [NSCharacterSet whitespaceAndNewlineCharacterSet]];												if(trim_s == nil || [trim_s length] == 0)						{								ret = NO;						}																}else				{						ret = NO;				}						}else if([menuItem action] == @selector(buildParser:)) 		{				if([s length] == 0)				{						ret = NO;				}						}else if([menuItem action] == @selector(showActionTable:))		{				ret = parserIsBuild;						}else if([menuItem action] == @selector(showConflicts:))		{				ret = parserIsBuild;						}else if([menuItem action] == @selector(showFirstFollow:))		{				ret = parserIsBuild;						}else if([menuItem action] == @selector(showLeftRecursive:))		{				ret = parserIsBuild;						}else if([menuItem action] == @selector(showLeftFactor:))		{				ret = parserIsBuild;		}else		{				ret = [super validateUserInterfaceItem : item];		}						return ret;				}@end