#ifndef __COROUTINE_H__
#define __COROUTINE_H__

#include <ucontext.h>

#define STACKSZ (1024*64)

struct schedule;

enum State {DEAD, READY, RUNNING, SUSPEND};

// 协程结构体
typedef struct {
	void *(*call_back)(struct schedule *s, void*args); // 回调函数
	void *args; // 回调函数参数
	ucontext_t ctx; // 协程上下文
	char stack[STACKSZ]; // 协程栈
	enum State state; // 协程状态
}coroutine_t;

// 协程调度器
typedef struct schedule {
	coroutine_t **coroutines; // 所有协程
	int current_id; // 当前正在运行的协程，如果没有正在运行的协程-1
	int max_id; // 最大的下标
	ucontext_t ctx_main; // 主流程上下文
}schedule_t;

//__COROUTINE_H__
