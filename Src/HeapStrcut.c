#include "HeapStrcut.h"
//大顶堆

PriorityQueue *safe_event_pq;

//初始化优先队列 ,创建空堆，初始化元素为0 
PriorityQueue *HEAP_init(int max_size){  //这里初始化init定义为指针函数，其返回值是指针，可以直接赋值给*pq 
	PriorityQueue *pq = NULL;
	
	//检查输入大小的合法性 
	if(max_size <= 0){
		user_heap_error("max_size is illeagel!");
		return NULL;		
	} 
	pq = malloc(sizeof(PriorityQueue));
	
	if(pq == NULL){
		user_heap_error("malloc failed!");
		return NULL;
	}
	//下标为0的位置保留，不使用 
	pq->eles = malloc((max_size+1)*sizeof(Type));
	if(pq->eles == NULL){
		user_heap_error("eles malloc failed!");
		free(pq);
		return NULL;
	}
	
	//初始化队列成员
	memset(pq->eles, 0, (max_size+1)*sizeof(Type));
	pq->capacity = max_size;
	pq->size = 0;
	 
	return pq;
}

//判断是否为空 
int HEAP_is_empty(PriorityQueue *pq){
	if(pq == NULL)
		return 0;
	if(pq->size == 0)
		return 1;
	else
		return 0; 
}

//判断是否为满
int HEAP_is_full(PriorityQueue *pq){
	if(pq == NULL)
		return 0;
	if(pq->size == pq->capacity)
		return 1;
	else
		return 0;
}

//插入
int HEAP_push(Type value, PriorityQueue *pq){
	int i=0;
	 
	//首先判断队列是否为满 
	if(HEAP_is_full(pq)){
		user_heap_error("Queue is Full!");
		return 0;
	}
	user_heap_info("push priority:%d data:%d", value.priority, value.data);
	
	//将最后位置+1,作为待插入的空穴，不断将空穴与父节点比较，如果比父节点大，则空穴上移 
	for(i = pq->size+1; value.priority > pq->eles[i/2].priority && i>1; i/=2){
		pq->eles[i] = pq->eles[i/2];
	} 
	pq->eles[i] = value; //将value放到最后的空穴中 
	pq->size ++;
	return 1;	
}

//查找最值 
int HEAP_find_max(PriorityQueue *pq, Type *value){
	if(HEAP_is_empty(pq)){
		user_heap_error("Queue is empty");
		return 0;
	}
	*value = pq->eles[1];
	return 1;
}

//删除 
int HEAP_pop(PriorityQueue *pq, Type *max){
	int i=1;
	int maxchild = 0;
	
	//判断是否为空 
	if(HEAP_is_empty(pq)){
		user_heap_error("Queue is empty!");
		return 0;
	}
	//先把最后一个元素取出来 
	Type last;
	last = pq->eles[pq->size];
	
	//取得最大值 
	*max = pq->eles[1];
	user_heap_info("Pop %d", pq->eles[1].priority);
	pq->size --;
	if(pq->size == 0){ //如果只有一个元素
		pq->eles[i].priority = 0; //删除之后变为0
        pq->eles[i].data = 0; //删除之后变为0  
		return 1;	
	}
	
	//将第一个元素作为空穴，并不断下移 
	for(i=1; i * 2 <= pq->size; i = maxchild){
		maxchild = i * 2;
		//选择两个孩子中的最大值 
		if(pq->eles[maxchild].priority < pq->eles[maxchild+1].priority && maxchild != pq->size)
			maxchild += 1;
		
		//如果last比空穴处的元素小，则空穴还要往下
		if(last.priority < pq->eles[maxchild].priority)	
			pq->eles[i] = pq->eles[maxchild];
		else
			break;
	} 
	pq->eles[i] = last;
	return 1;	 
} 
//摧毁
int HEAP_destroy(PriorityQueue *pq) {
	if(pq == NULL){
		return 0;
	}
	free(pq->eles);
	pq->eles = NULL;
	free(pq);
	pq = NULL;
	
	user_heap_info("destroy success");
	return 1;
}

//测试
int HEAP_main(void){
	//创建容量为6的优先队列 
	PriorityQueue *pq = HEAP_init(10); //init在定义时，定义为指针，因此其返回值是个指针，直接复制给pq 
	SafeEvent arr[] = {{5,5},{2,4},{3,6},{1,4},{4,4},{9,4},{7,3},{2,10},{8,6}};
	//int arr[]={3,3,7,6,8,7,9,10} ;
	int i = 0;
	
	//试图插入多于6个的元素，最后两个元素将无法插入 
	for(i = 0; i<8; i++){
		HEAP_push(arr[i], pq);
	} 
	
	//遍历队列内容
	user_heap_info("the value of pq are: ") ;
	for(i=0; i<pq->size; i++){
		user_heap_info("priority:%d data:%d;", pq->eles[i+1].priority, pq->eles[i+1].data); 
	}
	user_heap_info("pq size is %d", pq->size);
	
	Type max;
	int size = pq->size;
	//每次从堆顶取元素
	for(i=0; i<size; i++){
		if(HEAP_find_max(pq, &max)){
			user_heap_info("the max is priority:%d data:%d", max.priority, max.data);
			HEAP_pop(pq, &max);
		}	
	}
	user_heap_info("new size is %d", pq->size) ;
	
	//销毁队列 
	HEAP_destroy(pq);
	
	return 0;
}
