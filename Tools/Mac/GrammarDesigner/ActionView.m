////  ActionView.m//  GrammarDesigner////  Created by liu peng on 9/26/11.//  Copyright 2011 none. All rights reserved.//#import "ActionView.h"@implementation ActionView-(void)clear{		while(YES)		{				NSArray *columns = [self tableColumns];								if([columns count] == 0)				{						break;				}						[				 self removeTableColumn  : 	 [columns objectAtIndex : 0]				 ];		}}-(void)awakeFromNib{		[self clear];		}@end