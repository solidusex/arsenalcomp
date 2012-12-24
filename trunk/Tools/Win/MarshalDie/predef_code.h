#include <string>



static const std::wstring code_for_macro = L"/************************************************Macro*****************************************************/\r\n\r\n#define __CHECK_ARG(_obj, _fn)\tdo{ if(SN_GetObjectType((_obj)) != SN_DICT_T || AR_wcslen((_fn)) == 0) {ar_status = AR_E_INVAL; goto END_POINT; }}while(0)\r\n";


static const std::wstring code_for_byte = L"/************************************************ar_byte_t*****************************************************/\r\n\r\nstatic arStatus_t\t\t__put_byte_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_byte_t v)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\r\n\t\tar_status = AR_S_YES;\r\n\r\n\t\t__CHECK_ARG(obj,field_name);\r\n\t\t\r\n\t\tar_status = SN_InsertToDictObjectByWcsUInt(obj, field_name, (ar_uint_64_t)v);\r\n\t\t\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__put_byte_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, const ar_byte_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*data_obj;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\tdata_obj = NULL;\r\n\t\t\r\n\t\t__CHECK_ARG(obj,field_name);\r\n\r\n\t\tdata_obj = SN_CreateObject(SN_STRING_T);\r\n\r\n\t\tif(data_obj == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tar_status = SN_SetStringObjectByData(data_obj, arr, arr_size);\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, data_obj);\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tif(data_obj)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tSN_DestroyObject(data_obj);\r\n\t\t\t\t\t\tdata_obj = NULL;\r\n\t\t\t\t}\r\n\t\t}\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__get_byte_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_byte_t *pv)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*int_obj;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\tint_obj = NULL;\r\n\t\t\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tint_obj = SN_FindFromDictObjectByWcs(obj, field_name);\r\n\r\n\t\tif(int_obj == NULL || SN_GetObjectType(int_obj) != SN_INT_T)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_INVAL;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\t\t\r\n\t\t*pv = (ar_uint_8_t)SN_GetUIntObject(int_obj);\r\n\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\nstatic arStatus_t\t\t__get_byte_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_byte_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*data_obj;\r\n\t\tar_int_t\t\t\tret;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tdata_obj = SN_FindFromDictObjectByWcs(obj, field_name);\r\n\t\t\r\n\t\tif(data_obj == NULL || SN_GetObjectType(data_obj) != SN_STRING_T)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOTMATCHED;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tret = SN_GetDataFromStringObject(data_obj, arr, arr_size);\r\n\r\n\t\tif(ret < 0 || (size_t)ret != arr_size)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_FAIL;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n";





static const std::wstring code_for_bool = L"\r\n/**********************************************************ar_bool_t***************************************************/\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_bool_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_bool_t v)\r\n{\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\treturn __put_byte_t_to_dict(obj, field_name, (ar_byte_t)v);\r\n}\r\n\r\n\r\n/*这里可以优化，但是，何必呢~~~~*/\r\n\r\nstatic arStatus_t\t\t__put_bool_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_bool_t *arr, size_t arr_size)\r\n{\r\n\t\treturn __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)arr, arr_size);\r\n}\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__get_bool_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_bool_t *pv)\r\n{\r\n\t\tar_byte_t v;\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\tar_status = __get_byte_t_from_dict(obj, field_name, &v);\r\n\t\tif(ar_status == AR_S_YES)\r\n\t\t{\r\n\t\t\t\t*pv = (ar_bool_t)v ? true : false;\r\n\t\t}\r\n\t\treturn ar_status;\r\n\r\n}\r\n\r\n\r\n/*...，同上*/\r\nstatic arStatus_t\t\t__get_bool_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_bool_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);\r\n\r\n\t\tar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, arr_size);\r\n\r\n\t\tif(ar_status == AR_S_YES)\r\n\t\t{\r\n\t\t\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tarr[i] = arr[i] ? true : false;\r\n\t\t\t\t}\r\n\t\t}\r\n\r\n\t\treturn ar_status;\r\n}\r\n\r\n";



static const std::wstring code_for_char = L"\r\n/**********************************************************char***************************************************/\r\n\r\nstatic arStatus_t\t\t__put_char_to_dict(snObject_t *obj, const wchar_t *field_name, char v)\r\n{\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\treturn __put_byte_t_to_dict(obj, field_name, (ar_byte_t)v);\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__put_char_array_to_dict(snObject_t *obj, const wchar_t *field_name, const char *v_str)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t *str_obj;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && v_str != NULL);\r\n\t\t\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\tstr_obj = NULL;\r\n\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tstr_obj = SN_CreateObject(SN_STRING_T);\r\n\r\n\t\tif(str_obj == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tar_status = SN_SetStringObjectByStr(str_obj, v_str);\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, str_obj);\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tif(str_obj)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tSN_DestroyObject(str_obj);\r\n\t\t\t\t\t\tstr_obj = NULL;\r\n\t\t\t\t}\r\n\t\t}\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_char_from_dict(snObject_t *obj, const wchar_t *field_name, char *pv)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tar_byte_t\t\t\tv;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tar_status = __get_byte_t_from_dict(obj, field_name, &v);\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\treturn ar_status;\r\n\t\t}\r\n\r\n\t\t*pv = (char)v;\r\n\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__get_char_array_from_dict(snObject_t *obj, const wchar_t *field_name, char *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*str_obj;\r\n\t\tar_int_t\t\t\tret;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tstr_obj = SN_FindFromDictObjectByWcs(obj, field_name);\r\n\t\t\r\n\t\tif(str_obj == NULL || SN_GetObjectType(str_obj) != SN_STRING_T)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOTMATCHED;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\t\t\r\n\t\tret = SN_GetStrFromStringObject(str_obj, arr, arr_size);\r\n\r\n\t\tif(ret < 0)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_FAIL;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tarr[ret] = '\\0';\r\n\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n";




static const std::wstring code_for_wchar = L"\r\n/**********************************************************wchar_t***************************************************/\r\n\r\n\r\nstatic arStatus_t\t\t__put_wchar_t_to_dict(snObject_t *obj, const wchar_t *field_name, wchar_t v)\r\n{\r\n\t\tar_uint_32_t t;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\r\n\t\tt = (ar_uint_32_t)v;\r\n\t\tt = AR_LTON_32(t);\r\n\t\treturn __put_byte_t_array_to_dict(obj, field_name, (const ar_byte_t*)&t, sizeof(ar_uint_32_t));\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_wchar_t_from_dict(snObject_t *obj, const wchar_t *field_name, wchar_t *pv)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tar_uint_32_t\t\tt;\r\n\t\t\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\t\t\r\n\t\tar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)&t, sizeof(ar_uint_32_t));\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\treturn ar_status;\r\n\t\t}\r\n\r\n\t\tt = AR_NTOL_32(t);\r\n\t\t*pv = (wchar_t)t;\r\n\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_wchar_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, const wchar_t *v_str)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t *str_obj;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && v_str != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\tstr_obj = NULL;\r\n\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tstr_obj = SN_CreateObject(SN_STRING_T);\r\n\r\n\t\tif(str_obj == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tar_status = SN_SetStringObjectByWcs(str_obj, v_str);\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, str_obj);\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tif(str_obj)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tSN_DestroyObject(str_obj);\r\n\t\t\t\t\t\tstr_obj = NULL;\r\n\t\t\t\t}\r\n\t\t}\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__get_wchar_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, wchar_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*str_obj;\r\n\t\tar_int_t\t\t\tret;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);\r\n\r\n\t\tar_status = AR_S_YES;\r\n\t\t\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tstr_obj = SN_FindFromDictObjectByWcs(obj, field_name);\r\n\t\t\r\n\t\tif(str_obj == NULL || SN_GetObjectType(str_obj) != SN_STRING_T)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOTMATCHED;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tret = SN_GetWcsFromStringObject(str_obj, arr, arr_size);\r\n\r\n\t\tif(ret < 0)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_FAIL;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tarr[ret] = L'\\0';\r\n\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n";


static const std::wstring code_for_integer_8 = L"\r\n/************************************************Integer 8*****************************************************/\r\n\r\nstatic arStatus_t\t\t__put_int_8_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_8_t v)\r\n{\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\treturn __put_byte_t_to_dict(obj, field_name, (ar_byte_t)v);\r\n}\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_int_8_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_8_t *arr, size_t arr_size)\r\n{\r\n\t\treturn __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)arr, arr_size);\r\n}\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__get_int_8_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_8_t *pv)\r\n{\r\n\t\tar_byte_t v;\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\tar_status = __get_byte_t_from_dict(obj, field_name, &v);\r\n\t\tif(ar_status == AR_S_YES)\r\n\t\t{\r\n\t\t\t\t*pv = (ar_int_8_t)v;\r\n\t\t}\r\n\t\treturn ar_status;\r\n\r\n}\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__get_int_8_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_8_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);\r\n\r\n\t\tar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, arr_size);\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_uint_8_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_8_t v)\r\n{\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\treturn __put_byte_t_to_dict(obj, field_name, (ar_byte_t)v);\r\n}\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_uint_8_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_8_t *arr, size_t arr_size)\r\n{\r\n\t\treturn __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)arr, arr_size);\r\n}\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__get_uint_8_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_8_t *pv)\r\n{\r\n\t\tar_byte_t v;\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\tar_status = __get_byte_t_from_dict(obj, field_name, &v);\r\n\t\tif(ar_status == AR_S_YES)\r\n\t\t{\r\n\t\t\t\t*pv = (ar_uint_8_t)v;\r\n\t\t}\r\n\t\treturn ar_status;\r\n\r\n}\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__get_uint_8_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_8_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);\r\n\r\n\t\tar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, arr_size);\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n";


static const std::wstring code_for_integer_16 = L"\r\n/************************************************Integer 16*****************************************************/\r\n\r\nstatic arStatus_t\t\t__put_int_16_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_16_t v)\r\n{\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\tv = AR_LTON_16(v);\r\n\t\treturn __put_byte_t_array_to_dict(obj, field_name, (const ar_byte_t*)&v, sizeof(ar_int_16_t));\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_int_16_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_16_t *pv)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\t\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\t\t\r\n\t\tar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)pv, sizeof(ar_int_16_t));\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\treturn ar_status;\r\n\t\t}\r\n\r\n\t\t*pv = AR_NTOL_16(*pv);\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_int_16_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_16_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tar_int_16_t\t\t*tmp_arr;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\ttmp_arr = NULL;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\ttmp_arr = AR_NEWARR(ar_int_16_t, arr_size);\r\n\t\t\r\n\t\tif(tmp_arr == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\ttmp_arr[i] = AR_LTON_16(arr[i]);\r\n\t\t}\r\n\r\n\t\tar_status = __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);\r\n\r\nEND_POINT:\r\n\r\n\t\tif(tmp_arr)\r\n\t\t{\r\n\t\t\t\tAR_DEL(tmp_arr);\r\n\t\t\t\ttmp_arr = NULL;\r\n\t\t}\r\n\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_int_16_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_16_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\t\r\n\t\tar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, sizeof(arr[0]) * arr_size);\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\tarr[i] = AR_NTOL_16(arr[i]);\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\t\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_uint_16_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_16_t v)\r\n{\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\tv = AR_LTON_U16(v);\r\n\t\treturn __put_byte_t_array_to_dict(obj, field_name, (const ar_byte_t*)&v, sizeof(ar_uint_16_t));\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_uint_16_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_16_t *pv)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\t\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\t\t\r\n\t\tar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)pv, sizeof(ar_uint_16_t));\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\treturn ar_status;\r\n\t\t}\r\n\r\n\t\t*pv = AR_NTOL_U16(*pv);\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_uint_16_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_16_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tar_uint_16_t\t\t*tmp_arr;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\ttmp_arr = NULL;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\ttmp_arr = AR_NEWARR(ar_uint_16_t, arr_size);\r\n\t\t\r\n\t\tif(tmp_arr == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\ttmp_arr[i] = AR_LTON_U16(arr[i]);\r\n\t\t}\r\n\r\n\t\tar_status = __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);\r\n\r\nEND_POINT:\r\n\r\n\t\tif(tmp_arr)\r\n\t\t{\r\n\t\t\t\tAR_DEL(tmp_arr);\r\n\t\t\t\ttmp_arr = NULL;\r\n\t\t}\r\n\t\t\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_uint_16_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_16_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\t\r\n\t\tar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, sizeof(arr[0]) * arr_size);\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\tarr[i] = AR_NTOL_U16(arr[i]);\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\t\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\n\r\n";



static const std::wstring code_for_integer_32 = L"\r\n/************************************************Integer 32*****************************************************/\r\n\r\nstatic arStatus_t\t\t__put_int_32_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_32_t v)\r\n{\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\tv = AR_LTON_32(v);\r\n\t\treturn __put_byte_t_array_to_dict(obj, field_name, (const ar_byte_t*)&v, sizeof(ar_int_32_t));\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_int_32_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_32_t *pv)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\t\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\t\t\r\n\t\tar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)pv, sizeof(ar_int_32_t));\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\treturn ar_status;\r\n\t\t}\r\n\r\n\t\t*pv = AR_NTOL_32(*pv);\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_int_32_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_32_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tar_int_32_t\t\t*tmp_arr;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\ttmp_arr = NULL;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\ttmp_arr = AR_NEWARR(ar_int_32_t, arr_size);\r\n\t\t\r\n\t\tif(tmp_arr == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\ttmp_arr[i] = AR_LTON_32(arr[i]);\r\n\t\t}\r\n\r\n\t\tar_status = __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);\r\n\r\nEND_POINT:\r\n\r\n\t\tif(tmp_arr)\r\n\t\t{\r\n\t\t\t\tAR_DEL(tmp_arr);\r\n\t\t\t\ttmp_arr = NULL;\r\n\t\t}\r\n\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_int_32_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_32_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\t\r\n\t\tar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, sizeof(arr[0]) * arr_size);\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\tarr[i] = AR_NTOL_32(arr[i]);\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\t\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_uint_32_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_32_t v)\r\n{\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\tv = AR_LTON_U32(v);\r\n\t\treturn __put_byte_t_array_to_dict(obj, field_name, (const ar_byte_t*)&v, sizeof(ar_uint_32_t));\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_uint_32_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_32_t *pv)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\t\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\t\t\r\n\t\tar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)pv, sizeof(ar_uint_32_t));\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\treturn ar_status;\r\n\t\t}\r\n\r\n\t\t*pv = AR_NTOL_U32(*pv);\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_uint_32_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_32_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tar_uint_32_t\t\t*tmp_arr;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\ttmp_arr = NULL;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\ttmp_arr = AR_NEWARR(ar_uint_32_t, arr_size);\r\n\t\t\r\n\t\tif(tmp_arr == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\ttmp_arr[i] = AR_LTON_U32(arr[i]);\r\n\t\t}\r\n\r\n\t\tar_status = __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);\r\n\r\nEND_POINT:\r\n\r\n\t\tif(tmp_arr)\r\n\t\t{\r\n\t\t\t\tAR_DEL(tmp_arr);\r\n\t\t\t\ttmp_arr = NULL;\r\n\t\t}\r\n\t\t\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_uint_32_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_32_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\t\r\n\t\tar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, sizeof(arr[0]) * arr_size);\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\tarr[i] = AR_NTOL_U32(arr[i]);\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\t\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\n";


static const std::wstring code_for_integer_64 = L"\r\n/************************************************Integer 64*****************************************************/\r\n\r\nstatic arStatus_t\t\t__put_int_64_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_64_t v)\r\n{\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\tv = AR_LTON_64(v);\r\n\t\treturn __put_byte_t_array_to_dict(obj, field_name, (const ar_byte_t*)&v, sizeof(ar_int_64_t));\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_int_64_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_64_t *pv)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\t\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\t\t\r\n\t\tar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)pv, sizeof(ar_int_64_t));\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\treturn ar_status;\r\n\t\t}\r\n\r\n\t\t*pv = AR_NTOL_64(*pv);\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_int_64_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_64_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tar_int_64_t\t\t*tmp_arr;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\ttmp_arr = NULL;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\ttmp_arr = AR_NEWARR(ar_int_64_t, arr_size);\r\n\t\t\r\n\t\tif(tmp_arr == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\ttmp_arr[i] = AR_LTON_64(arr[i]);\r\n\t\t}\r\n\r\n\t\tar_status = __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);\r\n\r\nEND_POINT:\r\n\r\n\t\tif(tmp_arr)\r\n\t\t{\r\n\t\t\t\tAR_DEL(tmp_arr);\r\n\t\t\t\ttmp_arr = NULL;\r\n\t\t}\r\n\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_int_64_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_64_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, sizeof(arr[0]) * arr_size);\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\tarr[i] = AR_NTOL_64(arr[i]);\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\t\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_uint_64_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_64_t v)\r\n{\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\tv = AR_LTON_U64(v);\r\n\t\treturn __put_byte_t_array_to_dict(obj, field_name, (const ar_byte_t*)&v, sizeof(ar_uint_64_t));\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_uint_64_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_64_t *pv)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\t\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\t\t\r\n\t\tar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)pv, sizeof(ar_uint_64_t));\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\treturn ar_status;\r\n\t\t}\r\n\r\n\t\t*pv = AR_NTOL_U64(*pv);\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_uint_64_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_64_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tar_uint_64_t\t\t*tmp_arr;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\ttmp_arr = NULL;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\ttmp_arr = AR_NEWARR(ar_uint_64_t, arr_size);\r\n\t\t\r\n\t\tif(tmp_arr == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\ttmp_arr[i] = AR_LTON_U64(arr[i]);\r\n\t\t}\r\n\r\n\t\tar_status = __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);\r\n\r\nEND_POINT:\r\n\r\n\t\tif(tmp_arr)\r\n\t\t{\r\n\t\t\t\tAR_DEL(tmp_arr);\r\n\t\t\t\ttmp_arr = NULL;\r\n\t\t}\r\n\t\t\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\nstatic arStatus_t\t\t__get_uint_64_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_64_t *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\t\r\n\t\tar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, sizeof(arr[0]) * arr_size);\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\tarr[i] = AR_NTOL_U64(arr[i]);\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\t\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\n";



static const std::wstring code_for_float = L"\r\n/******************************************************float****************************************************************/\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_float_to_dict(snObject_t *obj, const wchar_t *field_name, float v)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\t\r\n\r\n\t\tar_status = SN_InsertToDictObjectByWcsFloat(obj, field_name, (double)v);\r\n\r\nEND_POINT:\r\n\r\n\t\treturn ar_status;\r\n}\r\n\r\nstatic arStatus_t\t\t__get_float_from_dict(snObject_t *obj, const wchar_t *field_name, float *pv)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*flt_obj;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tflt_obj = SN_FindFromDictObjectByWcs(obj, field_name);\r\n\r\n\t\tif(flt_obj == NULL || SN_GetObjectType(flt_obj) != SN_FLOAT_T)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_INVAL;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\t*pv = (float)SN_GetFloatObject(flt_obj);\r\n\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_float_array_to_dict(snObject_t *obj, const wchar_t *field_name, float *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*float_arr_obj;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\t\tAR_ASSERT(arr_size > 0);\r\n\r\n\t\tar_status = AR_S_YES;\r\n\t\tfloat_arr_obj = NULL;\r\n\t\t\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\r\n\t\tfloat_arr_obj = SN_CreateObject(SN_LIST_T);\r\n\t\tif(float_arr_obj == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\tar_status = SN_InsertToListObjectByFloat(float_arr_obj, (double)arr[i]);\r\n\t\t\t\t\r\n\t\t\t\tif(ar_status != AR_S_YES)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tgoto END_POINT;\r\n\t\t\t\t}\r\n\t\t}\r\n\r\n\t\tar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, float_arr_obj);\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tif(float_arr_obj != NULL)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tSN_DestroyObject(float_arr_obj);\r\n\t\t\t\t\t\tfloat_arr_obj = NULL;\r\n\t\t\t\t}\r\n\t\t}\r\n\r\n\t\treturn ar_status;\r\n\r\n}\r\n\r\nstatic arStatus_t\t\t__get_float_array_from_dict(snObject_t *obj, const wchar_t *field_name, float *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*arr_list;\r\n\t\tsize_t\t\t\ti;\r\n\t\tsize_t\t\t\tlist_cnt;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);\r\n\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tarr_list = SN_FindFromDictObjectByWcs(obj, field_name);\r\n\t\t\r\n\t\tif(arr_list == NULL || SN_GetObjectType(arr_list) != SN_LIST_T)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOTMATCHED;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tlist_cnt = SN_GetListObjectCount(arr_list);\r\n\r\n\t\tif(list_cnt != arr_size)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOTMATCHED;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tfor(i = 0; i < SN_GetListObjectCount(arr_list); ++i)\r\n\t\t{\r\n\t\t\t\tsnObject_t *flt_obj = SN_GetFromListObject(arr_list, i);\r\n\t\t\t\tif(flt_obj == NULL || SN_GetObjectType(flt_obj) != SN_FLOAT_T)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tar_status = AR_E_NOTMATCHED;\r\n\t\t\t\t\t\tgoto END_POINT;\r\n\t\t\t\t}\r\n\r\n\t\t\t\tarr[i] = (float)SN_GetFloatObject(flt_obj);\r\n\t\t}\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n";


static const std::wstring code_for_double = L"/******************************************************double****************************************************************/\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_double_to_dict(snObject_t *obj, const wchar_t *field_name, double v)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL);\r\n\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tar_status = SN_InsertToDictObjectByWcsFloat(obj, field_name, v);\r\n\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__put_double_array_to_dict(snObject_t *obj, const wchar_t *field_name, double *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*float_arr_obj;\r\n\t\tsize_t\t\t\ti;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);\r\n\t\t\r\n\r\n\t\tar_status = AR_S_YES;\r\n\t\tfloat_arr_obj = NULL;\r\n\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tfloat_arr_obj = SN_CreateObject(SN_LIST_T);\r\n\t\tif(float_arr_obj == NULL)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOMEM;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tfor(i = 0; i < arr_size; ++i)\r\n\t\t{\r\n\t\t\t\tar_status = SN_InsertToListObjectByFloat(float_arr_obj, arr[i]);\r\n\t\t\t\t\r\n\t\t\t\tif(ar_status != AR_S_YES)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tgoto END_POINT;\r\n\t\t\t\t}\r\n\t\t}\r\n\r\n\t\tar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, float_arr_obj);\r\n\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\nEND_POINT:\r\n\t\tif(ar_status != AR_S_YES)\r\n\t\t{\r\n\t\t\t\tif(float_arr_obj != NULL)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tSN_DestroyObject(float_arr_obj);\r\n\t\t\t\t\t\tfloat_arr_obj = NULL;\r\n\t\t\t\t}\r\n\t\t}\r\n\r\n\t\treturn ar_status;\r\n\r\n}\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__get_double_from_dict(snObject_t *obj, const wchar_t *field_name, double *pv)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*flt_obj;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);\r\n\t\t\r\n\t\tar_status = AR_S_YES;\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tflt_obj = SN_FindFromDictObjectByWcs(obj, field_name);\r\n\r\n\t\t\r\n\t\tif(flt_obj == NULL || SN_GetObjectType(flt_obj) != SN_FLOAT_T)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOTMATCHED;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\t*pv = SN_GetFloatObject(flt_obj);\r\n\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n\r\n\r\nstatic arStatus_t\t\t__get_double_array_from_dict(snObject_t *obj, const wchar_t *field_name, double *arr, size_t arr_size)\r\n{\r\n\t\tarStatus_t\t\tar_status;\r\n\t\tsnObject_t\t\t*arr_list;\r\n\t\tsize_t\t\t\ti;\r\n\t\tsize_t\t\t\tlist_cnt;\r\n\t\tAR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);\r\n\t\t\r\n\r\n\t\tar_status = AR_S_YES;\r\n\r\n\t\t__CHECK_ARG(obj, field_name);\r\n\r\n\t\tarr_list = SN_FindFromDictObjectByWcs(obj, field_name);\r\n\t\t\r\n\t\tif(SN_GetObjectType(arr_list) != SN_LIST_T)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOTMATCHED;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tlist_cnt = SN_GetListObjectCount(arr_list);\r\n\r\n\t\tif(list_cnt != arr_size)\r\n\t\t{\r\n\t\t\t\tar_status = AR_E_NOTMATCHED;\r\n\t\t\t\tgoto END_POINT;\r\n\t\t}\r\n\r\n\t\tfor(i = 0; i < SN_GetListObjectCount(arr_list); ++i)\r\n\t\t{\r\n\t\t\t\tsnObject_t *flt_obj = SN_GetFromListObject(arr_list, i);\r\n\t\t\t\tif(flt_obj == NULL || SN_GetObjectType(flt_obj) != SN_FLOAT_T)\r\n\t\t\t\t{\r\n\t\t\t\t\t\tar_status = AR_E_NOTMATCHED;\r\n\t\t\t\t\t\tgoto END_POINT;\r\n\t\t\t\t}\r\n\r\n\t\t\t\tarr[i] = SN_GetFloatObject(flt_obj);\r\n\t\t}\r\nEND_POINT:\r\n\t\treturn ar_status;\r\n}\r\n\r\n";





static const wchar_t* __g_inner_type_marshal_call_name[CUSTOM_T] = 
{
		L"__put_bool_t_to_dict",
		L"__put_byte_t_to_dict",

		L"__put_int_8_t_to_dict",
		L"__put_uint_8_t_to_dict",

		L"__put_int_16_t_to_dict",
		L"__put_uint_16_t_to_dict",

		L"__put_int_32_t_to_dict",
		L"__put_uint_32_t_to_dict",
        
		L"__put_int_64_t_to_dict",
		L"__put_uint_64_t_to_dict",

		L"__put_float_to_dict",
		L"__put_double_to_dict",

		L"__put_char_to_dict",
		L"__put_wchar_t_to_dict",
};


static const wchar_t* __g_inner_type_marshal_array_call_name[CUSTOM_T] = 
{
		L"__put_bool_t_array_to_dict",
		L"__put_byte_t_array_to_dict",

		L"__put_int_8_t_array_to_dict",
		L"__put_uint_8_t_array_to_dict",

		L"__put_int_16_t_array_to_dict",
		L"__put_uint_16_t_array_to_dict",

		L"__put_int_32_t_array_to_dict",
		L"__put_uint_32_t_array_to_dict",
        
		L"__put_int_64_t_array_to_dict",
		L"__put_uint_64_t_array_to_dict",

		L"__put_float_array_to_dict",
		L"__put_double_array_to_dict",

		L"__put_char_array_to_dict",
		L"__put_wchar_t_array_to_dict",
};




static const wchar_t* __g_inner_type_unmarshal_call_name[CUSTOM_T] = 
{
		L"__get_bool_t_from_dict",
		L"__get_byte_t_from_dict",

		L"__get_int_8_t_from_dict",
		L"__get_uint_8_t_from_dict",

		L"__get_int_16_t_from_dict",
		L"__get_uint_16_t_from_dict",

		L"__get_int_32_t_from_dict",
		L"__get_uint_32_t_from_dict",
        
		L"__get_int_64_t_from_dict",
		L"__get_uint_64_t_from_dict",

		L"__get_float_from_dict",
		L"__get_double_from_dict",

		L"__get_char_from_dict",
		L"__get_wchar_t_from_dict",
};


static const wchar_t* __g_inner_type_unmarshal_array_call_name[CUSTOM_T] = 
{
		L"__get_bool_t_array_from_dict",
		L"__get_byte_t_array_from_dict",

		L"__get_int_8_t_array_from_dict",
		L"__get_uint_8_t_array_from_dict",

		L"__get_int_16_t_array_from_dict",
		L"__get_uint_16_t_array_from_dict",

		L"__get_int_32_t_array_from_dict",
		L"__get_uint_32_t_array_from_dict",
        
		L"__get_int_64_t_array_from_dict",
		L"__get_uint_64_t_array_from_dict",

		L"__get_float_array_from_dict",
		L"__get_double_array_from_dict",

		L"__get_char_array_from_dict",
		L"__get_wchar_t_array_from_dict",
};




