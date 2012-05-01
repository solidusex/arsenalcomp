/*
 * The Arsenal Library
 * Copyright (c) 2009-2012 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.It is provided "as is" without express 
 * or implied warranty.
 *
 */

#include <pwd.h>
#include "common.h"


AR_NAMESPACE_BEGIN

		
#if defined(OS_FAMILY_WINDOWS)

arStatus_t		AR_GetCurrentPath(arString_t *str)
{
		arStatus_t status;
		wchar_t *tmp;
		DWORD l,n;
		AR_ASSERT(str != NULL);

		status = AR_S_YES;
		tmp = NULL;
		l = GetCurrentDirectoryW(0, NULL);
		
		if (l <= 0)
		{
				status = AR_E_FAIL;
				goto END_POINT;
		}

		tmp = AR_NEWARR(wchar_t, l + 3);
		
		if(tmp == NULL)
		{
				status = AR_E_NOMEM;
				goto END_POINT;
		}

		n = GetCurrentDirectoryW(l + 3, tmp);

		if(n <= 0 || n > l)
		{
				AR_DEL(tmp);
				tmp = NULL;
				status = AR_E_FAIL;
				goto END_POINT;
		}
		
		if(tmp[n - 1] != L'\\')
		{
				tmp[n] = L'\\';
				tmp[n+1] = L'\0';
		}

		status = AR_SetString(str, tmp);


END_POINT:
		if(tmp)
		{
				AR_DEL(tmp);
				tmp = NULL;
		}

		return status;
	
}

arStatus_t		AR_GetHomePath(arString_t *str)
{
		arStatus_t status;
		arString_t *tmp;
		AR_ASSERT(str != NULL);

		tmp = NULL;
		status = AR_S_YES;
		AR_ClearString(str);

		tmp = AR_CreateString();

		status = AR_getenv(L"HOMEDRIVE", str);
		
		if(status != AR_S_YES)
		{
				goto END_POINT;
		}
		
		status = AR_getenv(L"HOMEPATH", tmp);

		if(status != AR_S_YES)
		{
				goto END_POINT;
		}

		if(AR_GetStringChar(tmp, AR_GetStringLength(str) - 1) != L'\\')
		{
				status = AR_AppendCharToString(tmp, L'\\');
				if(status != AR_S_YES)
				{
						goto END_POINT;
				}
		}

		status = AR_AppendString(str, AR_GetStringCString(tmp));

		if(status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:
		if(tmp)
		{
				AR_DestroyString(tmp);
				tmp = NULL;
		}

		return status;
}

arStatus_t		AR_GetTempPath(arString_t *str)
{
		wchar_t tmp[MAX_PATH + 10];
		arStatus_t status;
		DWORD n;
		AR_ASSERT(str != NULL);

		status = AR_S_YES;
		n = GetTempPathW(MAX_PATH, tmp);
		if(n <= 0)
		{
				return AR_E_FAIL;
		}

		n = GetLongPathNameW(tmp, tmp, n);

		if(n <= 0)
		{
				return AR_E_FAIL;
		}

		if(tmp[n - 1] != L'\\')
		{
				tmp[n] = L'\\';
				tmp[n+1] = L'\0';
		}

		return AR_SetString(str, tmp);

}

arStatus_t		AR_GetNullPath(arString_t *str)
{
		AR_ASSERT(str != NULL);
		return AR_SetString(str, L"NUL:");
}

arStatus_t		AR_GetExpandPath(const wchar_t *path, arString_t *expanded_path)
{
		wchar_t tmp[MAX_PATH + 10];
		arStatus_t status;
		DWORD n;
		AR_ASSERT(path != NULL && expanded_path != NULL);

		status = AR_S_YES;
		n = ExpandEnvironmentStringsW(path, tmp, MAX_PATH);
		if(n <= 0 || n > MAX_PATH - 1)
		{
				return AR_SetString(expanded_path, path);
		}else
		{
				return AR_SetString(expanded_path, tmp);
		}
}



#elif defined(OS_FAMILY_UNIX)

#if !defined(PATH_MAX)
        #define PATH_MAX 1024
#endif
		
arStatus_t		AR_GetCurrentPath(arString_t *str)
{
        arStatus_t status;
        wchar_t *w_tmp;
        char tmp[PATH_MAX];
		AR_ASSERT(str != NULL);
        
        w_tmp = NULL;
        status = AR_S_YES;
        
        if(getcwd(tmp, PATH_MAX) == NULL)
        {
                status = AR_E_FAIL;
                goto END_POINT;
        }
        
        w_tmp = AR_str_to_wcs(AR_CP_ACP, tmp, AR_strlen(tmp));
        
        if(w_tmp == NULL)
        {
                status = AR_E_BADENCCONV;
                goto END_POINT;
        }
        
        status = AR_SetString(str, w_tmp);
        
        if(status != AR_S_YES)
        {
                goto END_POINT;
        }
        
        if(AR_GetStringChar(str, AR_GetStringLength(str) - 1) != L'/')
        {
                status = AR_AppendCharToString(str, L'/');
        }
       
        
        
END_POINT:
        if(w_tmp)
        {
                AR_DEL(w_tmp);
                w_tmp = NULL;
        }
        return status;
}


arStatus_t		AR_GetHomePath(arString_t *str)
{
        char *path;
        wchar_t *w_tmp;
        arStatus_t status;
        struct passwd *pwd;
        AR_ASSERT(str != NULL);
        
        status = AR_S_YES;
        path = NULL;
        w_tmp = NULL;
        pwd = getpwuid(getuid());
        
        if(pwd != NULL)
        {
                path = pwd->pw_dir;  
        }else
        {
                pwd = getpwuid(geteuid());
                if(pwd != NULL)
                {
                        path = pwd->pw_dir;
                }else
                {
                        path = NULL;
                }
        }
        
        if(path == NULL)
        {
                status = AR_getenv(L"HOME", str);
                
                if(status != AR_S_YES)
                {
                        goto END_POINT;
                }
        }else
        {
                w_tmp = AR_str_to_wcs(AR_CP_ACP, path, AR_strlen(path));
                
                if(w_tmp == NULL)
                {
                        status = AR_E_BADENCCONV;
                        goto END_POINT;
                }
                
                status = AR_SetString(str, w_tmp);
                
                if(status != AR_S_YES)
                {
                        goto END_POINT;
                }
        }
        
        if(AR_GetStringChar(str, AR_GetStringLength(str) - 1) != L'/')
        {
                status = AR_AppendCharToString(str, L'/');
                if(status != AR_S_YES)
                {
                        goto END_POINT;
                }
        }
       
END_POINT:
        if(w_tmp != NULL)
        {
                AR_DEL(w_tmp);
                w_tmp = NULL;
        }
                
        return status;
}

arStatus_t		AR_GetTempPath(arString_t *str)
{
        arStatus_t status;
        AR_ASSERT(str != NULL);

        status = AR_S_YES;
        
        status = AR_getenv(L"TMPDIR", str);
        
        if(status != AR_S_YES)
        {
                status = AR_SetString(str, L"/tmp/");
                
                if(status != AR_S_YES)
                {
                        return status;
                }
        }
        
        
        if(AR_GetStringChar(str, AR_GetStringLength(str) - 1) != L'/')
        {
                status = AR_AppendCharToString(str, L'/');
                if(status != AR_S_YES)
                {
                        return status;
                }
        }
        
        return AR_S_YES;
}


arStatus_t		AR_GetNullPath(arString_t *str)
{
		AR_ASSERT(str != NULL);
		return AR_SetString(str, L"/dev/null");
}

arStatus_t		AR_GetExpandPath(const wchar_t *path, arString_t *expanded_path)
{
        arStatus_t status;
        const wchar_t *b,*e;
        arString_t *tmp, *var;
        AR_ASSERT(path != NULL);
        
        status = AR_S_YES;
        b = path; 
        e = path + AR_wcslen(path);
        AR_ClearString(expanded_path);
        
        tmp = AR_CreateString();
        var = AR_CreateString();
        if(tmp == NULL || var == NULL)
        {
                status = AR_E_NOMEM;
                goto END_POINT;
        }
        
        if(b < e && *b == L'~')
        {
                ++b;
                
                if(b < e && *b == L'/')
                {
                        status = AR_GetHomePath(tmp);
                        if(status != AR_S_YES)
                        {
                                goto END_POINT;
                        }
                       
                        status = AR_AppendString(expanded_path, AR_GetStringCString(tmp));
                        
                        if(status != AR_S_YES)
                        {
                                goto END_POINT;
                        }
                        ++b;
                }else
                {
                        status = AR_AppendString(expanded_path, L"~");
                        if(status != AR_S_YES)
                        {
                                goto END_POINT;
                        }
                }
        }
        
        
        while(b < e)
        {
                if(*b == L'$')
                {
                        AR_ClearString(var);
                        ++b;
                        if(b < e && *b == L'{')
                        {
                                ++b;
                                while(b < e && *b != L'}')
                                {
                                        status = AR_AppendCharToString(var, *b);
                                        if(status != AR_S_YES)
                                        {
                                                goto END_POINT;
                                        }
                                        
                                        ++b;
                                }
                                
                                if(b < e)
                                {
                                        ++b;
                                }
                        }else
                        {
                                while(b < e && (AR_iswalnum(*b) || *b == L'_'))
                                {
                                        status = AR_AppendCharToString(var, *b);
                                        if(status != AR_S_YES)
                                        {
                                                goto END_POINT;
                                        }
                                        ++b;
                                }
                                
                        }
                        
                        status = AR_getenv(AR_GetStringCString(var), tmp);
                        
                        if(status != AR_S_YES)
                        {
                                goto END_POINT;
                        }
                        
                        status = AR_AppendString(expanded_path, AR_GetStringCString(tmp));
                        
                        if(status != AR_S_YES)
                        {
                                goto END_POINT;
                        }
                }else
                {
                        status = AR_AppendCharToString(expanded_path, *b);
                        if(status != AR_S_YES)
                        {
                                goto END_POINT;
                        }
                        ++b;
                }
        }
        
        
        status = AR_SetString(tmp, AR_GetStringCString(expanded_path));
        
        if(status != AR_S_YES)
        {
                goto END_POINT;
        }
        
        AR_ClearString(expanded_path);
        
        b = AR_GetStringCString(tmp);
        e = b + AR_wcslen(b);
        
        while(b < e)
        {
                if(*b == L'/')
                {
                        if(b + 1 < e && *(b + 1) == L'/')
                        {
                                b++;
                                continue;
                        }
                        
                }
                
                status = AR_AppendCharToString(expanded_path, *b);
                        
                if(status != AR_S_YES)
                {
                        goto END_POINT;
                }
                
                ++b;
        }
                                                 
END_POINT:
        if(tmp != NULL)
        {
                AR_DestroyString(tmp);
                tmp = NULL;
        }
        
        if(var != NULL)
        {
                AR_DestroyString(var);
                var = NULL;
        }
        
        return status;
}



#else
		#error "Unknown OS!"
#endif


AR_NAMESPACE_END

