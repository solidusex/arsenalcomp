////  ARPrintParser.m//  GrammarDesigner////  Created by liu peng on 9/29/11.//  Copyright 2011 none. All rights reserved.//#import "ARPrintParser.h"#import "ARUtility.h"@implementation PrintNode : NSObject@synthesize		name;@synthesize 	index;@synthesize 	length;@synthesize		line;@synthesize		col;@synthesize 	isTerm;-(id)init{		self = [super init];		if(self)		{						}				return self;}-(void)dealloc{		[name release];		name = nil;				[nodes release];		nodes = nil;		[super dealloc];}-(size_t)		getChildCount{		size_t n = 0;		if(nodes != nil)		{				n = [nodes count];		}		return n;}-(PrintNode*)	getChild : (size_t)idx{		const size_t cnt = [nodes count];		if(idx < cnt)		{				return [nodes objectAtIndex : idx];		}else		{				return nil;		}}-(void)			appendChild : (PrintNode*)node{		assert(node != nil && node != self);		if(nodes == nil)		{				nodes = [[NSMutableArray alloc] init];		}		[nodes addObject : node];}@endstatic psrNode_t*		AR_STDCALL build_leaf(const psrToken_t *tok, void *ctx){		AR_ASSERT(tok != NULL && ctx != NULL);		NSString *str = nil;				if(tok->term_val == 0)		{				str = @"EOI";		}else		{				//ASSERT(tok->str_cnt > 0);				str = [ARUtility convertUTF32ToNSString : tok->str												 Length : tok->str_cnt					   ];		}					PrintNode *node = [[PrintNode alloc] init];		node.name = str;		node.index = tok->index;		node.length = tok->str_cnt;		node.line = tok->line;		node.col = tok->col;		node.isTerm = YES;		return (psrNode_t*)node;}static psrNode_t*	 AR_STDCALL build_rule(psrNode_t **nodes, size_t count, const wchar_t *name, void *ctx){			AR_ASSERT(name != NULL && ctx != NULL);				PrintNode *node = [[PrintNode alloc] init];		node.name = [ARUtility convertUTF32ToNSString : name];		node.isTerm = NO;				if(count > 0)		{				PrintNode **ns = (PrintNode**)nodes;								for(size_t i = 0; i < count; ++i)				{						PrintNode *tmp = ns[i];						if(tmp == nil)						{								tmp = [[PrintNode alloc] init];								tmp.name = @"%EMPTY";								tmp.isTerm = YES;						}												[node appendChild : tmp];						[tmp release];						tmp = nil;						ns[i] = NULL;				}		}				return (psrNode_t*)node;}static void	AR_STDCALL	__free_node(psrNode_t *node, void *ctx){		PrintNode		*print_node = (PrintNode*)node;		AR_ASSERT(print_node != nil);		[print_node release];		print_node = nil;		}static bool_t	AR_STDCALL	__on_parse_error(const psrToken_t *tok, const size_t expected[], size_t count, void *ctx){		NSObject<PrintNodeParserDelegate>		*context;		const parser_t *parser = NULL;		AR_ASSERT(ctx != NULL);		context = (NSObject<PrintNodeParserDelegate>*)ctx;		parser = [context parser];		AR_ASSERT(parser != NULL);				NSMutableString *msg = [[NSMutableString alloc] init];		NSMutableString *token = [[NSMutableString alloc] init];				if(tok->term_val != 0)		{				[token setString : [ARUtility convertUTF32ToNSString : tok->str Length : tok->str_cnt]];		}else		{				[token setString : @"%EOI"];		}						if(count > 0)		{				[msg appendFormat : @"Invalid Token \"%@\", expected\t:\t", token];								for(size_t i = 0; i < count; ++i)				{						const psrGrammar_t *grammar = Parser_GetGrammar(parser);						AR_ASSERT(grammar != NULL);						const psrTermInfo_t	*term_info = Parser_GetTermSymbInfoByValue(grammar, expected[i]);						AR_ASSERT(term_info != NULL && term_info->term->name != NULL);						[msg appendFormat : @"\"%@\" \t", [ARUtility convertUTF32ToNSString : term_info->term->name]];				}		}else		{				[msg appendFormat : @"Invalid Token \"%@\"", token];		}		[msg appendString : @"."];		[context onParseMsg : PRINT_NODE_ERROR_T 						Msg : msg					   Line : tok->line		 ];		return true;		}static const psrHandler_t		__def_handler = {		__on_parse_error,		__free_node};static void AR_STDCALL report_build_func(const cfgReportInfo_t *report, void *ctx){		NSObject<PrintNodeParserDelegate>		*context;		AR_ASSERT(ctx != NULL);		context = (NSObject<PrintNodeParserDelegate>*)ctx;						switch(report->type)		{				case CFG_REPORT_MESSAGE_T:				{						[context onBuildParserMsg : PRINT_NODE_MSG_T											  Msg : [ARUtility convertUTF32ToNSString : report->std_msg.message]											 Line : -1						 ];				}						break;				case CFG_REPORT_ERROR_T:				{						[context onBuildParserMsg : PRINT_NODE_MSG_T											  Msg : [ARUtility convertUTF32ToNSString : report->error.err_msg]											 Line : -1						 ];				}						break;				case CFG_REPORT_ERROR_LEX_T:				{						NSString *s = [NSString stringWithFormat : @"Lex error : %@", 																[ARUtility convertUTF32ToNSString : report->lex_error.msg]									  									   ];												[context onBuildParserMsg : PRINT_NODE_ERROR_T											  Msg : s											 Line : report->lex_error.tok->line						 ];				}						break;				case CFG_REPORT_ERROR_SYNTAX_T:				{						NSString *s = [NSString stringWithFormat : @"Syntax error : %@", 									   [ARUtility convertUTF32ToNSString : report->syntax_error.msg]									   ];																		[context onBuildParserMsg : PRINT_NODE_ERROR_T											  Msg : s											 Line : report->syntax_error.tok->line						 ];				}						break;				case	CFG_REPORT_WARNING_SYNTAX_T:				{						NSString *s = [NSString stringWithFormat : @"Syntax warning : %@", 									   [ARUtility convertUTF32ToNSString : report->warning.msg]									   ];												[context onBuildParserMsg : PRINT_NODE_ERROR_T											  Msg : s											 Line : report->warning.line						 ];				}						break;				default:						AR_ASSERT(false);		}		}static void	AR_STDCALL	__report_io_error_func(int_t level, const wchar_t *msg, void *ctx){		NSObject<PrintNodeParserDelegate>		*context;		AR_ASSERT(ctx != NULL);		context = (NSObject<PrintNodeParserDelegate>*)ctx;		[context onBuildParserMsg : PRINT_NODE_ERROR_T							  Msg : [ARUtility convertUTF32ToNSString : msg]							 Line : -1		 ];		}static void	AR_STDCALL	__report_io_print_func(const wchar_t *msg, void *ctx){		NSObject<PrintNodeParserDelegate>		*context;		AR_ASSERT(ctx != NULL);		context = (NSObject<PrintNodeParserDelegate>*)ctx;				[context onBuildParserMsg : PRINT_NODE_MSG_T							  Msg : [ARUtility convertUTF32ToNSString : msg]							 Line : -1		 ];		} /****************************************************************************************************************************************************/@implementation PrintNodeParser		:		NSObject@synthesize parserMode;@synthesize lexIsSingleLine;@synthesize lexIsIgnoreCase;@synthesize delegate;-(id)init{		self = [super init];		if(self)		{				parserMode = PARSER_LALR;				lexIsSingleLine = NO;				lexIsIgnoreCase = NO;								lex = NULL;				grammar = NULL;				parser = NULL;						}				return self;}-(void)dealloc{		[self clear];		[super dealloc];}-(BOOL)generateFromGrammar : (NSString *)grammarInput;{		[self clear];				if([grammarInput length] == 0)		{				return NO;		}				cfgReport_t	report = {report_build_func, (void*)delegate};				WideCharWrapper *wrapper = [ARUtility convertNSStringToUTF32 : grammarInput];				const cfgConfig_t *cfg = CFG_CollectGrammarConfig([wrapper string], &report);				if(cfg == NULL || cfg->has_error)		{				goto FAILED_POINT;				}				if(![self generate : cfg])		{				goto FAILED_POINT;			}				if(cfg)		{				CFG_DestroyGrammarConfig(cfg);				cfg = NULL;		}						[delegate onBuildParserMsg : PRINT_NODE_MSG_T							   Msg : @"Build Parser successful!"							  Line : -1		 ];						return YES;		FAILED_POINT:		[self clear];				if(cfg)		{				CFG_DestroyGrammarConfig(cfg);				cfg = NULL;		}				[delegate onBuildParserMsg : PRINT_NODE_MSG_T							   Msg : @"Build Parser failed!"							  Line : -1		 ];		return NO;		}-(void)clear{		if(parser != NULL)		{				Parser_DestroyParser(parser);				parser = NULL;		}				if(grammar != NULL)		{				Parser_DestroyGrammar(grammar);				grammar = NULL;		}				if(lex != NULL)		{				Lex_Destroy(lex);				lex = NULL;		}}-(BOOL)generate : (const cfgConfig_t *)cfg{		BOOL has_error = NO;		lex_t	*lexer = Lex_Create();		psrGrammar_t	*gmr = Parser_CreateGrammar(&__def_handler);		for(size_t i = 0; i < cfg->name_cnt; ++i)		{				const cfgName_t		*name = &cfg->name[i];								if(!Lex_InsertName(lexer, name->name, name->regex))				{#if(1)						NSString *msg = [NSString stringWithFormat : @"Name Error : \"%@ : %@\"",										 [ARUtility convertUTF32ToNSString : name->name],										 [ARUtility convertUTF32ToNSString : name->regex]										 ];												[ self.delegate onBuildParserMsg : PRINT_NODE_ERROR_T													 Msg : msg													Line : name->line						 ];#endif						has_error = YES;				}		}						for(size_t i = 0; i < cfg->tok_cnt; ++i)		{				const cfgToken_t		*tok = &cfg->tok[i];								lexAction_t	action;				action.is_skip = tok->is_skip;				action.priority = tok->lex_prec;				action.value = tok->tokval;				if(!Lex_InsertRule(lexer, tok->regex, &action))				{#if(1)						NSString *msg = [NSString stringWithFormat : @"Token Error : \"%@ : %@\"",										 [ARUtility convertUTF32ToNSString : tok->name],										 [ARUtility convertUTF32ToNSString : tok->regex]										 ];												[ delegate onBuildParserMsg : PRINT_NODE_ERROR_T												Msg : msg											   Line : tok->line						 ];#endif						has_error = YES;				}								if(tok->is_skip || tok->tokval == 0)				{						continue;				}								if(!Parser_InsertTerm(gmr, tok->name, tok->tokval, PARSER_ASSOC_NONASSOC, 0, build_leaf))				{#if(1)						NSString *msg = [NSString stringWithFormat : @"Term Error : \"%@ : %@\"",										 [ARUtility convertUTF32ToNSString : tok->name],										 [ARUtility convertUTF32ToNSString : tok->regex]										 ];												[ delegate onBuildParserMsg : PRINT_NODE_ERROR_T												Msg : msg											   Line : tok->line						 ];#endif						has_error = YES;				}		}				for(size_t i = 0; i < cfg->prec_cnt; ++i)		{				const cfgPrec_t		*prec = &cfg->prec[i];								for(size_t k = 0; k < prec->count; ++k)				{						psrTermInfo_t *info = Parser_GetTermSymbInfoByName(gmr,prec->prec_tok_set[k]);												if(info == NULL)						{								if(!Parser_InsertTerm(gmr, prec->prec_tok_set[k], prec->prec_tok_val[k], prec->assoc, prec->prec_level, NULL))								{#if(1)										NSString *msg = [NSString stringWithFormat : @"Prec Error : \"%@\"",														 [ARUtility convertUTF32ToNSString : prec->prec_tok_set[k]]														 ];																				[ delegate onBuildParserMsg : PRINT_NODE_ERROR_T																Msg : msg															   Line : prec->line										 ];#endif																				has_error = YES;								}						}else						{								info->assoc = prec->assoc;								info->prec = prec->prec_level;						}				}						}								for(size_t i = 0; i < cfg->rule_cnt; ++i)		{				const cfgRule_t		*rule = &cfg->rule[i];#if(1)				NSString *str = [NSString stringWithFormat : @"%@ : %@",								 [ARUtility convertUTF32ToNSString : rule->lhs],     							  [ARUtility convertUTF32ToNSString : rule->rhs]								 ];				WideCharWrapper *wcs = [ARUtility convertNSStringToUTF32 : str];#endif								{						const wchar_t *wcs_tmp = [wcs string];						size_t l = wcslen(wcs_tmp);						size_t xx = 0;														for(xx = 0; xx < l; ++xx)						{								if(wcs_tmp[xx] > 30000)								{										int x = 0; 										x++;								}						}				}								if(!Parser_InsertRuleByStr(gmr, [wcs string], rule->prec_tok,  build_rule, 0))				{#if(1)						NSString *msg = [NSString stringWithFormat : @"Rule Error : \"%@\"", str ];						[ delegate onBuildParserMsg : PRINT_NODE_ERROR_T												Msg : msg											   Line : rule->line						 ];#endif												has_error = YES;				}		}									if(cfg->start.start_rule != NULL)		{				BOOL has_start_rule = NO;								for(size_t i = 0; i < cfg->rule_cnt; ++i)				{						if(AR_wcscmp(cfg->start.start_rule, cfg->rule[i].lhs) == 0)						{								has_start_rule = YES;								break;						}				}												if(!has_start_rule || !Parser_SetStartRule(gmr, cfg->start.start_rule))				{#if(1)						NSString *msg = [NSString stringWithFormat : @"Start Rule Error : \"%@\"", 										 [ARUtility convertUTF32ToNSString : cfg->start.start_rule]										 										 ];						[ delegate onBuildParserMsg : PRINT_NODE_ERROR_T												Msg : msg											   Line : cfg->start.line						 ];#endif						has_error = YES;				}		}						/*********************************************检查并报告语法错误*******************************************************/		{								arIOCtx_t	io_context = 				{						__report_io_error_func,						__report_io_print_func,						(void*)delegate										};								if(!Parser_CheckIsValidGrammar(gmr, &io_context))				{						has_error = YES;				}		}		/******************************************************************************************************/							if(has_error)		{				Lex_Destroy(lexer);				lexer = NULL;				Parser_DestroyGrammar(gmr);				gmr = NULL;				return NO;		}else		{				uint_64_t beg, end;								beg = AR_GetTime_Milliseconds();								lex = lexer;				Lex_GenerateTransTable(lex);				grammar	= gmr;								parser = Parser_CreateParser(grammar, parserMode);								end = AR_GetTime_Milliseconds();				#if(1)				NSString *str = [NSString stringWithFormat : @"Build Parser Tick count %qu",								 end - beg								 ];								[ delegate onBuildParserMsg : PRINT_NODE_MSG_T										Msg : str									   Line : -1				 ];#endif				size_t conflict = Parser_CountParserConflict(parser);								if(conflict > 0)				{#if(1)						str = [NSString stringWithFormat : @"The grammar has %u conflicts",							   (uint_32_t)conflict							   ];												[ delegate onBuildParserMsg : PRINT_NODE_MSG_T												Msg : str											   Line : -1						 ];#endif				}							return YES;		}}-(BOOL)isValidParser{		return parser == NULL ? NO : YES;}-(PrintNode*)	parse : (NSString*)src{		DLog(@"On PrintNodeParser::parse");				assert(src != nil);				if(![self isValidParser])		{				return nil;		}				[delegate setParser : parser];								PrintNode *result = nil;				WideCharWrapper *wcs = [ARUtility convertNSStringToUTF32 : src];								bool is_ok = true;						lexMatch_t *match;		psrContext_t *parser_context = Parser_CreateContext(parser, (void*)delegate);		match = Lex_CreateMatch(lex);		Lex_ResetInput(match, [wcs string]);		Lex_MatchClearFlags(match);		uint_t flags = 0;		if(lexIsSingleLine)		{				flags |= LEX_SINGLE_LINE;		}				if(lexIsIgnoreCase)		{				flags |= LEX_IGNORE_CASE;		}				Lex_MatchFlags(match, flags, true);				lexToken_t		token;		memset(&token, 0, sizeof(token));				uint_64_t beg = 0, end = 0;				beg = AR_GetTime_Milliseconds();										while(is_ok)		{				if(!Lex_Match(match, &token))				{						size_t len = wcslen(Lex_GetNextInput(match));						if(len > 10) len = 10;												NSMutableString *lex_err = [[NSMutableString alloc] init], *msg = [[NSMutableString alloc] init];						[						 msg appendString : [ARUtility convertUTF32ToNSString : Lex_GetNextInput(match)																	   Length : len											 ]						 ];												[lex_err appendFormat : @"Lexer Error : %@", msg];												size_t line;						Lex_MatchGetCoordinate(match, NULL, &line, NULL);						 [delegate onParseMsg : PRINT_NODE_ERROR_T										  Msg : lex_err										 Line : (int)line						  ];												Lex_Skip(match);						Lex_ClearError(match);				}else				{						psrToken_t		psr_tok;												PARSER_TOTERMTOK(&token, &psr_tok);						is_ok = Parser_AddToken(parser_context, &psr_tok);						if(token.value == 0)						{								break;						}				}		}				end = AR_GetTime_Milliseconds();				{				NSString *str = [NSString stringWithFormat : @"Parse code Tick count %u", (uint_32_t)(end - beg)];				[delegate onParseMsg : PRINT_NODE_MSG_T								 Msg : str								Line : -1				 ];		}								if(is_ok)		{				result = (PrintNode*)Parser_GetResult(parser_context);				assert(result != nil);				[delegate onParseMsg : PRINT_NODE_MSG_T								 Msg : @"Parse input successful !"								Line : -1				 ];		}else		{				[delegate onParseMsg : PRINT_NODE_MSG_T								 Msg : @"Parse input failed !"								Line : -1				 ];		}									Lex_DestroyMatch(match);		Parser_DestroyContext(parser_context);						return result;}@end