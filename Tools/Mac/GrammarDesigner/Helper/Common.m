#import "Common.h"/********************************/#import "ARUtility.h"void msgbox_nsstr(NSString *msg){		NSRunAlertPanel(@"Warning", msg, @"OK", nil, nil);}static void utility_test(){		wchar_t *ws = L"中国字真难!";				NSString *s = [ARUtility convertUTF32ToNSString : ws];				msgbox_nsstr(s);}void AR_Test(){		utility_test();		}