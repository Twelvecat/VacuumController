#ifndef __HEAPSTRUCT_H__
#define __HEAPSTRUCT_H__
//大顶堆
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

#define USER_HEAP_DEBUG
#ifdef USER_HEAP_DEBUG
	#define user_heap_printf(format, ...) printf( format "\r\n", ##__VA_ARGS__)
	#define user_heap_info(format, ...) printf("[\theap]info:" format "\r\n", ##__VA_ARGS__)
	#define user_heap_debug(format, ...) printf("[\theap]debug:" format "\r\n", ##__VA_ARGS__)
	#define user_heap_error(format, ...) printf("[\theap]error:" format "\r\n",##__VA_ARGS__)
#else
	#define user_heap_printf(format, ...)
	#define user_heap_info(format, ...)
	#define user_heap_debug(format, ...)
	#define user_heap_error(format, ...)
#endif

#define Type SafeEvent 

typedef struct SafeEvent{
	int priority; //优先级
	int data; //内容
}SafeEvent;

/*定义优先队列的数据结构*/ 
typedef struct HeapStruct{
	int capacity; //最大容量 
	int size;
	Type *eles; 
}PriorityQueue;

PriorityQueue * HEAP_init(int max_size);
int HEAP_is_empty(PriorityQueue *pq);
int HEAP_is_full(PriorityQueue *pq);
int HEAP_push(Type value, PriorityQueue *pq);
int HEAP_find_max(PriorityQueue *pq, Type *value);
int HEAP_pop(PriorityQueue *pq, Type *max);
int HEAP_destroy(PriorityQueue *pq);
int HEAP_main(void);

extern PriorityQueue *safe_event_pq;

#endif /* __HEAPSTRUCT_H__ */
