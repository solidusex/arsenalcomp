


#include "test.h"
#include <iostream>
#include <locale.h>
#include <wchar.h>

//using namespace std;
using namespace ARSpace;




void AR_STDCALL tiny_error(int_t level, const wchar_t* msg, void *ctx)
{

        printf("%ls\r\n", msg);
}

void AR_STDCALL tiny_printf(const wchar_t *msg, void *ctx)
{

        printf("%ls\r\n", msg);
}






int  main()
{
        printf("%s\r\n", AR_FUNC_NAME);
        arInit_t ai = {{tiny_error, tiny_printf, NULL}};

        //printf("%s\r\n", setlocale(LC_ALL, NULL));
        Arsenal_Init(&ai);

        AR_Test();


        Arsenal_UnInit();



	printf("done\r\n");


	getchar();

	return 0;

}
