#include "test.h"


AR_NAMESPACE_BEGIN




struct __arsenal_uri_tag;
typedef struct __arsenal_uri_tag		arURI_t;


arURI_t*		AR_CreateURI(arCodePage_t cp);
void			AR_DestroyURI(arURI_t *uri);
void			AR_ClearURI(arURI_t *uri);
int_t			AR_CompURI(const arURI_t *l, const arURI_t *r);

arCodePage_t	AR_GetURICodePage(const arURI_t *uri);
void			AR_SetURICodePage(arURI_t *uri, arCodePage_t cp);

bool_t			AR_IsRelativeURI(const arURI_t *uri);

arStatus_t		AR_NormalizeURI(arURI_t *uri);

/*����Ĭ����Ϊ�����URI��ȫΪ������URL��֮�����URI���õ�code page��������uri���룬��ת��Ϊunicode*/
arStatus_t		AR_SetURI(arURI_t *uri, const wchar_t *str);

/*ȡ�õ�URIΪ������URI,����unicode��ʽ����*/
arStatus_t		AR_GetURI(const arURI_t *uri, arString_t *str);




arStatus_t		AR_GetURIScheme(const arURI_t *uri, arString_t *str);
arStatus_t		AR_SetURIScheme(arURI_t *uri, const wchar_t *str);

arStatus_t		AR_GetURIUserInfo(const arURI_t *uri, arString_t *str);
arStatus_t		AR_SetURIUserInfo(arURI_t *uri, const wchar_t *str);

arStatus_t		AR_GetURIHost(const arURI_t *uri, arString_t *str);
arStatus_t		AR_SetURIHost(arURI_t *uri, const wchar_t *str);

uint_16_t		AR_GetURIPort(const arURI_t *uri);
void			AR_SetURIPort(arURI_t *uri, uint_16_t port);

arStatus_t		AR_GetURIAuthority(const arURI_t *uri, arString_t *str);
arStatus_t		AR_SetURIAuthority(arURI_t *uri, const wchar_t *str);

arStatus_t		AR_GetURIPath(const arURI_t *uri, arString_t *str);
arStatus_t		AR_SetURIPath(arURI_t *uri, const wchar_t *str);

arStatus_t		AR_GetURIQuery(const arURI_t *uri, arString_t *str);
arStatus_t		AR_SetURIQuery(arURI_t *uri, const wchar_t *str);


arStatus_t		AR_GetURIFragment(const arURI_t *uri, arString_t *str);
arStatus_t		AR_SetURIFragment(arURI_t *uri, const wchar_t *str);

arStatus_t		AR_GetURIPathEtc(const arURI_t *uri, arString_t *str);
arStatus_t		AR_SetURIPathEtc(arURI_t *uri, const wchar_t *str);





AR_NAMESPACE_END
