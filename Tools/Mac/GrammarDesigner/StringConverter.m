/* * Copyright (c) 2011 by Solidus * This file is part of Arsenal library tools source code. * * Permission to use, copy, modify, distribute and sell this software * and its documentation for any purpose is hereby granted without fee, * provided that the above copyright notice appear in all copies and * that both that copyright notice and this permission notice appear * in supporting documentation.It is provided "as is" without express  * or implied warranty. * */#import "StringConverter.h"void disableLineWrap(NSTextView		*txtView){		const float LargeNumberForText = 1.0e7;				NSScrollView *scrollView = [txtView enclosingScrollView];		[scrollView setHasVerticalScroller:YES];		[scrollView setHasHorizontalScroller:YES];		[scrollView setAutoresizingMask:(NSViewWidthSizable | NSViewHeightSizable)];				NSTextContainer *textContainer = [txtView textContainer];		[textContainer setContainerSize:NSMakeSize(LargeNumberForText, LargeNumberForText)];		[textContainer setWidthTracksTextView:NO];		[textContainer setHeightTracksTextView:NO];				[txtView setMaxSize:NSMakeSize(LargeNumberForText, LargeNumberForText)];		[txtView setHorizontallyResizable:YES];		[txtView setVerticallyResizable:YES];		[txtView setAutoresizingMask:NSViewNotSizable];}@implementation StringConverter-(id)init{		self = [super initWithWindowNibName : @"StringConverter"];				if(self)		{				default_font = [NSFont fontWithName : @"Menlo"											   size : 18								];				[default_font retain];		}		return self;}-(void)dealloc{		[super dealloc];}-(void)show : (id)sender{				[self showWindow : sender];}-(void)awakeFromNib{		[stringText setFont : default_font];		[codeText setFont : default_font];				disableLineWrap(stringText);		disableLineWrap(codeText);		}-(IBAction)convertToCode : (id)sender{		NSString *str = [stringText string];				if([str length] == 0)		{				DLog(@"Empty string input failed");				return;		}				WideCharWrapper *s = [ARUtility convertNSStringToUTF32 : str];										int_t len = AR_str_to_escstr_buf(NULL, 0, [s string]);				if(len == 0)		{				DLog(@"AR_str_to_escstr_buf failed");				return;		}				wchar_t *code = (wchar_t*)malloc(sizeof(wchar_t) * (len + 1));				len = AR_str_to_escstr_buf(code, len, [s string]);				NSString *code_tmp = [ARUtility convertUTF32ToNSString : code];						[codeText setString : code_tmp];				free(code);		code = NULL;}-(IBAction)convertToString : (id)sender{		NSString *code = [codeText string];				if([code length] == 0)		{				DLog(@"convertToString empty code");				return;		}				WideCharWrapper *tmp = [ARUtility convertNSStringToUTF32 : code];						arEscStrErr_t	err;		int_t len;				len = AR_escstr_to_str_buf(NULL, 0, [tmp string], &err);				if(len == 0)		{				DLog(@"AR_escstr_to_str_buf convertNSStringToUTF32 return 0 length string");				return;		}				NSString *info = NULL;				if(err.type == AR_ESCSTR_ERR_VALUE)		{				info = [NSString stringWithFormat : @"Invalid value %d : too big for wide character\r\n",						(int)err.value						];		}else if(err.type == AR_ESCSTR_ERR_CHAR)		{				info = [NSString stringWithFormat : @"Invalid char \\x%x : unrecognized character escape sequence\r\n",						(unsigned int)*err.pos						];						}else if(err.type == AR_ESCSTR_ERR_BUFFER)		{				AR_ASSERT(false);				return;		}				if(info != nil)		{				[stringText setString : info];				return;		}				wchar_t *str = (wchar_t*)malloc(sizeof(wchar_t) * (len));		len = AR_escstr_to_str_buf(str, len, [tmp string], &err);				NSString *str_tmp = [ARUtility convertUTF32ToNSString : str];		free(str);		str = NULL;				if(str_tmp == nil)		{				DLog(@"ARUtility convertUTF32ToNSString failed");				return;		}				[stringText setString : str_tmp];						}@end