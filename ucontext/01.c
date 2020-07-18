#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ucontext.h>

int main( void ) 
{
	int i;
	ucontext_t ctx;

	getcontext(&ctx);
	printf("i=%d\n", i++);
	sleep(1);
	setcontext(&ctx);
}
