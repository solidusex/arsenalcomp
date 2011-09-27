/* * Copyright (c) 2011 by Solidus * This file is part of Arsenal library tools source code. * * Permission to use, copy, modify, distribute and sell this software * and its documentation for any purpose is hereby granted without fee, * provided that the above copyright notice appear in all copies and * that both that copyright notice and this permission notice appear * in supporting documentation.It is provided "as is" without express  * or implied warranty. * */#import "stdheader.h"@class GrammarDesignerDocument;@class InputChecker;/**************************************InputTextView*******************/@interface InputTextView : NSTextView{		NSFont			*font;		BOOL			hasValidParser;}-(void)setHasValidParser : (BOOL)valid;@end/**************************************SyntaxTreeView*******************/@interface InputSyntaxTreeView : NSOutlineView{		NSFont			*font;		BOOL			hasValidParser;}-(void)setHasValidParser : (BOOL)valid;@end/**************************************InputCheckerController*******************/@interface InputChecker : NSWindowController{		IBOutlet		InputTextView			*textView;		IBOutlet		InputSyntaxTreeView		*syntaxView;		}@property (readonly) InputTextView			*textView;@property (readonly) InputSyntaxTreeView	*syntaxView;-(void)dealloc;-(void)show : (id)sender;-(void)hide : (id)sender;-(BOOL)isShow;-(void)setHasValidParser : (BOOL)valid;@end