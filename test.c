
#include <stdio.h>
#include "prtfmt.h"

int main(void)
{
	//mini_printf("[%99% %T %hd, hex=%x, %X, %b]\n", 65534, 123, 123, 123);
	//mini_printf("short=%ld", -2);
	//mini_printf("[%9s]", "hello, world");
	//mini_printf("[%10.6s]\n", "hello, world");
	//mini_printf("[%4.6c]\n", "A");
	//mini_printf("[%4.0c]\n", "A");
	//mini_printf("[%-20c]\n", "A");
	
	//mini_printf ("[%d %x %X %b]\n", 15, 15, 15, 15);
	//mini_printf ("[%10.6d]\n", 16);
	printf ("[%10.05d]\n", 169);
	//printf ("[%b]\n", 16);

	//printf("\n\nprintf sharot=%ld", -2);

	return 0;
}

