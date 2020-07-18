#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "coroutine.h"

schedule_t *schedule_create() {
	schedule_t *s = (schedule_t*)malloc(sizeof(schedule_t));
	if ( s != NULL ) {
		s->coroutines = (coroutine_t**)malloc(sizeof(coroutine_t*)*CORSZ); memset(s->coroutines, 0x00, sizeof(coroutine_t*)*CORSZ);

		s->current_id = -1;
		s->max_id = 0;
	}

	return s;
}

// 创建协程，返回当前协程在调度器的下标
int coroutine_create(schedule_t *s, void *(*call_back)(schedule_t *, void *args), void *args) {
	int i;
	coroutine_t *c = NULL;
	for (i=0; i<s->max_id; i++) {
		c = s->coroutines[i];
		if ( c->state == DEAD )
			break;
	}

	if ( i==s->max_id || c == NULL ) {
		s->coroutines[i] = (coroutine_t*)malloc(sizeof(coroutine_t));
		s->max_id ++;
	}

	c = s->coroutines[i];
	c->call_back = call_back;
	c->args = args;
	c->state = READY;

	getcontext(&c->ctx);
	c->ctx.uc_stack.ss_sp = c->stack;
	c->ctx.uc_stack.ss_size = STACKSZ;
	c->ctx.uc_stack.ss_flags = 0;
	c->ctx.uc_link = &s->ctx_main;
	makecontext(&c->ctx, (void (*)())&main_fun, 1, s);

	return i;
}
