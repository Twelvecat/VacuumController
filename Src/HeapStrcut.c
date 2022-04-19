#include "HeapStrcut.h"
//�󶥶�

PriorityQueue *safe_event_pq;

//��ʼ�����ȶ��� ,�����նѣ���ʼ��Ԫ��Ϊ0 
PriorityQueue *HEAP_init(int max_size){  //�����ʼ��init����Ϊָ�뺯�����䷵��ֵ��ָ�룬����ֱ�Ӹ�ֵ��*pq 
	PriorityQueue *pq = NULL;
	
	//��������С�ĺϷ��� 
	if(max_size <= 0){
		user_heap_error("max_size is illeagel!");
		return NULL;		
	} 
	pq = malloc(sizeof(PriorityQueue));
	
	if(pq == NULL){
		user_heap_error("malloc failed!");
		return NULL;
	}
	//�±�Ϊ0��λ�ñ�������ʹ�� 
	pq->eles = malloc((max_size+1)*sizeof(Type));
	if(pq->eles == NULL){
		user_heap_error("eles malloc failed!");
		free(pq);
		return NULL;
	}
	
	//��ʼ�����г�Ա
	memset(pq->eles, 0, (max_size+1)*sizeof(Type));
	pq->capacity = max_size;
	pq->size = 0;
	 
	return pq;
}

//�ж��Ƿ�Ϊ�� 
int HEAP_is_empty(PriorityQueue *pq){
	if(pq == NULL)
		return 0;
	if(pq->size == 0)
		return 1;
	else
		return 0; 
}

//�ж��Ƿ�Ϊ��
int HEAP_is_full(PriorityQueue *pq){
	if(pq == NULL)
		return 0;
	if(pq->size == pq->capacity)
		return 1;
	else
		return 0;
}

//����
int HEAP_push(Type value, PriorityQueue *pq){
	int i=0;
	 
	//�����ж϶����Ƿ�Ϊ�� 
	if(HEAP_is_full(pq)){
		user_heap_error("Queue is Full!");
		return 0;
	}
	user_heap_info("push priority:%d data:%d", value.priority, value.data);
	
	//�����λ��+1,��Ϊ������Ŀ�Ѩ�����Ͻ���Ѩ�븸�ڵ�Ƚϣ�����ȸ��ڵ�����Ѩ���� 
	for(i = pq->size+1; value.priority > pq->eles[i/2].priority && i>1; i/=2){
		pq->eles[i] = pq->eles[i/2];
	} 
	pq->eles[i] = value; //��value�ŵ����Ŀ�Ѩ�� 
//	user_heap_info("value: priority:%d data:%d addr:%x addr_p:%x addr_d:%x;",value.priority,value.data,&value,&value.priority,&value.data);
//  user_heap_info("pq->eles[i]: priority:%d data:%d addr:%x addr_p:%x addr_d:%x;",pq->eles[i].priority,pq->eles[i].data,&pq->eles[i],&pq->eles[i].priority,&pq->eles[i].data);
	pq->size ++;
	return 1;	
}

//������ֵ 
int HEAP_find_max(PriorityQueue *pq, Type *value){
	if(HEAP_is_empty(pq)){
		user_heap_error("Queue is empty");
		return 0;
	}
	*value = pq->eles[1];
	return 1;
}

//ɾ�� 
int HEAP_pop(PriorityQueue *pq, Type *max){
	int i=1;
	int maxchild = 0;
	
	//�ж��Ƿ�Ϊ�� 
	if(HEAP_is_empty(pq)){
		user_heap_error("Queue is empty!");
		return 0;
	}
	//�Ȱ����һ��Ԫ��ȡ���� 
	Type last;
	last = pq->eles[pq->size];
	
	//ȡ�����ֵ 
	*max = pq->eles[1];
	user_heap_info("Pop %d", pq->eles[1].priority);
	pq->size --;
	if(pq->size == 0){ //���ֻ��һ��Ԫ��
		pq->eles[i].priority = 0; //ɾ��֮���Ϊ0
        pq->eles[i].data = 0; //ɾ��֮���Ϊ0  
		return 1;	
	}
	
	//����һ��Ԫ����Ϊ��Ѩ������������ 
	for(i=1; i * 2 <= pq->size; i = maxchild){
		maxchild = i * 2;
		//ѡ�����������е����ֵ 
		if(pq->eles[maxchild].priority < pq->eles[maxchild+1].priority && maxchild != pq->size)
			maxchild += 1;
		
		//���last�ȿ�Ѩ����Ԫ��С�����Ѩ��Ҫ����
		if(last.priority < pq->eles[maxchild].priority)	
			pq->eles[i] = pq->eles[maxchild];
		else
			break;
	} 
	pq->eles[i] = last;
	return 1;	 
} 
//�ݻ�
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

//����
int HEAP_main(void){
	//��������Ϊ6�����ȶ��� 
	PriorityQueue *pq = HEAP_init(10); //init�ڶ���ʱ������Ϊָ�룬����䷵��ֵ�Ǹ�ָ�룬ֱ�Ӹ��Ƹ�pq 
	SafeEvent arr[] = {{5,5},{2,4},{3,6},{1,4},{4,4},{9,4},{7,3},{2,10},{8,6}};
	//int arr[]={3,3,7,6,8,7,9,10} ;
	int i = 0;
	
	//��ͼ�������6����Ԫ�أ��������Ԫ�ؽ��޷����� 
	for(i = 0; i<8; i++){
		HEAP_push(arr[i], pq);
	} 
	
	//������������
	user_heap_info("the value of pq are: ") ;
	for(i=0; i<pq->size; i++){
		user_heap_info("priority:%d data:%d;", pq->eles[i+1].priority, pq->eles[i+1].data); 
	}
	user_heap_info("pq size is %d", pq->size);
	
	Type max;
	int size = pq->size;
	//ÿ�δӶѶ�ȡԪ��
	for(i=0; i<size; i++){
		if(HEAP_find_max(pq, &max)){
			user_heap_info("the max is priority:%d data:%d", max.priority, max.data);
			HEAP_pop(pq, &max);
		}	
	}
	user_heap_info("new size is %d", pq->size) ;
	
	//���ٶ��� 
	HEAP_destroy(pq);
	
	return 0;
}