/* * Copyright (c) 2011 by Solidus * This file is part of Arsenal library tools source code. * * Permission to use, copy, modify, distribute and sell this software * and its documentation for any purpose is hereby granted without fee, * provided that the above copyright notice appear in all copies and * that both that copyright notice and this permission notice appear * in supporting documentation.It is provided "as is" without express  * or implied warranty. * */#import "GrammarDesignerDocument.h"@implementation GrammarDesignerDocument- (id)init{		DLog(@"On GrammarDesignerDocument::init");		self = [super init];		if (self) {    				// Add your subclass-specific initialization here.				// If an error occurs here, send a [self release] message and return nil.    		}		return self;}- (id)initWithType:(NSString *)typeName error:(NSError **)outError{		DLog(@"On GrammarDesignerDocument::initWithType : %@", typeName);				self = [super init];				if(self)		{						}				return self;		}- (id)initWithContentsOfURL:(NSURL *)absoluteURL ofType:(NSString *)typeName error:(NSError **)outError{		DLog(@"On GrammarDesignerDocument::initWithContentsOfURL");		self = [super init];				if(self)		{						}						return self;}- (id)initForURL:(NSURL *)absoluteDocumentURL withContentsOfURL:(NSURL *)absoluteDocumentContentsURL ofType:(NSString *)typeName error:(NSError **)outError{		DLog(@"On GrammarDesignerDocument::initForURL");				self = [super init];				if(self)		{		}		return self;}- (NSString *)windowNibName{    // Override returning the nib file name of the document    // If you need to use a subclass of NSWindowController or if your document supports multiple NSWindowControllers, you should remove this method and override -makeWindowControllers instead.    return @"GrammarDesignerDocument";}- (void)windowControllerDidLoadNib:(NSWindowController *) aController{    [super windowControllerDidLoadNib:aController];    // Add any code here that needs to be executed once the windowController has loaded the document's window.}- (NSData *)dataOfType:(NSString *)typeName error:(NSError **)outError{    // Insert code here to write your document to data of the specified type. If the given outError != NULL, ensure that you set *outError when returning nil.    // You can also choose to override -fileWrapperOfType:error:, -writeToURL:ofType:error:, or -writeToURL:ofType:forSaveOperation:originalContentsURL:error: instead.    // For applications targeted for Panther or earlier systems, you should use the deprecated API -dataRepresentationOfType:. In this case you can also choose to override -fileWrapperRepresentationOfType: or -writeToFile:ofType: instead.    if ( outError != NULL ) {		*outError = [NSError errorWithDomain:NSOSStatusErrorDomain code:unimpErr userInfo:NULL];	}	return nil;}- (BOOL)readFromData:(NSData *)data ofType:(NSString *)typeName error:(NSError **)outError{    // Insert code here to read your document from the given data of the specified type.  If the given outError != NULL, ensure that you set *outError when returning NO.    // You can also choose to override -readFromFileWrapper:ofType:error: or -readFromURL:ofType:error: instead.         // For applications targeted for Panther or earlier systems, you should use the deprecated API -loadDataRepresentation:ofType. In this case you can also choose to override -readFromFile:ofType: or -loadFileWrapperRepresentation:ofType: instead.        if ( outError != NULL ) {		*outError = [NSError errorWithDomain:NSOSStatusErrorDomain code:unimpErr userInfo:NULL];	}    return YES;}-(IBAction)setDocumentLexerSingleLine : (id)sender{		DLog(@"On GrammarDesignerDocument::setDocumentLexerSingleLine");}-(IBAction)setDocumentLexerIgnoreCase : (id)sender{		DLog(@"On GrammarDesignerDocument::setDocumentLexerIgnoreCase");}-(IBAction)setDocumentParserModeSLR : (id)sender{		DLog(@"On GrammarDesignerDocument::setDocumentParserModeSLR");}-(IBAction)setDocumentParserModeLALR : (id)sender{		DLog(@"On GrammarDesignerDocument::setDocumentParserModeLALR");}-(IBAction)buildDocumentParser	:	(id)sender{		DLog(@"On GrammarDesignerDocument::buildDocumentParser");}-(IBAction)parseDocumentInput	:	(id)sender{		DLog(@"On GrammarDesignerDocument::parseDocumentInput");}-(IBAction)rebuildDocumentTags	:	(id)sender{		DLog(@"On GrammarDesignerDocument::rebuildDocumentTags");}-(IBAction)generateDocumentGrammarTemplateCode : (id)sender{		DLog(@"On GrammarDesignerDocument::generateDocumentGrammarTemplateCode");}@end