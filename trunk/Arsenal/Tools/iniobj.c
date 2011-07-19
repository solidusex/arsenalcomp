#include "iniobj.h"



AR_NAMESPACE_BEGIN



typedef enum
{
		INI_INVALID,
		INI_EMPTY,
		INI_SECT,
		INI_KEY_VAL,
		INI_COMMENT,
}arIniLineType_t;

#define AR_MAX_LINE_LENGTH		1024


static const wchar_t __g_comment[] = L"#;";



static arIniLineType_t __parse_line(const wchar_t *line, wchar_t key[AR_MAX_LINE_LENGTH], wchar_t val[AR_MAX_LINE_LENGTH], wchar_t comment[AR_MAX_LINE_LENGTH])
{
		
		enum
		{
				start,
				after_comment_tag,
				after_sect_tag_l,
				after_sect_name,
				after_sect_name_ws,
				try_get_comment,
				after_key_name,
				after_key_name_ws,
				after_equal,
				after_value_name,
				after_value_name_ws,
		}stat;

		const wchar_t *p;
		const wchar_t *b, *e;
		size_t line_len;
		arIniLineType_t last_type;
		AR_ASSERT(line != NULL && key != NULL && val != NULL && comment != NULL);

		line_len = AR_wcslen(line);
		if(line_len >= AR_MAX_LINE_LENGTH)
		{
				return INI_INVALID;
		}else if(line_len == 0)
		{
				return INI_EMPTY;
		}

		stat = start;
		last_type = INI_COMMENT;
		p = line;
		b = NULL;
		e = NULL;

		key[0] = L'\0';
		val[0] = L'\0';
		comment[0] = L'\0';


		for(p = line; p <= line + line_len; ++p)
		{
				switch(stat)
				{
				case start:
				{
						if(*p == L'\0')
						{
								return INI_EMPTY;
						}else if(AR_iswspace(*p))
						{

						}else if(*p == L'[')
						{
								stat = after_sect_tag_l;
						}else if(AR_wcschr(__g_comment, *p) != NULL)
						{
								stat = after_comment_tag;
								b = p;
								e = b;
						}else
						{
								stat = after_key_name;
								b = p;
								e = b;
						}
				}
						break;
				case try_get_comment:
				{
						if(*p == L'\0')
						{
								return last_type;
						}else if(AR_wcschr(__g_comment, *p) != NULL)
						{
								stat = after_comment_tag;
								b = p;
								e = b;
						}else if(*p == L'\0')
						{
								return last_type;
						}else
						{

						}
										
				}
						break;
				case after_comment_tag:
				{
						e = p;
						if(*p == L'\0')
						{
								size_t l = e - b;
								AR_wcsncpy(comment, b, l);
								comment[l] = L'\0';
								return last_type;
						}
				}
						break;
				case after_sect_tag_l:
				{
						if(*p == L'\0')
						{
								return INI_INVALID;
						}else if(AR_iswspace(*p))
						{

						}else if(*p == L'[' || *p == L']')
						{
								return INI_INVALID;
						}else
						{
								b = p;
								e = b;
								stat = after_sect_name;
						}
				}
						break;
				case after_sect_name:
				{
						if(*p == L'\0')
						{
								return INI_INVALID;

						}else if(*p == L']')
						{
								size_t l = e - b + 1;
								AR_wcsncpy(key, b, l);
								key[l] = L'\0';
								last_type = INI_SECT;
								stat = try_get_comment;
						}else if(*p == L'[')
						{
								return INI_INVALID;

						}else if(AR_iswspace(*p))
						{
								stat = after_sect_name_ws;
						}else
						{
								e = p;
						}
				}
						break;
				case after_sect_name_ws:
				{
						if(*p == L'\0')
						{
								return INI_INVALID;
						
						}else if(*p == L']')
						{
								size_t l = e - b + 1;
								AR_wcsncpy(key, b, l);
								key[l] = L'\0';
								last_type = INI_SECT;
								stat = try_get_comment;
						}else if(*p == L'[')
						{
								return INI_INVALID;

						}else if(AR_iswspace(*p))
						{

						}else
						{
								e = p;
								stat = after_sect_name;
						}
				}
						break;

				case after_key_name:
				{
						if(*p == L'\0')
						{
								return INI_INVALID;
						}else if(*p == L'=')
						{
								size_t	l = e - b + 1;
								AR_wcsncpy(key, b, l);
								key[l] = L'\0';
								stat = after_equal;
						}else if(AR_iswspace(*p))
						{
								stat = after_key_name_ws;
						}else
						{
								e = p;
						}
				}
						break;
				case after_key_name_ws:
				{
						if(*p == L'\0')
						{
								return INI_INVALID;
						}else if(*p == L'=')
						{
								size_t	l = e - b + 1;
								AR_wcsncpy(key, b, l);
								key[l] = L'\0';
								stat = after_equal;
						}else if(AR_iswspace(*p))
						{

						}else
						{
								e = p;
						}
				}
						break;
				case after_equal:
				{
						if(*p == L'\0')
						{
								return INI_KEY_VAL;

						}else if(AR_iswspace(*p))
						{

						}else if(AR_wcschr(__g_comment, *p) != NULL)
						{
								last_type = INI_KEY_VAL;
								stat = try_get_comment;
								--p; 
						}else
						{
								b = p;
								e = b;
								stat = after_value_name;
						}
				}
						break;
				case  after_value_name:
				{
						if(*p == L'\0')
						{
								size_t	l = e - b + 1;
								AR_wcsncpy(val, b, l);
								val[l] = L'\0';
								return INI_KEY_VAL;
						}else if(AR_iswspace(*p))
						{
								stat = after_value_name_ws;
						}else if(AR_wcschr(__g_comment, *p) != NULL)
						{
								size_t	l = e - b + 1;
								AR_wcsncpy(val, b, l);
								val[l] = L'\0';
								last_type = INI_KEY_VAL;
								stat = try_get_comment;
								--p;
						}else
						{
								e = p;
						}
				}
						break;
				case  after_value_name_ws:
				{
						if(*p == L'\0')
						{
								size_t	l = e - b + 1;
								AR_wcsncpy(val, b, l);
								val[l] = L'\0';
								return INI_KEY_VAL;
						}else if(AR_iswspace(*p))
						{

						}else if(AR_wcschr(__g_comment, *p) != NULL)
						{
								size_t	l = e - b + 1;
								AR_wcsncpy(val, b, l);
								val[l] = L'\0';
								last_type = INI_KEY_VAL;
								stat = try_get_comment;
								--p;
						}else
						{
								stat = after_value_name;
						}
				}
						break;
				default:
						AR_ASSERT(false);
						break;
				}
		}
		return INI_INVALID;
}





struct __ini_node_tag;
typedef struct __ini_node_tag	iniNode_t;

typedef struct __ini_keyval_tag	
{
		wchar_t			*key;
		wchar_t			*val;
		wchar_t			*comment;
		bool_t			is_comment;
}iniKeyVal_t;

static iniKeyVal_t*		__ini_create_kvpair(const wchar_t *key, const wchar_t *val, const wchar_t *comment)
{
		iniKeyVal_t		*kv;
		
		kv = AR_NEW0(iniKeyVal_t);
		
		kv->comment = comment != NULL ? AR_wcsdup(comment) : NULL;
		kv->key = key != NULL ? AR_wcsdup(key) : NULL;
		kv->val = val != NULL ? AR_wcsdup(val) : NULL;
		return kv;
}

static void		__ini_destroy_kvpair(iniKeyVal_t *kv)
{
		AR_ASSERT(kv != NULL);

		if(kv->key)
		{
				AR_DEL(kv->key);
				kv->key = NULL;
		}

		if(kv->val)
		{
				AR_DEL(kv->val);
				kv->val = NULL;
		}

		if(kv->comment)
		{
				AR_DEL(kv->comment);
				kv->comment = NULL;
		}

		AR_DEL(kv);
		kv = NULL;
}

static void	__ini_reset_kvpair_value(iniKeyVal_t *kv, const wchar_t *val)
{
		AR_ASSERT(kv != NULL);

		if(kv->val)
		{
				AR_DEL(kv->val);
				kv->val = NULL;
		}
		
		kv->val = val != NULL ? AR_wcsdup(val) : NULL;
}

static void __ini_reset_kvpair_comment(iniKeyVal_t *kv, const wchar_t *comment)
{
		AR_ASSERT(kv != NULL);

		if(kv->comment)
		{
				AR_DEL(kv->comment);
				kv->comment = NULL;
		}
		
		kv->comment = comment != NULL ? AR_wcsdup(comment) : NULL;
}


typedef struct __ini_section_tag
{

		wchar_t			*section_name;
		wchar_t			*comment;
		iniKeyVal_t		**kv_pairs;
		size_t			cnt;
		size_t			cap;
}iniSection_t;


static iniSection_t*	__ini_create_section(const wchar_t *name, const wchar_t *comment)
{
		iniSection_t	*sec;
		AR_ASSERT(name != NULL);
		sec = AR_NEW0(iniSection_t);
		sec->section_name = AR_wcsdup(name);
		if(comment)
		{
				sec->comment = AR_wcsdup(comment);
		}
		return sec;
}

static void	__ini_clear_section(iniSection_t		*sec)
{
		size_t i;
		AR_ASSERT(sec != NULL);

		for(i = 0; i < sec->cnt; ++i)
		{
				__ini_destroy_kvpair(sec->kv_pairs[i]);
				sec->kv_pairs[i] = NULL;
		}

		sec->cnt = 0;

		if(sec->comment)
		{
				AR_DEL(sec->comment);
				sec->comment = NULL;
		}

		if(sec->section_name)
		{
				AR_DEL(sec->section_name);
				sec->section_name = NULL;
		}
}

static void	__ini_destroy_section(iniSection_t		*sec)
{
		AR_ASSERT(sec != NULL);
		
		__ini_clear_section(sec);

		if(sec->kv_pairs)
		{
				AR_DEL(sec->kv_pairs);
				sec->kv_pairs = NULL;
		}
		AR_DEL(sec);
		sec = NULL;
}


static iniKeyVal_t*	__ini_find_from_section(iniSection_t *sec, const wchar_t *key)
{
		size_t i;
		iniKeyVal_t		*kv;
		AR_ASSERT(sec != NULL && key != NULL);

		for(i = 0; i < sec->cnt; ++i)
		{
				kv = sec->kv_pairs[i];
				if(kv->key && AR_wcscmp(kv->key, key) == 0)
				{
						return kv;
				}
		}

		return NULL;
}

/*
插入：
key = 333 ; 键位设置
此类项，因此key不可为NULL
*/
static iniKeyVal_t* __ini_insert_kvpair_to_section(iniSection_t *sec, const wchar_t *key, const wchar_t *val, const wchar_t *comment)
{
		iniKeyVal_t		*kv;
		AR_ASSERT(sec != NULL && key != NULL);
		kv = __ini_find_from_section(sec, key);

		if(kv)
		{
				__ini_reset_kvpair_value(kv, val);
				__ini_reset_kvpair_comment(kv, comment);
		}else
		{
				kv = __ini_create_kvpair(key, val, comment);
				if(sec->cnt == sec->cap)
				{
						sec->cap = (sec->cap + 4)*2;
						sec->kv_pairs = AR_REALLOC(iniKeyVal_t*, sec->kv_pairs, sec->cap);
				}

				sec->kv_pairs[sec->cnt] = kv;
				sec->cnt++;
		}
		return kv;
}

static bool_t	__ini_remove_kvpair_from_section(iniSection_t *sec, const wchar_t *key)
{
		iniKeyVal_t *kv;
		size_t i;
		AR_ASSERT(sec != NULL && key != NULL);

		for(i = 0; i < sec->cnt; ++i)
		{
				kv = sec->kv_pairs[i];
				if(kv->key && AR_wcscmp(kv->key, key) == 0)
				{
						break;
				}
		}

		if(i == sec->cnt)
		{
				return false;
		}

		__ini_destroy_kvpair(kv);
		kv = NULL;

		while(i < sec->cnt - 1)
		{
				sec->kv_pairs[i] = sec->kv_pairs[i + 1];
				i++;
		}
		sec->cnt--;
		return true;
}

static void	__ini_insert_comment_to_section(iniSection_t *sec, const wchar_t *comment)
{
		iniKeyVal_t		*kv;
		AR_ASSERT(sec != NULL);
		
		kv = __ini_create_kvpair(NULL, NULL, comment);
		kv->is_comment = true;
		if(sec->cnt == sec->cap)
		{
				sec->cap = (sec->cap + 4)*2;
				sec->kv_pairs = AR_REALLOC(iniKeyVal_t*, sec->kv_pairs, sec->cap);
		}

		sec->kv_pairs[sec->cnt] = kv;
		sec->cnt++;
}



struct __ini_object_tag
{
		iniSection_t	**sect;
		size_t			cnt;
		size_t			cap;
};


iniObject_t*	Ini_CreateObject()
{
		return AR_NEW0(iniObject_t);
}

void			Ini_DestroyObject(iniObject_t *obj)
{
		AR_ASSERT(obj != NULL);
		Ini_ClearObject(obj);

		if(obj->sect)
		{
				AR_DEL(obj->sect);
				obj->sect = NULL;
		}

		AR_DEL(obj);

}

void			Ini_ClearObject(iniObject_t *obj)
{
		size_t i;
		AR_ASSERT(obj != NULL);
		
		for(i = 0; i < obj->cnt; ++i)
		{
				__ini_destroy_section(obj->sect[i]);
				obj->sect[i] = NULL;
		}

		obj->cnt = 0;
}


static int_t	__find_section(const iniObject_t *obj, const wchar_t *sect)
{
		size_t i;
		AR_ASSERT(obj != NULL && sect != NULL);

		for(i = 0; i < obj->cnt; ++i)
		{
				if(AR_wcscmp(obj->sect[i]->section_name, sect) == 0)
				{
						return (int_t)i;
				}
		}

		return -1;
}

static iniKeyVal_t*		__find_keyval(iniObject_t *obj, const wchar_t *sect, const wchar_t *key)
{
		iniSection_t *section;
		iniKeyVal_t  *kv;
		int_t idx;
		AR_ASSERT(obj != NULL && sect != NULL && key != NULL);

		idx = __find_section(obj, sect);
		if(idx == -1)
		{
				return NULL;
		}
		
		section = obj->sect[idx];
		
		kv = __ini_find_from_section(section, key);

		return kv;
}


bool_t			Ini_SectionIsExisted(const iniObject_t *obj, const wchar_t *sect)
{
		AR_ASSERT(obj != NULL && sect != NULL);

		return __find_section(obj, sect) != -1 ? true : false;

}

bool_t			Ini_InsertSection(iniObject_t *obj, const wchar_t *sect, const wchar_t *comment)
{

		AR_ASSERT(obj != NULL && sect != NULL);

		if(Ini_SectionIsExisted(obj, sect))
		{
				return false;
		}

		if(obj->cnt == obj->cap)
		{
				obj->cap = (obj->cap + 4) * 2;
				obj->sect = AR_REALLOC(iniSection_t*, obj->sect, obj->cap);
		}
		
		obj->sect[obj->cnt] = __ini_create_section(sect, comment);
		obj->cnt++;
		return true;

}

bool_t			Ini_RemoveSection(iniObject_t *obj, const wchar_t *sect)
{
		int_t idx;
		iniSection_t *section;
		AR_ASSERT(obj != NULL && sect != NULL);
		idx = __find_section(obj, sect);

		if(idx == -1)
		{
				return false;
		}

		section = obj->sect[idx];
		obj->sect[idx] = NULL;

		while((size_t)idx < obj->cnt - 1)
		{
				obj->sect[idx] = obj->sect[idx + 1];
				idx++;
		}
		
		__ini_destroy_section(section);
		return true;
}



const wchar_t*	Ini_GetComment(const iniObject_t *obj, const wchar_t *sect, const wchar_t *key)
{
		iniKeyVal_t		*kv;
		AR_ASSERT(obj != NULL && sect != NULL);
		
		kv = __find_keyval((iniObject_t*)obj, sect, key);
		return kv == NULL ? NULL : kv->comment;
}


const wchar_t*	Ini_GetString(const iniObject_t *obj, const wchar_t *sect, const wchar_t *key)
{
		iniKeyVal_t *kv;
		AR_ASSERT(obj != NULL && sect != NULL && key != NULL);
		
		kv = __find_keyval((iniObject_t*)obj, sect, key);

		if(kv == NULL)
		{
				return NULL;
		}else
		{
				return kv->val;
		}
}



void			Ini_SetString(iniObject_t *obj, const wchar_t *sect, const wchar_t *key, const wchar_t *val, const wchar_t *comment)
{
		int_t idx;
		AR_ASSERT(obj != NULL && sect != NULL && key != NULL);

RECHECK_POINT:
		idx = __find_section(obj, sect);
		
		if(idx == -1)
		{
				Ini_InsertSection(obj, sect, NULL);
				goto RECHECK_POINT;
		}else
		{
				__ini_insert_kvpair_to_section(obj->sect[idx], key, val, comment);
		}

}



bool_t			Ini_LoadObjectFromString(iniObject_t *obj, const wchar_t *ini_data)
{
		const wchar_t	*s;
		arString_t		*line;
		bool_t is_ok;
		int_t	last_sect_idx;
		AR_ASSERT(obj != NULL && ini_data != NULL);
		
		Ini_ClearObject(obj);
		line = AR_CreateString();
		s = ini_data;
		is_ok = true;
		last_sect_idx = -1;

		while(*s && is_ok)
		{
				if(*s != L'\r' && *s != L'\n')
				{
						AR_AppendCharToString(line, *s);
				}else
				{
						arIniLineType_t ret;
						wchar_t key[AR_MAX_LINE_LENGTH], val[AR_MAX_LINE_LENGTH], comment[AR_MAX_LINE_LENGTH];

						ret = __parse_line(AR_GetStrString(line), key,val, comment);
						
						switch(ret)
						{
						case INI_INVALID:
								is_ok = false;
								break;
						case INI_EMPTY:
								break;
						case INI_SECT:
								Ini_InsertSection(obj, key, AR_wcslen(comment) > 0 ? comment : NULL);
								last_sect_idx = (int_t)obj->cnt - 1;
								break;
						case INI_KEY_VAL:
								if(last_sect_idx == -1)
								{
										Ini_InsertSection(obj, INI_EMPTY_SECTION_NAME, NULL);
										last_sect_idx = (int_t)obj->cnt - 1;
								}
								__ini_insert_kvpair_to_section(obj->sect[last_sect_idx], key, AR_wcslen(val) > 0 ? val : NULL, AR_wcslen(comment) > 0 ? comment : NULL);
								break;
						case INI_COMMENT:
								if(last_sect_idx == -1)
								{
										Ini_InsertSection(obj, INI_EMPTY_SECTION_NAME, NULL);
										last_sect_idx = (int_t)obj->cnt - 1;
								}
								__ini_insert_comment_to_section(obj->sect[last_sect_idx], comment);
								break;
						default:
								break;
						}
						AR_ClearString(line);
				}
				++s;
		}
		
		AR_DestroyString(line);
		line = NULL;
		return is_ok;
}

void			Ini_SaveObjectToString(const iniObject_t *obj, arString_t *out)
{
		size_t i,k;
		AR_ASSERT(obj != NULL && out != NULL);
		AR_ClearString(out);

		for(i = 0; i < obj->cnt; ++i)
		{
				const iniSection_t *sect = obj->sect[i];

				if(AR_wcscmp(sect->section_name, INI_EMPTY_SECTION_NAME) != 0)
				{
						if(sect->comment)
						{
								AR_AppendFormatString(out, L"[%ls] %ls\r\n", sect->section_name, sect->comment);
						}else
						{
								AR_AppendFormatString(out, L"[%ls]\r\n", sect->section_name);
						}
				}

				for(k = 0; k < sect->cnt; ++k)
				{
						const iniKeyVal_t *kv = sect->kv_pairs[k];
						const wchar_t *key = kv->key;
						const wchar_t *val = kv->val == NULL ? L"" : kv->val;
						const wchar_t *comment = kv->comment == NULL ? L"" : kv->comment;
						
						if(kv->is_comment)
						{
								AR_AppendFormatString(out, L"%ls\r\n", comment);
						}else
						{

								if(AR_wcslen(val) == 0)
								{
										AR_AppendFormatString(out, L"%ls %ls\r\n", key, comment);
								}else
								{
										AR_AppendFormatString(out, L"%ls=%ls %ls\r\n", key, val, comment);
								}
						}
				}

				AR_AppendString(out, L"\r\n");
		}

}


#if(0)

void			Ini_SetString(const iniObject_t *obj, const wchar_t *sect, const wchar_t *key, const wchar_t *val, const wchar_t *comment);
void			Ini_SetInt(const iniObject_t *obj, const wchar_t *sect, const wchar_t *key, int_64_t val, const wchar_t *comment);
void			Ini_SetUInt(const iniObject_t *obj, const wchar_t *sect, const wchar_t *key, uint_64_t val, const wchar_t *comment);
void			Ini_SetUFloat(const iniObject_t *obj, const wchar_t *sect, const wchar_t *key, double val, const wchar_t *comment);
#endif






AR_NAMESPACE_END

