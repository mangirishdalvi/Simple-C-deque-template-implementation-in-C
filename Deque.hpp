#include <stdlib.h>
#include <string.h>

#ifndef DEQUE_H
#define DEQUE_H

#define INIT_SIZE 20

#define Deque_DEFINE(type) \
struct Deque_##type; \
struct type##function_wrapper \
{ \
	bool (*less)(const type &,const type &); \
	type val; \
}; \
struct Deque_##type##_Iterator \
{ \
 	Deque_##type *ptr; \
 	int index; \
 	void (*inc)(Deque_##type##_Iterator*); \
 	void (*dec)(Deque_##type##_Iterator*); \
 	type& (*deref)(Deque_##type##_Iterator*); \
}; \
struct Deque_##type \
{ \
	type *data; \
	char type_name[6+sizeof(#type)]={0}; \
	int nSize; \
	int capacity; \
	int start; \
	int last; \
	type& (*front)(Deque_##type *); \
	type& (*back)(Deque_##type *); \
	type& (*at)(Deque_##type *,int); \
	void (*push_back)(Deque_##type *,const type& val); \
	void (*push_front)(Deque_##type *,const type& val); \
	void (*pop_front)(Deque_##type *); \
	void (*pop_back)(Deque_##type *); \
	Deque_##type##_Iterator (*begin)(Deque_##type *); \
	Deque_##type##_Iterator (*end)(Deque_##type *); \
	bool (*less)(const type &,const type &); \
	size_t (*size)(Deque_##type *); \
	bool (*empty)(Deque_##type *); \
	void (*sort)(Deque_##type *,Deque_##type##_Iterator,Deque_##type##_Iterator); \
	void (*clear)(Deque_##type *); \
	void (*dtor)(Deque_##type *); \
}; \
bool empty(Deque_##type *ths) \
{ \
 	return (ths->nSize==0); \
} \
size_t size(Deque_##type *ths) \
{ \
	return ths->nSize; \
} \
void Deque_##type##resize(Deque_##type *deq,size_t newSize) \
{ \
	type *copy=(type *)calloc(newSize,sizeof(type)); \
	int dataIndex,copyIndex; \
	for(copyIndex=0,dataIndex=deq->start;dataIndex!=deq->last;copyIndex++,dataIndex=(dataIndex+1)%deq->capacity) \
	{ \
		copy[copyIndex]=deq->data[dataIndex]; \
	}  \
	free(deq->data); \
	deq->start=0; \
	deq->last=copyIndex; \
	deq->data=copy; \
	deq->capacity=newSize; \
} \
type& front(Deque_##type *ths) \
{ \
	return ths->data[ths->start]; \
} \
type& back(Deque_##type *ths) \
{ \
	return (ths->last==0)?ths->data[ths->capacity-1]:ths->data[ths->last-1]; \
} \
type& at(Deque_##type *ths,int index) \
{ \
	return ths->data[(ths->start+index)%ths->capacity]; \
} \
void push_back(Deque_##type *ths,const type& val) \
{ \
  int next=(ths->last==ths->capacity-1)?0:(ths->last+1); \
  if(next==ths->start)\
  { \
	Deque_##type##resize(ths,ths->capacity*3); \
	ths->data[ths->last++]=val; \
	ths->nSize++; \
	return; \
  } \
  ths->data[ths->last]=val; \
  ths->last=next; \
  ths->nSize++; \
} \
void push_front(Deque_##type *ths,const type& val) \
{ \
	if(ths->nSize==0) \
	{ \
		ths->last++; \
		ths->data[ths->start]=val; \
		ths->nSize++; return; \
	} \
	int next=(ths->start==0)?(ths->capacity-1):(ths->start-1); \
	if(next==ths->last) \
	{ \
		Deque_##type##resize(ths,ths->capacity*3); \
		ths->start=ths->capacity-1; \
		ths->data[ths->start]=val; \
		ths->nSize++; return; \
	} \
	ths->start=next; \
	ths->data[ths->start]=val; \
	ths->nSize++; \
} \
void pop_front(Deque_##type *ths) \
{ \
	if(ths->nSize==0) \
	{ \
		return; \
	} \
	ths->start=(ths->start==ths->capacity-1)?0:(ths->start+1); \
	if(ths->start==ths->last) \
	{ \
		ths->start=0; ths->last=0; ths->nSize=0; \
		return; \
	} \
	ths->nSize--; \
} \
void pop_back(Deque_##type *ths) \
{ \
	if(ths->nSize==0) \
	{ \
		return; \
	} \
	ths->last=(ths->last==0)?(ths->capacity-1):(ths->last-1); \
	if(ths->start==ths->last) \
	{ \
		ths->start=0; ths->last=0; ths->nSize=0; \
		return; \
	} \
	ths->nSize--; \
} \
void clear(Deque_##type *ths) \
{ \
	type *copy=(type *)calloc(INIT_SIZE,sizeof(type)); \
	ths->nSize=0; \
	ths->start=0; ths->last=0; \
	ths->capacity=INIT_SIZE; \
	free(ths->data); \
	ths->data=copy; \
} \
void inc(Deque_##type##_Iterator *ths) \
{ \
	ths->index=(ths->index==(ths->ptr->capacity-1))?0:ths->index+1; \
} \
void dec(Deque_##type##_Iterator *ths) \
{ \
	ths->index=(ths->index==0)?(ths->ptr->capacity-1):ths->index-1; \
} \
type& deref(Deque_##type##_Iterator *ths) \
{ \
	return ths->ptr->data[ths->index]; \
} \
bool Deque_##type##_Iterator_equal(Deque_##type##_Iterator a,Deque_##type##_Iterator b) \
{ \
	return (a.index==b.index); \
} \
Deque_##type##_Iterator begin(Deque_##type *ths) \
{ \
	Deque_##type##_Iterator res; \
	res.ptr=ths; \
	res.index=ths->start; \
	res.inc=inc; \
	res.dec=dec; \
	res.deref=deref; \
	return res; \
} \
Deque_##type##_Iterator end(Deque_##type *ths) \
{ \
	Deque_##type##_Iterator res; \
	res.ptr=ths; \
	res.index=ths->last; \
	res.inc=inc; \
	res.dec=dec; \
	res.deref=deref; \
	return res; \
} \
bool Deque_##type##_equal(Deque_##type o1,Deque_##type o2) \
{ \
	if(o1.size(&o1)!=o2.size(&o2)) \
		return false; \
	for(size_t i=0;i<o1.size(&o1);i++) \
	{ \
		if(o1.less(o1.at(&o1,i),o2.at(&o2,i))||o1.less(o2.at(&o2,i),o1.at(&o1,i))) \
		{ \
			return false; \
		} \
	} \
	return true; \
} \
int type##compare(const void *a,const void *b,void *ths) \
{ \
	if(((Deque_##type *)ths)->less(*(type*)a,*(type*)b)) \
	{ \
		return -1; \
	} \
	if(((Deque_##type *)ths)->less(*(type*)b,*(type*)a)) \
	{ \
		return 1; \
	} \
	return 0; \
} \
void sort(Deque_##type *ths,Deque_##type##_Iterator beg,Deque_##type##_Iterator e) \
{ \
	int sortSize=(beg.index>e.index)?(ths->capacity-(beg.index-e.index))\
									:(e.index-beg.index); \
 	type* array=(type *)calloc(sortSize,sizeof(type)); \
 	int i,j; \
 	for (i = 0,j=beg.index; j != e.index; ++i,j=(j+1)%ths->capacity) \
 	{ \
 		array[i]=ths->data[j]; \
 	} \
 	qsort_r(array,i,sizeof(type),type##compare,ths); \
 	for(i=0,j=beg.index;j!=e.index;j=(j+1)%ths->capacity,i++) \
 	{ \
 		ths->data[j]=array[i]; \
 	} \
 	free(array); \
} \
void dtor(Deque_##type *ths) \
{ \
	free(ths->data); \
} \
void Deque_##type##_ctor(Deque_##type *ths,bool (*less)(const type &o1,const type &o2)) \
{ \
  ths->data=(type *) calloc(INIT_SIZE,sizeof(type)); \
  strcat(ths->type_name,"Deque_"); \
  strcat(ths->type_name,#type); \
  ths->capacity=INIT_SIZE; \
  ths->nSize=0; \
  ths->start=0; \
  ths->last=0; \
  ths->size=size; \
  ths->front=front; \
  ths->at=at; \
  ths->back=back; \
  ths->push_back=push_back; \
  ths->push_front=push_front; \
  ths->pop_back=pop_back; \
  ths->pop_front=pop_front; \
  ths->begin=begin; \
  ths->end=end; \
  ths->less=less; \
  ths->sort=sort; \
  ths->clear=clear; \
  ths->dtor=dtor; \
  ths->empty=empty; \
}

#endif

