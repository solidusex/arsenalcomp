//
//  marshal_die_test.c
//  Test
//
//  Created by Solidus on 12-8-28.
//  Copyright (c) 2012年 none. All rights reserved.
//


#include <map>
#include <vector>
#include <list>
#include <string>
#include <set>
#include "Arsenal.h"




#if defined(__cplusplus)
using namespace ARSpace;
#define MARSHAL_DIE_BEGIN extern "C"{
#define MARSHAL_DIE_END }
#endif



MARSHAL_DIE_BEGIN


typedef enum
{
        BYET_T,

        INT8_T,
        UINTT8_T,
        
        INT16_T,
        UINTT16_T,
        
        INT32_T,
        UINTT32_T,
        
        INT64_T,
        UINTT64_T,

        
        CHAR_T,
        WCHAR_T,
        
        FLOAT_T,
        DOUBLE_T,
        
        CUSTOM_T,
}FieldType_t;


const wchar_t* __g_inner_type[CUSTOM_T] = 
{
        L"byte_t",
        L"int_8_t",
        L"uint_8_t",
        L"int_16_t",
        L"uint_16_t",
        L"int_32_t",
        L"uint_32_t",
        L"int_64_t",
        L"uint_64_t",
        L"char",
        L"wchar_t",
        L"float",
        L"double",
};


static bool_t is_inner_type(FieldType_t t)
{
        switch(t)
        {
                case BYET_T:
                case INT8_T:
                case UINTT8_T:
                case INT16_T:
                case UINTT16_T:
                case INT32_T:
                case UINTT32_T:
                case INT64_T:
                case UINTT64_T:
                case CHAR_T:
                case WCHAR_T:
                case FLOAT_T:
                case DOUBLE_T:
                        return true;
                case CUSTOM_T:
                default:
                        return false;
        };
}



static bool_t is_inner_type_name(const wchar_t *name)
{
        for(size_t i = 0; i < CUSTOM_T; ++i)
        {
                if(AR_wcscmp(name, __g_inner_type[i]) == 0)
                {
                        return true;
                }
        }
        
        return false;
}

static const wchar_t* get_inner_type_name(FieldType_t t)
{
        AR_ASSERT(is_inner_type(t));
        return __g_inner_type[t];
}




static std::wstring generate_inner_type_marshal_code(const std::wstring &sn_obj_name, const std::wstring &obj_name, const std::wstring &field_name, FieldType_t t, bool_t is_array, size_t array_size)
{
        std::wstring ret;
        AR_ASSERT(is_inner_type(t));
        
        if(is_array)
        {
                if(array_size == 0)
                {
                        AR_error(AR_ERR_FATAL, L"empty array size for %ls\r\n",  get_inner_type_name(t));
                }
                
                ret += L"{\n";
                ret += L"ret=";
                
                switch(t)
                {
                        case BYET_T:
                        case CHAR_T:
                        case WCHAR_T:
                                ret += L"SN_CreateObject(SN_STRING_T)\nif(ret == NULL){goto END_POINT;}\n";
                                break;
                        case INT8_T:
                        case UINTT8_T:
                        case INT16_T:
                        case UINTT16_T:
                        case INT32_T:
                        case UINTT32_T:
                        case INT64_T:
                        case UINTT64_T:
                        case FLOAT_T:
                        case DOUBLE_T:
                                ret += L"SN_CreateObject(SN_LIST_T)\nif(ret == NULL){goto END_POINT;}\n";
                                break;
                        default:
                                AR_ASSERT(false);
                                break;
                }
                
                
                
                /*
                        ar_status = SN_InsertToDictObject(sn_obj, obj_name, field_name);
                        if(ar_status != AR_S_YES)
                        {
                                goto END_POINT;
                        }
                 */
                
                
        }else
        {
                
        }
        
        
}


struct Type;

std::vector<Type*>    g_type_list;
typedef std::map<std::wstring, Type*>    FieldMap;

struct Type
{
        FieldType_t             type;
        bool_t                  is_inner_type;

        std::wstring            name;
        
        bool                    is_array;
        size_t                  array_size;
        
        
        FieldMap                fields;
        

        const std::wstring& generate_marshal_code()
        {
                return std::wstring(L"");
        }
        
        const std::wstring& generate_unmarshal_code()
        {
                return std::wstring(L"");
        }
};




std::vector<std::wstring>    g_head_code;
std::vector<std::wstring>    g_tail_code;



void marshal_die_test()
{
        AR_printf(L"%hs\r\n", AR_FUNC_NAME);
        
}



MARSHAL_DIE_END


