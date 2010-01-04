


#include "test.h"
#include <iostream>
#include "Arsenal.h"
#pragma comment(lib, "Arsenal.lib")



//using namespace std;
using namespace ARSpace;


const wchar_t *test =
L"{"

L"		int i; int j;  float[100] a; float v; float x;"
L"	bool_t x; char[1024] carr;"

L"	while(true)"
L"	{"
L"		do i = i + 1; while(a[i] < v);"
L"		do j = j-1; while(a[j] > v);"
L"		if(i >= j)break;"
L"		x = a[i]; a[i] = a[j]; a[j] = x;"
L"	}"
L"}"
;


void AR_STDCALL tiny_error(int_t level, const wchar_t* msg, void *ctx)
{
		printf("%ls\r\n", msg);
}

void AR_STDCALL tiny_printf(const wchar_t *msg, void *ctx)
{
		printf("%ls", msg);
}






int main()
{
		
		

		arInit_t ai = {tiny_error, tiny_printf, NULL};
		arCtx_t ctx;
		ctx.init = ai;

		

		Arsenal_Init(&ctx);
		
		AR_Test();
		

		Arsenal_UnInit();

		_CrtDumpMemoryLeaks();

		printf("done\r\n");

		
		getchar();

		return 0;

}
