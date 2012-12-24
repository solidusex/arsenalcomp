/************************************************Macro*****************************************************/

#define __CHECK_ARG(_obj, _fn)	do{ if(SN_GetObjectType((_obj)) != SN_DICT_T || AR_wcslen((_fn)) == 0) {ar_status = AR_E_INVAL; goto END_POINT; }}while(0)


/************************************************ar_byte_t*****************************************************/

static arStatus_t		__put_byte_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_byte_t v)
{
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL);

		ar_status = AR_S_YES;

		__CHECK_ARG(obj,field_name);
		
		ar_status = SN_InsertToDictObjectByWcsUInt(obj, field_name, (ar_uint_64_t)v);
		
END_POINT:
		return ar_status;
}


static arStatus_t		__put_byte_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, const ar_byte_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*data_obj;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		data_obj = NULL;
		
		__CHECK_ARG(obj,field_name);

		data_obj = SN_CreateObject(SN_STRING_T);

		if(data_obj == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		ar_status = SN_SetStringObjectByData(data_obj, arr, arr_size);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

		ar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, data_obj);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:
		if(ar_status != AR_S_YES)
		{
				if(data_obj)
				{
						SN_DestroyObject(data_obj);
						data_obj = NULL;
				}
		}
		return ar_status;
}



static arStatus_t		__get_byte_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_byte_t *pv)
{
		arStatus_t		ar_status;
		snObject_t		*int_obj;
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		
		ar_status = AR_S_YES;
		int_obj = NULL;
		
		__CHECK_ARG(obj, field_name);

		int_obj = SN_FindFromDictObjectByWcs(obj, field_name);

		if(int_obj == NULL || SN_GetObjectType(int_obj) != SN_INT_T)
		{
				ar_status = AR_E_INVAL;
				goto END_POINT;
		}
		
		*pv = (ar_uint_8_t)SN_GetUIntObject(int_obj);

END_POINT:
		return ar_status;
}

static arStatus_t		__get_byte_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_byte_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*data_obj;
		ar_int_t			ret;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;

		__CHECK_ARG(obj, field_name);

		data_obj = SN_FindFromDictObjectByWcs(obj, field_name);
		
		if(data_obj == NULL || SN_GetObjectType(data_obj) != SN_STRING_T)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		ret = SN_GetDataFromStringObject(data_obj, arr, arr_size);

		if(ret < 0 || (size_t)ret != arr_size)
		{
				ar_status = AR_E_FAIL;
				goto END_POINT;
		}

END_POINT:
		return ar_status;
}




/**********************************************************bool_t***************************************************/



static arStatus_t		__put_bool_t_to_dict(snObject_t *obj, const wchar_t *field_name, bool_t v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		return __put_byte_t_to_dict(obj, field_name, (ar_byte_t)v);
}


/*��������Ż������ǣ��α���~~~~*/

static arStatus_t		__put_bool_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, bool_t *arr, size_t arr_size)
{
		return __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)arr, arr_size);
}





static arStatus_t		__get_bool_t_from_dict(snObject_t *obj, const wchar_t *field_name, bool_t *pv)
{
		ar_byte_t v;
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		ar_status = __get_byte_t_from_dict(obj, field_name, &v);
		if(ar_status == AR_S_YES)
		{
				*pv = (bool_t)v ? true : false;
		}
		return ar_status;

}


/*...��ͬ��*/
static arStatus_t		__get_bool_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, bool_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);

		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, arr_size);

		if(ar_status == AR_S_YES)
		{
				for(i = 0; i < arr_size; ++i)
				{
						arr[i] = arr[i] ? true : false;
				}
		}

		return ar_status;
}




/**********************************************************char***************************************************/

static arStatus_t		__put_char_to_dict(snObject_t *obj, const wchar_t *field_name, char v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		return __put_byte_t_to_dict(obj, field_name, (ar_byte_t)v);
}


static arStatus_t		__put_char_array_to_dict(snObject_t *obj, const wchar_t *field_name, const char *v_str)
{
		arStatus_t		ar_status;
		snObject_t *str_obj;
		AR_ASSERT(obj != NULL && field_name != NULL && v_str != NULL);
		
		
		ar_status = AR_S_YES;
		str_obj = NULL;

		__CHECK_ARG(obj, field_name);

		str_obj = SN_CreateObject(SN_STRING_T);

		if(str_obj == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		ar_status = SN_SetStringObjectByStr(str_obj, v_str);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

		ar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, str_obj);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:
		if(ar_status != AR_S_YES)
		{
				if(str_obj)
				{
						SN_DestroyObject(str_obj);
						str_obj = NULL;
				}
		}
		return ar_status;
}


static arStatus_t		__get_char_from_dict(snObject_t *obj, const wchar_t *field_name, char *pv)
{
		arStatus_t		ar_status;
		ar_byte_t			v;
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		
		ar_status = AR_S_YES;
		
		__CHECK_ARG(obj, field_name);

		ar_status = __get_byte_t_from_dict(obj, field_name, &v);

		if(ar_status != AR_S_YES)
		{
				return ar_status;
		}

		*pv = (char)v;

END_POINT:
		return ar_status;
}




static arStatus_t		__get_char_array_from_dict(snObject_t *obj, const wchar_t *field_name, char *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*str_obj;
		ar_int_t			ret;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);

		str_obj = SN_FindFromDictObjectByWcs(obj, field_name);
		
		if(str_obj == NULL || SN_GetObjectType(str_obj) != SN_STRING_T)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}
		
		ret = SN_GetStrFromStringObject(str_obj, arr, arr_size);

		if(ret < 0)
		{
				ar_status = AR_E_FAIL;
				goto END_POINT;
		}

		arr[ret] = '\0';

END_POINT:
		return ar_status;
}





/**********************************************************wchar_t***************************************************/


static arStatus_t		__put_wchar_t_to_dict(snObject_t *obj, const wchar_t *field_name, wchar_t v)
{
		ar_uint_32_t t;
		AR_ASSERT(obj != NULL && field_name != NULL);

		t = (ar_uint_32_t)v;
		t = AR_LTON_32(t);
		return __put_byte_t_array_to_dict(obj, field_name, (const ar_byte_t*)&t, sizeof(ar_uint_32_t));
}


static arStatus_t		__get_wchar_t_from_dict(snObject_t *obj, const wchar_t *field_name, wchar_t *pv)
{
		arStatus_t		ar_status;
		ar_uint_32_t		t;
		
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		
		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);
		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)&t, sizeof(ar_uint_32_t));

		if(ar_status != AR_S_YES)
		{
				return ar_status;
		}

		t = AR_NTOL_32(t);
		*pv = (wchar_t)t;

END_POINT:
		return ar_status;
}



static arStatus_t		__put_wchar_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, const wchar_t *v_str)
{
		arStatus_t		ar_status;
		snObject_t *str_obj;
		AR_ASSERT(obj != NULL && field_name != NULL && v_str != NULL);
		
		ar_status = AR_S_YES;
		str_obj = NULL;

		__CHECK_ARG(obj, field_name);

		str_obj = SN_CreateObject(SN_STRING_T);

		if(str_obj == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		ar_status = SN_SetStringObjectByWcs(str_obj, v_str);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

		ar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, str_obj);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:
		if(ar_status != AR_S_YES)
		{
				if(str_obj)
				{
						SN_DestroyObject(str_obj);
						str_obj = NULL;
				}
		}
		return ar_status;
}






static arStatus_t		__get_wchar_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, wchar_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*str_obj;
		ar_int_t			ret;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);

		ar_status = AR_S_YES;
		
		__CHECK_ARG(obj, field_name);

		str_obj = SN_FindFromDictObjectByWcs(obj, field_name);
		
		if(str_obj == NULL || SN_GetObjectType(str_obj) != SN_STRING_T)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		ret = SN_GetWcsFromStringObject(str_obj, arr, arr_size);

		if(ret < 0)
		{
				ar_status = AR_E_FAIL;
				goto END_POINT;
		}

		arr[ret] = L'\0';

END_POINT:
		return ar_status;
}



/************************************************Integer 8*****************************************************/

static arStatus_t		__put_int_8_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_8_t v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		return __put_byte_t_to_dict(obj, field_name, (ar_byte_t)v);
}



static arStatus_t		__put_int_8_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_8_t *arr, size_t arr_size)
{
		return __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)arr, arr_size);
}





static arStatus_t		__get_int_8_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_8_t *pv)
{
		ar_byte_t v;
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		ar_status = __get_byte_t_from_dict(obj, field_name, &v);
		if(ar_status == AR_S_YES)
		{
				*pv = (ar_int_8_t)v;
		}
		return ar_status;

}



static arStatus_t		__get_int_8_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_8_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);

		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, arr_size);
		return ar_status;
}






static arStatus_t		__put_uint_8_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_8_t v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		return __put_byte_t_to_dict(obj, field_name, (ar_byte_t)v);
}



static arStatus_t		__put_uint_8_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_8_t *arr, size_t arr_size)
{
		return __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)arr, arr_size);
}





static arStatus_t		__get_uint_8_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_8_t *pv)
{
		ar_byte_t v;
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		ar_status = __get_byte_t_from_dict(obj, field_name, &v);
		if(ar_status == AR_S_YES)
		{
				*pv = (ar_uint_8_t)v;
		}
		return ar_status;

}



static arStatus_t		__get_uint_8_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_8_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);

		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, arr_size);
		return ar_status;
}




/************************************************Integer 16*****************************************************/

static arStatus_t		__put_int_16_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_16_t v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		v = AR_LTON_16(v);
		return __put_byte_t_array_to_dict(obj, field_name, (const ar_byte_t*)&v, sizeof(ar_int_16_t));
}


static arStatus_t		__get_int_16_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_16_t *pv)
{
		arStatus_t		ar_status;
		
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		
		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);
		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)pv, sizeof(ar_int_16_t));

		if(ar_status != AR_S_YES)
		{
				return ar_status;
		}

		*pv = AR_NTOL_16(*pv);
END_POINT:
		return ar_status;
}





static arStatus_t		__put_int_16_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_16_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		ar_int_16_t		*tmp_arr;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		tmp_arr = NULL;
		__CHECK_ARG(obj, field_name);

		tmp_arr = AR_NEWARR(ar_int_16_t, arr_size);
		
		if(tmp_arr == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				tmp_arr[i] = AR_LTON_16(arr[i]);
		}

		ar_status = __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);

END_POINT:

		if(tmp_arr)
		{
				AR_DEL(tmp_arr);
				tmp_arr = NULL;
		}

		return ar_status;
}


static arStatus_t		__get_int_16_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_16_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		
		__CHECK_ARG(obj, field_name);

		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, sizeof(arr[0]) * arr_size);

		for(i = 0; i < arr_size; ++i)
		{
				arr[i] = AR_NTOL_16(arr[i]);
		}

END_POINT:
		
		return ar_status;
}






static arStatus_t		__put_uint_16_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_16_t v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		v = AR_LTON_U16(v);
		return __put_byte_t_array_to_dict(obj, field_name, (const ar_byte_t*)&v, sizeof(ar_uint_16_t));
}


static arStatus_t		__get_uint_16_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_16_t *pv)
{
		arStatus_t		ar_status;
		
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		
		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);
		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)pv, sizeof(ar_uint_16_t));

		if(ar_status != AR_S_YES)
		{
				return ar_status;
		}

		*pv = AR_NTOL_U16(*pv);
END_POINT:
		return ar_status;
}





static arStatus_t		__put_uint_16_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_16_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		ar_uint_16_t		*tmp_arr;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		tmp_arr = NULL;
		__CHECK_ARG(obj, field_name);

		tmp_arr = AR_NEWARR(ar_uint_16_t, arr_size);
		
		if(tmp_arr == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				tmp_arr[i] = AR_LTON_U16(arr[i]);
		}

		ar_status = __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);

END_POINT:

		if(tmp_arr)
		{
				AR_DEL(tmp_arr);
				tmp_arr = NULL;
		}
		
		return ar_status;
}


static arStatus_t		__get_uint_16_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_16_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		
		__CHECK_ARG(obj, field_name);

		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, sizeof(arr[0]) * arr_size);

		for(i = 0; i < arr_size; ++i)
		{
				arr[i] = AR_NTOL_U16(arr[i]);
		}

END_POINT:
		
		return ar_status;
}







/************************************************Integer 32*****************************************************/

static arStatus_t		__put_int_32_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_32_t v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		v = AR_LTON_32(v);
		return __put_byte_t_array_to_dict(obj, field_name, (const ar_byte_t*)&v, sizeof(ar_int_32_t));
}


static arStatus_t		__get_int_32_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_32_t *pv)
{
		arStatus_t		ar_status;
		
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		
		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);
		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)pv, sizeof(ar_int_32_t));

		if(ar_status != AR_S_YES)
		{
				return ar_status;
		}

		*pv = AR_NTOL_32(*pv);
END_POINT:
		return ar_status;
}





static arStatus_t		__put_int_32_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_32_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		ar_int_32_t		*tmp_arr;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		tmp_arr = NULL;
		__CHECK_ARG(obj, field_name);

		tmp_arr = AR_NEWARR(ar_int_32_t, arr_size);
		
		if(tmp_arr == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				tmp_arr[i] = AR_LTON_32(arr[i]);
		}

		ar_status = __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);

END_POINT:

		if(tmp_arr)
		{
				AR_DEL(tmp_arr);
				tmp_arr = NULL;
		}

		return ar_status;
}


static arStatus_t		__get_int_32_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_32_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		
		__CHECK_ARG(obj, field_name);

		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, sizeof(arr[0]) * arr_size);

		for(i = 0; i < arr_size; ++i)
		{
				arr[i] = AR_NTOL_32(arr[i]);
		}

END_POINT:
		
		return ar_status;
}






static arStatus_t		__put_uint_32_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_32_t v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		v = AR_LTON_U32(v);
		return __put_byte_t_array_to_dict(obj, field_name, (const ar_byte_t*)&v, sizeof(ar_uint_32_t));
}


static arStatus_t		__get_uint_32_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_32_t *pv)
{
		arStatus_t		ar_status;
		
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		
		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);
		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)pv, sizeof(ar_uint_32_t));

		if(ar_status != AR_S_YES)
		{
				return ar_status;
		}

		*pv = AR_NTOL_U32(*pv);
END_POINT:
		return ar_status;
}





static arStatus_t		__put_uint_32_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_32_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		ar_uint_32_t		*tmp_arr;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		tmp_arr = NULL;
		__CHECK_ARG(obj, field_name);

		tmp_arr = AR_NEWARR(ar_uint_32_t, arr_size);
		
		if(tmp_arr == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				tmp_arr[i] = AR_LTON_U32(arr[i]);
		}

		ar_status = __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);

END_POINT:

		if(tmp_arr)
		{
				AR_DEL(tmp_arr);
				tmp_arr = NULL;
		}
		
		return ar_status;
}


static arStatus_t		__get_uint_32_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_32_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		
		__CHECK_ARG(obj, field_name);

		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, sizeof(arr[0]) * arr_size);

		for(i = 0; i < arr_size; ++i)
		{
				arr[i] = AR_NTOL_U32(arr[i]);
		}

END_POINT:
		
		return ar_status;
}




/************************************************Integer 64*****************************************************/

static arStatus_t		__put_int_64_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_64_t v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		v = AR_LTON_64(v);
		return __put_byte_t_array_to_dict(obj, field_name, (const ar_byte_t*)&v, sizeof(ar_int_64_t));
}


static arStatus_t		__get_int_64_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_64_t *pv)
{
		arStatus_t		ar_status;
		
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		
		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);
		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)pv, sizeof(ar_int_64_t));

		if(ar_status != AR_S_YES)
		{
				return ar_status;
		}

		*pv = AR_NTOL_64(*pv);
END_POINT:
		return ar_status;
}





static arStatus_t		__put_int_64_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_int_64_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		ar_int_64_t		*tmp_arr;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		tmp_arr = NULL;
		__CHECK_ARG(obj, field_name);

		tmp_arr = AR_NEWARR(ar_int_64_t, arr_size);
		
		if(tmp_arr == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				tmp_arr[i] = AR_LTON_64(arr[i]);
		}

		ar_status = __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);

END_POINT:

		if(tmp_arr)
		{
				AR_DEL(tmp_arr);
				tmp_arr = NULL;
		}

		return ar_status;
}


static arStatus_t		__get_int_64_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_int_64_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		
		__CHECK_ARG(obj, field_name);

		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, sizeof(arr[0]) * arr_size);

		for(i = 0; i < arr_size; ++i)
		{
				arr[i] = AR_NTOL_64(arr[i]);
		}

END_POINT:
		
		return ar_status;
}






static arStatus_t		__put_uint_64_t_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_64_t v)
{
		AR_ASSERT(obj != NULL && field_name != NULL);
		v = AR_LTON_U64(v);
		return __put_byte_t_array_to_dict(obj, field_name, (const ar_byte_t*)&v, sizeof(ar_uint_64_t));
}


static arStatus_t		__get_uint_64_t_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_64_t *pv)
{
		arStatus_t		ar_status;
		
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		
		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);
		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)pv, sizeof(ar_uint_64_t));

		if(ar_status != AR_S_YES)
		{
				return ar_status;
		}

		*pv = AR_NTOL_U64(*pv);
END_POINT:
		return ar_status;
}





static arStatus_t		__put_uint_64_t_array_to_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_64_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		ar_uint_64_t		*tmp_arr;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		tmp_arr = NULL;
		__CHECK_ARG(obj, field_name);

		tmp_arr = AR_NEWARR(ar_uint_64_t, arr_size);
		
		if(tmp_arr == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				tmp_arr[i] = AR_LTON_U64(arr[i]);
		}

		ar_status = __put_byte_t_array_to_dict(obj, field_name, (ar_byte_t*)tmp_arr, sizeof(tmp_arr[0]) * arr_size);

END_POINT:

		if(tmp_arr)
		{
				AR_DEL(tmp_arr);
				tmp_arr = NULL;
		}
		
		return ar_status;
}


static arStatus_t		__get_uint_64_t_array_from_dict(snObject_t *obj, const wchar_t *field_name, ar_uint_64_t *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		
		ar_status = AR_S_YES;
		
		__CHECK_ARG(obj, field_name);

		
		ar_status = __get_byte_t_array_from_dict(obj, field_name, (ar_byte_t*)arr, sizeof(arr[0]) * arr_size);

		for(i = 0; i < arr_size; ++i)
		{
				arr[i] = AR_NTOL_U64(arr[i]);
		}

END_POINT:
		
		return ar_status;
}




/******************************************************float****************************************************************/



static arStatus_t		__put_float_to_dict(snObject_t *obj, const wchar_t *field_name, float v)
{
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL);
		
		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);

		

		ar_status = SN_InsertToDictObjectByWcsFloat(obj, field_name, (double)v);

END_POINT:

		return ar_status;
}

static arStatus_t		__get_float_from_dict(snObject_t *obj, const wchar_t *field_name, float *pv)
{
		arStatus_t		ar_status;
		snObject_t		*flt_obj;
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		
		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);

		flt_obj = SN_FindFromDictObjectByWcs(obj, field_name);

		if(flt_obj == NULL || SN_GetObjectType(flt_obj) != SN_FLOAT_T)
		{
				ar_status = AR_E_INVAL;
				goto END_POINT;
		}

		*pv = (float)SN_GetFloatObject(flt_obj);

END_POINT:
		return ar_status;
}



static arStatus_t		__put_float_array_to_dict(snObject_t *obj, const wchar_t *field_name, float *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*float_arr_obj;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL);
		AR_ASSERT(arr_size > 0);

		ar_status = AR_S_YES;
		float_arr_obj = NULL;
		
		__CHECK_ARG(obj, field_name);


		float_arr_obj = SN_CreateObject(SN_LIST_T);
		if(float_arr_obj == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				ar_status = SN_InsertToListObjectByFloat(float_arr_obj, (double)arr[i]);
				
				if(ar_status != AR_S_YES)
				{
						goto END_POINT;
				}
		}

		ar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, float_arr_obj);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:
		if(ar_status != AR_S_YES)
		{
				if(float_arr_obj != NULL)
				{
						SN_DestroyObject(float_arr_obj);
						float_arr_obj = NULL;
				}
		}

		return ar_status;

}

static arStatus_t		__get_float_array_from_dict(snObject_t *obj, const wchar_t *field_name, float *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*arr_list;
		size_t			i;
		size_t			list_cnt;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);

		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);

		arr_list = SN_FindFromDictObjectByWcs(obj, field_name);
		
		if(arr_list == NULL || SN_GetObjectType(arr_list) != SN_LIST_T)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		list_cnt = SN_GetListObjectCount(arr_list);

		if(list_cnt != arr_size)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		for(i = 0; i < SN_GetListObjectCount(arr_list); ++i)
		{
				snObject_t *flt_obj = SN_GetFromListObject(arr_list, i);
				if(flt_obj == NULL || SN_GetObjectType(flt_obj) != SN_FLOAT_T)
				{
						ar_status = AR_E_NOTMATCHED;
						goto END_POINT;
				}

				arr[i] = (float)SN_GetFloatObject(flt_obj);
		}
END_POINT:
		return ar_status;
}



/******************************************************double****************************************************************/



static arStatus_t		__put_double_to_dict(snObject_t *obj, const wchar_t *field_name, double v)
{
		arStatus_t		ar_status;
		AR_ASSERT(obj != NULL && field_name != NULL);

		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);

		ar_status = SN_InsertToDictObjectByWcsFloat(obj, field_name, v);

END_POINT:
		return ar_status;
}



static arStatus_t		__put_double_array_to_dict(snObject_t *obj, const wchar_t *field_name, double *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*float_arr_obj;
		size_t			i;
		AR_ASSERT(obj != NULL && field_name != NULL && arr_size > 0);
		

		ar_status = AR_S_YES;
		float_arr_obj = NULL;

		__CHECK_ARG(obj, field_name);

		float_arr_obj = SN_CreateObject(SN_LIST_T);
		if(float_arr_obj == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		for(i = 0; i < arr_size; ++i)
		{
				ar_status = SN_InsertToListObjectByFloat(float_arr_obj, arr[i]);
				
				if(ar_status != AR_S_YES)
				{
						goto END_POINT;
				}
		}

		ar_status = SN_InsertToDictObjectByWcsObject(obj, field_name, float_arr_obj);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:
		if(ar_status != AR_S_YES)
		{
				if(float_arr_obj != NULL)
				{
						SN_DestroyObject(float_arr_obj);
						float_arr_obj = NULL;
				}
		}

		return ar_status;

}







static arStatus_t		__get_double_from_dict(snObject_t *obj, const wchar_t *field_name, double *pv)
{
		arStatus_t		ar_status;
		snObject_t		*flt_obj;
		AR_ASSERT(obj != NULL && field_name != NULL && pv != NULL);
		
		ar_status = AR_S_YES;
		__CHECK_ARG(obj, field_name);

		flt_obj = SN_FindFromDictObjectByWcs(obj, field_name);

		
		if(flt_obj == NULL || SN_GetObjectType(flt_obj) != SN_FLOAT_T)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		*pv = SN_GetFloatObject(flt_obj);

END_POINT:
		return ar_status;
}



static arStatus_t		__get_double_array_from_dict(snObject_t *obj, const wchar_t *field_name, double *arr, size_t arr_size)
{
		arStatus_t		ar_status;
		snObject_t		*arr_list;
		size_t			i;
		size_t			list_cnt;
		AR_ASSERT(obj != NULL && field_name != NULL && arr != NULL && arr_size > 0);
		

		ar_status = AR_S_YES;

		__CHECK_ARG(obj, field_name);

		arr_list = SN_FindFromDictObjectByWcs(obj, field_name);
		
		if(SN_GetObjectType(arr_list) != SN_LIST_T)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		list_cnt = SN_GetListObjectCount(arr_list);

		if(list_cnt != arr_size)
		{
				ar_status = AR_E_NOTMATCHED;
				goto END_POINT;
		}

		for(i = 0; i < SN_GetListObjectCount(arr_list); ++i)
		{
				snObject_t *flt_obj = SN_GetFromListObject(arr_list, i);
				if(flt_obj == NULL || SN_GetObjectType(flt_obj) != SN_FLOAT_T)
				{
						ar_status = AR_E_NOTMATCHED;
						goto END_POINT;
				}

				arr[i] = SN_GetFloatObject(flt_obj);
		}
END_POINT:
		return ar_status;
}





































typedef struct {
		ar_uint_32_t		unused;
		ar_int_32_t		x[1024];
}daemonKeepalive_t;





typedef struct {
		ar_uint_32_t		t1;
		ar_uint_32_t		t2;
		wchar_t		t3[1024];
}anonymous_type_1;





typedef struct {
		anonymous_type_1		test;
		daemonKeepalive_t		kp;
		daemonKeepalive_t		kp_arr[1024];
}daemonTest_t;





typedef struct {
		ar_uint_32_t		unused;
		daemonTest_t		unused2;
}daemonQueryCPUTemp_t;





typedef struct {
		bool_t		b;
		bool_t		b_arr[128];
		char		c;
		char		c_arr[1024];
		wchar_t		w;
		wchar_t		w_arr[1024];
		ar_byte_t		bt;
		ar_byte_t		bt_arr[1024];
		ar_int_8_t		i8;
		ar_int_8_t		i8_arr[1024];
		ar_uint_8_t		u8;
		ar_uint_8_t		u8_arr[1024];
		ar_int_16_t		i16;
		ar_int_16_t		i16_arr[1024];
		ar_uint_16_t		u16;
		ar_uint_16_t		u16_arr[1024];
		ar_int_32_t		i32;
		ar_int_32_t		i32_arr[1024];
		ar_uint_32_t		u32;
		ar_uint_32_t		u32_arr[1024];
		ar_int_64_t		i64;
		ar_int_64_t		i64_arr[1024];
		ar_uint_64_t		u64;
		ar_uint_64_t		u64_arr[1024];
}basicTest_t;



































/******************************daemonKeepalive_t********************************/

static snObject_t*		__put_daemonKeepalive_t(daemonKeepalive_t *stu)
{
		snObject_t		*obj, *tmp;
		AR_ASSERT(stu != NULL);
		obj = NULL;
		tmp = NULL;

		obj = SN_CreateObject(SN_DICT_T);
		if(obj == NULL)
		{
				goto INVALID_POINT;
		}
/************************************************************************************************/
		if(__put_uint_32_t_to_dict(obj, L"unused", stu->unused) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



		if(__put_int_32_t_array_to_dict(obj, L"x", stu->x, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



/************************************************************************************************/
		return obj;
INVALID_POINT:
		if(obj)
		{
				SN_DestroyObject(obj);
				obj = NULL;
		}
		return NULL;
}


static snObject_t*		__put_daemonKeepalive_t_array(daemonKeepalive_t *stu, size_t arr_size)
{
		snObject_t		*stu_list;
		size_t i;
		AR_ASSERT(stu != NULL && arr_size > 0);

		stu_list = SN_CreateObject(SN_LIST_T);

		for(i = 0; i < arr_size; ++i)
		{

				/*************************************************************************/
				snObject_t *tmp = __put_daemonKeepalive_t(&stu[i]);
				if(tmp == NULL)
				{
						goto INVALID_POINT;
				}
				/*************************************************************************/


				if(SN_InsertToListObject(stu_list, tmp) != AR_S_YES)
				{
						SN_DestroyObject(tmp);
						tmp = NULL;
						goto INVALID_POINT;
				}
		}

		return stu_list;
INVALID_POINT:

		if(stu_list)
		{
				SN_DestroyObject(stu_list);
				stu_list = NULL;
		}

		return stu_list;
}



static bool_t	__get_daemonKeepalive_t(snObject_t *obj, daemonKeepalive_t *stu)
{
		bool_t	is_ok;
		snObject_t *tmp;
		AR_ASSERT(stu != NULL && obj != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		is_ok = true;

		/***************************************************************************/
		if(__get_uint_32_t_from_dict(obj, L"unused", &stu->unused) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_int_32_t_array_from_dict(obj, L"x", stu->x, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;
		}



		/***************************************************************************/

		return true;
INVALID_POINT:
		is_ok = false;
		return is_ok;
}


static bool_t	__get_daemonKeepalive_t_array(snObject_t *obj, daemonKeepalive_t *stu, size_t arr_size)
{
		bool_t	is_ok;
		size_t list_cnt;
		size_t i;
		AR_ASSERT(stu != NULL && obj != NULL && arr_size > 0);

		is_ok = true;

		if(SN_GetObjectType(obj) != SN_LIST_T)
		{
				is_ok = false;
				goto INVALID_POINT;
		}

		list_cnt = SN_GetListObjectCount(obj);

		if(list_cnt != arr_size)
		{
				goto INVALID_POINT;
		}


		for(i = 0; i < list_cnt; ++i)
		{
				snObject_t *item = SN_GetFromListObject(obj, i);
				if(item == NULL || SN_GetObjectType(item) != SN_DICT_T)
				{
						goto INVALID_POINT;
				}
				
				/********************************************************************/
				if(!__get_daemonKeepalive_t(item, &stu[i]))
				{
						is_ok = false;
						goto INVALID_POINT;
				}
				/********************************************************************/
		}
		return true;

INVALID_POINT:
		is_ok = false;
		return is_ok;
}








/******************************anonymous_type_1********************************/

static snObject_t*		__put_anonymous_type_1(anonymous_type_1 *stu)
{
		snObject_t		*obj, *tmp;
		AR_ASSERT(stu != NULL);
		obj = NULL;
		tmp = NULL;

		obj = SN_CreateObject(SN_DICT_T);
		if(obj == NULL)
		{
				goto INVALID_POINT;
		}
/************************************************************************************************/
		if(__put_uint_32_t_to_dict(obj, L"t1", stu->t1) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



		if(__put_uint_32_t_to_dict(obj, L"t2", stu->t2) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



		if(__put_wchar_t_array_to_dict(obj, L"t3", stu->t3) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}




/************************************************************************************************/
		return obj;
INVALID_POINT:
		if(obj)
		{
				SN_DestroyObject(obj);
				obj = NULL;
		}
		return NULL;
}


static snObject_t*		__put_anonymous_type_1_array(anonymous_type_1 *stu, size_t arr_size)
{
		snObject_t		*stu_list;
		size_t i;
		AR_ASSERT(stu != NULL && arr_size > 0);

		stu_list = SN_CreateObject(SN_LIST_T);

		for(i = 0; i < arr_size; ++i)
		{

				/*************************************************************************/
				snObject_t *tmp = __put_anonymous_type_1(&stu[i]);
				if(tmp == NULL)
				{
						goto INVALID_POINT;
				}
				/*************************************************************************/


				if(SN_InsertToListObject(stu_list, tmp) != AR_S_YES)
				{
						SN_DestroyObject(tmp);
						tmp = NULL;
						goto INVALID_POINT;
				}
		}

		return stu_list;
INVALID_POINT:

		if(stu_list)
		{
				SN_DestroyObject(stu_list);
				stu_list = NULL;
		}

		return stu_list;
}



static bool_t	__get_anonymous_type_1(snObject_t *obj, anonymous_type_1 *stu)
{
		bool_t	is_ok;
		snObject_t *tmp;
		AR_ASSERT(stu != NULL && obj != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		is_ok = true;

		/***************************************************************************/
		if(__get_uint_32_t_from_dict(obj, L"t1", &stu->t1) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_uint_32_t_from_dict(obj, L"t2", &stu->t2) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_wchar_t_array_from_dict(obj, L"t3", stu->t3, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;
		}



		/***************************************************************************/

		return true;
INVALID_POINT:
		is_ok = false;
		return is_ok;
}


static bool_t	__get_anonymous_type_1_array(snObject_t *obj, anonymous_type_1 *stu, size_t arr_size)
{
		bool_t	is_ok;
		size_t list_cnt;
		size_t i;
		AR_ASSERT(stu != NULL && obj != NULL && arr_size > 0);

		is_ok = true;

		if(SN_GetObjectType(obj) != SN_LIST_T)
		{
				is_ok = false;
				goto INVALID_POINT;
		}

		list_cnt = SN_GetListObjectCount(obj);

		if(list_cnt != arr_size)
		{
				goto INVALID_POINT;
		}


		for(i = 0; i < list_cnt; ++i)
		{
				snObject_t *item = SN_GetFromListObject(obj, i);
				if(item == NULL || SN_GetObjectType(item) != SN_DICT_T)
				{
						goto INVALID_POINT;
				}
				
				/********************************************************************/
				if(!__get_anonymous_type_1(item, &stu[i]))
				{
						is_ok = false;
						goto INVALID_POINT;
				}
				/********************************************************************/
		}
		return true;

INVALID_POINT:
		is_ok = false;
		return is_ok;
}








/******************************daemonTest_t********************************/

static snObject_t*		__put_daemonTest_t(daemonTest_t *stu)
{
		snObject_t		*obj, *tmp;
		AR_ASSERT(stu != NULL);
		obj = NULL;
		tmp = NULL;

		obj = SN_CreateObject(SN_DICT_T);
		if(obj == NULL)
		{
				goto INVALID_POINT;
		}
/************************************************************************************************/
		tmp = __put_anonymous_type_1(&stu->test);
		if(tmp == NULL)
		{
				goto INVALID_POINT;
		}


		if(SN_InsertToDictObjectByWcsObject(obj, L"test", tmp) != AR_S_YES)
		{
				goto INVALID_POINT;
		}



		tmp = __put_daemonKeepalive_t(&stu->kp);
		if(tmp == NULL)
		{
				goto INVALID_POINT;
		}


		if(SN_InsertToDictObjectByWcsObject(obj, L"kp", tmp) != AR_S_YES)
		{
				goto INVALID_POINT;
		}



		tmp = __put_daemonKeepalive_t_array(stu->kp_arr, 1024);
		if(tmp == NULL)
		{
				goto INVALID_POINT;
		}

		if(SN_InsertToDictObjectByWcsObject(obj, L"kp_arr", tmp) != AR_S_YES)
		{
				goto INVALID_POINT;
		}



/************************************************************************************************/
		return obj;
INVALID_POINT:
		if(obj)
		{
				SN_DestroyObject(obj);
				obj = NULL;
		}
		return NULL;
}


static snObject_t*		__put_daemonTest_t_array(daemonTest_t *stu, size_t arr_size)
{
		snObject_t		*stu_list;
		size_t i;
		AR_ASSERT(stu != NULL && arr_size > 0);

		stu_list = SN_CreateObject(SN_LIST_T);

		for(i = 0; i < arr_size; ++i)
		{

				/*************************************************************************/
				snObject_t *tmp = __put_daemonTest_t(&stu[i]);
				if(tmp == NULL)
				{
						goto INVALID_POINT;
				}
				/*************************************************************************/


				if(SN_InsertToListObject(stu_list, tmp) != AR_S_YES)
				{
						SN_DestroyObject(tmp);
						tmp = NULL;
						goto INVALID_POINT;
				}
		}

		return stu_list;
INVALID_POINT:

		if(stu_list)
		{
				SN_DestroyObject(stu_list);
				stu_list = NULL;
		}

		return stu_list;
}



static bool_t	__get_daemonTest_t(snObject_t *obj, daemonTest_t *stu)
{
		bool_t	is_ok;
		snObject_t *tmp;
		AR_ASSERT(stu != NULL && obj != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		is_ok = true;

		/***************************************************************************/
		tmp = SN_FindFromDictObjectByWcs(obj, L"test");
		if(tmp == NULL)
		{
				is_ok = false;
				goto INVALID_POINT;
		}

		if(!__get_anonymous_type_1(tmp, &stu->test))
		{
				is_ok = false;
				goto INVALID_POINT;
		}


		tmp = SN_FindFromDictObjectByWcs(obj, L"kp");
		if(tmp == NULL)
		{
				is_ok = false;
				goto INVALID_POINT;
		}

		if(!__get_daemonKeepalive_t(tmp, &stu->kp))
		{
				is_ok = false;
				goto INVALID_POINT;
		}


		tmp = SN_FindFromDictObjectByWcs(obj, L"kp_arr");
		if(tmp == NULL)
		{
				is_ok = false;
				goto INVALID_POINT;
		}

		if(!__get_daemonKeepalive_t_array(tmp, stu->kp_arr, 1024))
		{
				is_ok = false;
				goto INVALID_POINT;
		}



		/***************************************************************************/

		return true;
INVALID_POINT:
		is_ok = false;
		return is_ok;
}


static bool_t	__get_daemonTest_t_array(snObject_t *obj, daemonTest_t *stu, size_t arr_size)
{
		bool_t	is_ok;
		size_t list_cnt;
		size_t i;
		AR_ASSERT(stu != NULL && obj != NULL && arr_size > 0);

		is_ok = true;

		if(SN_GetObjectType(obj) != SN_LIST_T)
		{
				is_ok = false;
				goto INVALID_POINT;
		}

		list_cnt = SN_GetListObjectCount(obj);

		if(list_cnt != arr_size)
		{
				goto INVALID_POINT;
		}


		for(i = 0; i < list_cnt; ++i)
		{
				snObject_t *item = SN_GetFromListObject(obj, i);
				if(item == NULL || SN_GetObjectType(item) != SN_DICT_T)
				{
						goto INVALID_POINT;
				}
				
				/********************************************************************/
				if(!__get_daemonTest_t(item, &stu[i]))
				{
						is_ok = false;
						goto INVALID_POINT;
				}
				/********************************************************************/
		}
		return true;

INVALID_POINT:
		is_ok = false;
		return is_ok;
}








/******************************daemonQueryCPUTemp_t********************************/

static snObject_t*		__put_daemonQueryCPUTemp_t(daemonQueryCPUTemp_t *stu)
{
		snObject_t		*obj, *tmp;
		AR_ASSERT(stu != NULL);
		obj = NULL;
		tmp = NULL;

		obj = SN_CreateObject(SN_DICT_T);
		if(obj == NULL)
		{
				goto INVALID_POINT;
		}
/************************************************************************************************/
		if(__put_uint_32_t_to_dict(obj, L"unused", stu->unused) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



		tmp = __put_daemonTest_t(&stu->unused2);
		if(tmp == NULL)
		{
				goto INVALID_POINT;
		}


		if(SN_InsertToDictObjectByWcsObject(obj, L"unused2", tmp) != AR_S_YES)
		{
				goto INVALID_POINT;
		}




/************************************************************************************************/
		return obj;
INVALID_POINT:
		if(obj)
		{
				SN_DestroyObject(obj);
				obj = NULL;
		}
		return NULL;
}


static snObject_t*		__put_daemonQueryCPUTemp_t_array(daemonQueryCPUTemp_t *stu, size_t arr_size)
{
		snObject_t		*stu_list;
		size_t i;
		AR_ASSERT(stu != NULL && arr_size > 0);

		stu_list = SN_CreateObject(SN_LIST_T);

		for(i = 0; i < arr_size; ++i)
		{

				/*************************************************************************/
				snObject_t *tmp = __put_daemonQueryCPUTemp_t(&stu[i]);
				if(tmp == NULL)
				{
						goto INVALID_POINT;
				}
				/*************************************************************************/


				if(SN_InsertToListObject(stu_list, tmp) != AR_S_YES)
				{
						SN_DestroyObject(tmp);
						tmp = NULL;
						goto INVALID_POINT;
				}
		}

		return stu_list;
INVALID_POINT:

		if(stu_list)
		{
				SN_DestroyObject(stu_list);
				stu_list = NULL;
		}

		return stu_list;
}



static bool_t	__get_daemonQueryCPUTemp_t(snObject_t *obj, daemonQueryCPUTemp_t *stu)
{
		bool_t	is_ok;
		snObject_t *tmp;
		AR_ASSERT(stu != NULL && obj != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		is_ok = true;

		/***************************************************************************/
		if(__get_uint_32_t_from_dict(obj, L"unused", &stu->unused) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		tmp = SN_FindFromDictObjectByWcs(obj, L"unused2");
		if(tmp == NULL)
		{
				is_ok = false;
				goto INVALID_POINT;
		}

		if(!__get_daemonTest_t(tmp, &stu->unused2))
		{
				is_ok = false;
				goto INVALID_POINT;
		}



		/***************************************************************************/

		return true;
INVALID_POINT:
		is_ok = false;
		return is_ok;
}


static bool_t	__get_daemonQueryCPUTemp_t_array(snObject_t *obj, daemonQueryCPUTemp_t *stu, size_t arr_size)
{
		bool_t	is_ok;
		size_t list_cnt;
		size_t i;
		AR_ASSERT(stu != NULL && obj != NULL && arr_size > 0);

		is_ok = true;

		if(SN_GetObjectType(obj) != SN_LIST_T)
		{
				is_ok = false;
				goto INVALID_POINT;
		}

		list_cnt = SN_GetListObjectCount(obj);

		if(list_cnt != arr_size)
		{
				goto INVALID_POINT;
		}


		for(i = 0; i < list_cnt; ++i)
		{
				snObject_t *item = SN_GetFromListObject(obj, i);
				if(item == NULL || SN_GetObjectType(item) != SN_DICT_T)
				{
						goto INVALID_POINT;
				}
				
				/********************************************************************/
				if(!__get_daemonQueryCPUTemp_t(item, &stu[i]))
				{
						is_ok = false;
						goto INVALID_POINT;
				}
				/********************************************************************/
		}
		return true;

INVALID_POINT:
		is_ok = false;
		return is_ok;
}








/******************************basicTest_t********************************/

static snObject_t*		__put_basicTest_t(basicTest_t *stu)
{
		snObject_t		*obj, *tmp;
		AR_ASSERT(stu != NULL);
		obj = NULL;
		tmp = NULL;

		obj = SN_CreateObject(SN_DICT_T);
		if(obj == NULL)
		{
				goto INVALID_POINT;
		}
/************************************************************************************************/
		if(__put_bool_t_to_dict(obj, L"b", stu->b) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



		if(__put_bool_t_array_to_dict(obj, L"b_arr", stu->b_arr, 128) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}


		if(__put_char_to_dict(obj, L"c", stu->c) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



		if(__put_char_array_to_dict(obj, L"c_arr", stu->c_arr) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



		if(__put_wchar_t_to_dict(obj, L"w", stu->w) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



		if(__put_wchar_t_array_to_dict(obj, L"w_arr", stu->w_arr) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



		if(__put_byte_t_to_dict(obj, L"bt", stu->bt) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



		if(__put_byte_t_array_to_dict(obj, L"bt_arr", stu->bt_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}


		if(__put_int_8_t_to_dict(obj, L"i8", stu->i8) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



		if(__put_int_8_t_array_to_dict(obj, L"i8_arr", stu->i8_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}


		if(__put_uint_8_t_to_dict(obj, L"u8", stu->u8) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



		if(__put_uint_8_t_array_to_dict(obj, L"u8_arr", stu->u8_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}


		if(__put_int_16_t_to_dict(obj, L"i16", stu->i16) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



		if(__put_int_16_t_array_to_dict(obj, L"i16_arr", stu->i16_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}


		if(__put_uint_16_t_to_dict(obj, L"u16", stu->u16) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



		if(__put_uint_16_t_array_to_dict(obj, L"u16_arr", stu->u16_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}


		if(__put_int_32_t_to_dict(obj, L"i32", stu->i32) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



		if(__put_int_32_t_array_to_dict(obj, L"i32_arr", stu->i32_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}


		if(__put_uint_32_t_to_dict(obj, L"u32", stu->u32) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



		if(__put_uint_32_t_array_to_dict(obj, L"u32_arr", stu->u32_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}


		if(__put_int_64_t_to_dict(obj, L"i64", stu->i64) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



		if(__put_int_64_t_array_to_dict(obj, L"i64_arr", stu->i64_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}


		if(__put_uint_64_t_to_dict(obj, L"u64", stu->u64) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



		if(__put_uint_64_t_array_to_dict(obj, L"u64_arr", stu->u64_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;		
		}



/************************************************************************************************/
		return obj;
INVALID_POINT:
		if(obj)
		{
				SN_DestroyObject(obj);
				obj = NULL;
		}
		return NULL;
}


static snObject_t*		__put_basicTest_t_array(basicTest_t *stu, size_t arr_size)
{
		snObject_t		*stu_list;
		size_t i;
		AR_ASSERT(stu != NULL && arr_size > 0);

		stu_list = SN_CreateObject(SN_LIST_T);

		for(i = 0; i < arr_size; ++i)
		{

				/*************************************************************************/
				snObject_t *tmp = __put_basicTest_t(&stu[i]);
				if(tmp == NULL)
				{
						goto INVALID_POINT;
				}
				/*************************************************************************/


				if(SN_InsertToListObject(stu_list, tmp) != AR_S_YES)
				{
						SN_DestroyObject(tmp);
						tmp = NULL;
						goto INVALID_POINT;
				}
		}

		return stu_list;
INVALID_POINT:

		if(stu_list)
		{
				SN_DestroyObject(stu_list);
				stu_list = NULL;
		}

		return stu_list;
}



static bool_t	__get_basicTest_t(snObject_t *obj, basicTest_t *stu)
{
		bool_t	is_ok;
		snObject_t *tmp;
		AR_ASSERT(stu != NULL && obj != NULL);
		AR_ASSERT(SN_GetObjectType(obj) == SN_DICT_T);
		is_ok = true;

		/***************************************************************************/
		if(__get_bool_t_from_dict(obj, L"b", &stu->b) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_bool_t_array_from_dict(obj, L"b_arr", stu->b_arr, 128) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_char_from_dict(obj, L"c", &stu->c) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_char_array_from_dict(obj, L"c_arr", stu->c_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_wchar_t_from_dict(obj, L"w", &stu->w) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_wchar_t_array_from_dict(obj, L"w_arr", stu->w_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_byte_t_from_dict(obj, L"bt", &stu->bt) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_byte_t_array_from_dict(obj, L"bt_arr", stu->bt_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_int_8_t_from_dict(obj, L"i8", &stu->i8) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_int_8_t_array_from_dict(obj, L"i8_arr", stu->i8_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_uint_8_t_from_dict(obj, L"u8", &stu->u8) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_uint_8_t_array_from_dict(obj, L"u8_arr", stu->u8_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_int_16_t_from_dict(obj, L"i16", &stu->i16) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_int_16_t_array_from_dict(obj, L"i16_arr", stu->i16_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_uint_16_t_from_dict(obj, L"u16", &stu->u16) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_uint_16_t_array_from_dict(obj, L"u16_arr", stu->u16_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_int_32_t_from_dict(obj, L"i32", &stu->i32) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_int_32_t_array_from_dict(obj, L"i32_arr", stu->i32_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_uint_32_t_from_dict(obj, L"u32", &stu->u32) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_uint_32_t_array_from_dict(obj, L"u32_arr", stu->u32_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_int_64_t_from_dict(obj, L"i64", &stu->i64) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_int_64_t_array_from_dict(obj, L"i64_arr", stu->i64_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_uint_64_t_from_dict(obj, L"u64", &stu->u64) != AR_S_YES)
		{
				goto INVALID_POINT;
		}


		if(__get_uint_64_t_array_from_dict(obj, L"u64_arr", stu->u64_arr, 1024) != AR_S_YES)
		{
				goto INVALID_POINT;
		}



		/***************************************************************************/

		return true;
INVALID_POINT:
		is_ok = false;
		return is_ok;
}


static bool_t	__get_basicTest_t_array(snObject_t *obj, basicTest_t *stu, size_t arr_size)
{
		bool_t	is_ok;
		size_t list_cnt;
		size_t i;
		AR_ASSERT(stu != NULL && obj != NULL && arr_size > 0);

		is_ok = true;

		if(SN_GetObjectType(obj) != SN_LIST_T)
		{
				is_ok = false;
				goto INVALID_POINT;
		}

		list_cnt = SN_GetListObjectCount(obj);

		if(list_cnt != arr_size)
		{
				goto INVALID_POINT;
		}


		for(i = 0; i < list_cnt; ++i)
		{
				snObject_t *item = SN_GetFromListObject(obj, i);
				if(item == NULL || SN_GetObjectType(item) != SN_DICT_T)
				{
						goto INVALID_POINT;
				}
				
				/********************************************************************/
				if(!__get_basicTest_t(item, &stu[i]))
				{
						is_ok = false;
						goto INVALID_POINT;
				}
				/********************************************************************/
		}
		return true;

INVALID_POINT:
		is_ok = false;
		return is_ok;
}





enum {

		DAEMONKEEPALIVE_T,
		DAEMONTEST_T,
		DAEMONQUERYCPUTEMP_T,
		BASICTEST_T,
};


typedef struct {
		int		type;
		daemonKeepalive_t		daemonKeepalive_t;
		daemonTest_t		daemonTest_t;
		daemonQueryCPUTemp_t		daemonQueryCPUTemp_t;
		basicTest_t		basicTest_t;
}clisrvMessage_t;


static arStatus_t		clisrvMessage_t_Marshal(clisrvMessage_t*uni_type, arBuffer_t *out)
{
		arStatus_t		ar_status;
		snObject_t		*key, *val;
		snObject_t		*final_obj;
		AR_ASSERT(uni_type != NULL && out != NULL);
		ar_status = AR_S_YES;
		key = NULL;
		val	= NULL;
		final_obj = NULL;
		
		key = SN_CreateObject(SN_INT_T);
		if(key == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		SN_SetIntObject(key, uni_type->type);

		switch(uni_type->type)
		{
/**********************************************/
		case DAEMONKEEPALIVE_T :
				val = __put_daemonKeepalive_t(&uni_type->daemonKeepalive_t);
				break;
		case DAEMONTEST_T :
				val = __put_daemonTest_t(&uni_type->daemonTest_t);
				break;
		case DAEMONQUERYCPUTEMP_T :
				val = __put_daemonQueryCPUTemp_t(&uni_type->daemonQueryCPUTemp_t);
				break;
		case BASICTEST_T :
				val = __put_basicTest_t(&uni_type->basicTest_t);
				break;
/**********************************************/
		default:
				ar_status = AR_E_INVAL;
				goto END_POINT;
		}
		

		if(val == NULL)
		{
				ar_status = AR_E_INVAL;
				goto END_POINT;
		}


		final_obj = SN_CreateObject(SN_LIST_T);

		if(final_obj == NULL)
		{
				ar_status = AR_E_NOMEM;
				goto END_POINT;
		}

		ar_status = SN_InsertToListObject(final_obj, key);
		
		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

		key = NULL;

		ar_status = SN_InsertToListObject(final_obj, val);
		
		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

		val = NULL;

		ar_status = SN_PutObject(out, final_obj);

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

END_POINT:

		if(key)
		{
				SN_DestroyObject(key);
				key = NULL;
		}
		
		if(val)
		{
				SN_DestroyObject(val);
				val = NULL;
		}

		if(final_obj)
		{
				SN_DestroyObject(final_obj);
				final_obj = NULL;
		}

		return ar_status;
}


static arStatus_t		clisrvMessage_t_UnMarshal(clisrvMessage_t*uni_type, arBuffer_t *in)
{
		snRetVal_t		sn_ret;
		arStatus_t		ar_status;
		snObject_t		*type_obj, *data_obj;
		AR_ASSERT(uni_type != NULL && in != NULL);

		ar_status = AR_S_YES;
		type_obj = NULL;
		data_obj = NULL;
		AR_memset(uni_type, 0, sizeof(*uni_type));

		
		sn_ret = SN_GetObject(in);

		ar_status = sn_ret.status;

		if(ar_status != AR_S_YES)
		{
				goto END_POINT;
		}

		if(SN_GetObjectType(sn_ret.obj) != SN_LIST_T || SN_GetListObjectCount(sn_ret.obj) != 2)
		{
				ar_status = AR_E_INVAL;
				goto END_POINT;
		}
		
		type_obj = SN_GetFromListObject(sn_ret.obj, 0);

		if(type_obj == NULL || SN_GetObjectType(type_obj) != SN_INT_T)
		{
				ar_status = AR_E_INVAL;
				goto END_POINT;
		}

		data_obj = SN_GetFromListObject(sn_ret.obj, 1);

		if(data_obj == NULL || SN_GetObjectType(data_obj) != SN_DICT_T)
		{
				ar_status = AR_E_INVAL;
				goto END_POINT;
		}
		uni_type->type = SN_GetIntObject(type_obj);
		switch(uni_type->type)
		{
		/*******************************************************************************************/
		case DAEMONKEEPALIVE_T :
				if(!__get_daemonKeepalive_t(data_obj, &uni_type->daemonKeepalive_t))
				{
								ar_status = AR_E_INVAL;
								goto END_POINT;
				};
				break;
		case DAEMONTEST_T :
				if(!__get_daemonTest_t(data_obj, &uni_type->daemonTest_t))
				{
								ar_status = AR_E_INVAL;
								goto END_POINT;
				};
				break;
		case DAEMONQUERYCPUTEMP_T :
				if(!__get_daemonQueryCPUTemp_t(data_obj, &uni_type->daemonQueryCPUTemp_t))
				{
								ar_status = AR_E_INVAL;
								goto END_POINT;
				};
				break;
		case BASICTEST_T :
				if(!__get_basicTest_t(data_obj, &uni_type->basicTest_t))
				{
								ar_status = AR_E_INVAL;
								goto END_POINT;
				};
				break;
/*******************************************************************************************/
		default:
				ar_status = AR_E_INVAL;
				goto END_POINT;
				break;
		}


END_POINT:
		if(sn_ret.obj)
		{
				SN_DestroyObject(sn_ret.obj);
				sn_ret.obj = NULL;
		}

		return ar_status;
}

