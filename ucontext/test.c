#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "coroutine.h"

void *fun1(schedule_t *s, void *args) {
	printf("fun1() start\n");
	coroutine_yield(s);
	int a = *(int*)args;
	printf("fun1 exit : %d\n", a);

	return NULL;
}

void *fun2(schedule_t *s, void *args) {
	printf("fun2() start\n");
	coroutine_yield(s);
	int a = *(int*)args;
	printf("fun2 exit : %d\n", a);

	return NULL;
}
int main( void ) {
	schedule_t *s = schedule_create();

	int *a = malloc(sizeof(int)); *a = 1;
	int *b = malloc(sizeof(int)); *b = 2;

	int id1 = coroutine_create(s, fun1, a);
	int id2 = coroutine_create(s, fun2, b);

	coroutine_running(s, id1);
	coroutine_running(s, id2);

	while ( !schedule_finished(s) ) {
		coroutine_resume(s, id2);
		coroutine_resume(s, id1);
	}

	schedule_destroy(s);
}
