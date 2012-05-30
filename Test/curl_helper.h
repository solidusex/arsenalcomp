#ifndef __CURL_HELPER_H__
#define __CURL_HELPER_H__

#include "test.h"

#if defined(__cplusplus)
extern "C"{

#include <curl/curl.h>

}

#else

#include <curl/curl.h>

#endif

#include "../Arsenal/Common/common.h"

#if defined(__LIB)


AR_NAMESPACE_BEGIN

typedef struct __http_request_callback_tag			httpRequestCallback_t;

/*
���»ص���������AR_S_YES�������������ֹ
*/
typedef arStatus_t		(*httpRequest_OnStatusFunc_t)(httpRequestCallback_t *cb, uint_32_t status, const char *reason);
typedef arStatus_t		(*httpRequest_OnHeaderFunc_t)(httpRequestCallback_t *cb, const char *header_type, const char *header_val);
typedef arStatus_t		(*httpRequest_OnResponseFunc_t)(httpRequestCallback_t *cb, const arBuffer_t *response_body);



struct __http_request_callback_tag
{
		void							*usr_ctx;
		arHash_t						*response_header;
		char							err_buf[CURL_ERROR_SIZE];
		
		uint_32_t						http_status;

		httpRequest_OnStatusFunc_t		on_status;		/*�������URL�ض�λ������ܱ���������*/
		httpRequest_OnHeaderFunc_t		on_header;
		httpRequest_OnResponseFunc_t	on_response;	/*��������ڴ�������ô˺�����response_bodyΪ����˷��ص�����*/
};


httpRequestCallback_t*	Http_CreateRequestCallback(void	*usr_ctx);
void					Http_DestroyRequestCallback(httpRequestCallback_t *cb);
const char*				Http_GetResponseHeaderValue(const httpRequestCallback_t *cb, const char *type);



typedef enum
{
		HTTP_REQ_INVALID,
		HTTP_REQ_HEAD,
		HTTP_REQ_GET,
		HTTP_REQ_PUT,
		HTTP_REQ_POST,
		HTTP_REQ_DELETE,
		HTTP_REQ_MOVE, // Caller will need to set 'Destination' header
		HTTP_REQ_MAX
}httpRequestType_t;



typedef struct __http_request_tag	
{
		CURL                    *curl;
        struct curl_slist       *request_headers;

        struct curl_httppost    *formpost;
        struct curl_httppost    *lastptr;
		
		httpRequestCallback_t	*callback;
}httpRequest_t;

httpRequest_t*	Http_CreateRequest(httpRequestType_t t);
void			Http_DestroyRequest(httpRequest_t *request);
void			Http_ClearRequest(httpRequest_t *request);

arStatus_t		Http_SetRequestURL(httpRequest_t *request, const char *url);
const char*		Http_GetRequestURL(const httpRequest_t *request);
arStatus_t		Http_SetRequestProxy(httpRequest_t *request, const char *proxy);		/*proxyΪNULL����رմ���*/

arStatus_t		Http_AddRequestHeader(httpRequest_t *request, const char *header);
arStatus_t		Http_SetRequestCallback(httpRequest_t *request,  httpRequestCallback_t *cb);

arStatus_t		Http_PerformRequest(httpRequest_t *request);



AR_NAMESPACE_END


#endif



#endif

