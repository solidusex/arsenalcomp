/* * Copyright (c) 2011 by Solidus * This file is part of Arsenal library tools source code. * * Permission to use, copy, modify, distribute and sell this software * and its documentation for any purpose is hereby granted without fee, * provided that the above copyright notice appear in all copies and * that both that copyright notice and this permission notice appear * in supporting documentation.It is provided "as is" without express  * or implied warranty. * */#import "stdheader.h"@interface ActionView : NSTableView<NSTableViewDataSource> {		NSFont	*font;		NSMutableArray	*columnsData;}-(void)clear;-(void)	drawFirstFollowView : (const psrStatusView_t*) view;-(void)	drawLeftRecursionView : (const psrStatusView_t*) view;-(void)	drawLeftFactorView : (const psrStatusView_t*) view;-(void)	drawConflictView : (const psrConflictView_t*) view;@end