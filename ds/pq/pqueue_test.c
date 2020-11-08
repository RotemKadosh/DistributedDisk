#include "../utils/test.h"
#include "pqueue.h"



#define ADJUSTMENT_TO_INSERT_AFTER_EQUAL (1)
#define EMPTY (1)
#define NOT_EMPTY (0)

/*-------------------Struct fot test----------------------*/

typedef struct test
{
	int priority;
	int queue_num;
}test_t;

/*---------------FUNCTION DECLERATION---------------------*/

int CompareFunc(const void *data, const void *data_to_compare);
int CompareFunc2(const void *data, const void *data_to_compare);
int IsMatch(const void *data, const void *data_to_compare);
int IsMatch2(const void *data, const void *data_to_compare);

static test_status_t StageOneTest(void);
static test_status_t StageTwoTest(void);
static test_status_t StageThreeTest(void);


/*-------------------------MAIN---------------------------*/


int main()
{
	RUNTEST(StageOneTest);
	RUNTEST(StageTwoTest);
	RUNTEST(StageThreeTest);

	return PASSED; 
}

/*--------------------------------------------------------*/

static test_status_t StageOneTest(void)
{
	/*create, enqueue, dequeue, isEmpty*/


	PQueue_t *user_pqueue = PQCreate(CompareFunc2);
	test_t t1 = {10,1};
	test_t t2 = {10,2};
	test_t t3 = {9,1};
	test_t t4 = {9,2};
	test_t t5 = {11,1};
	test_t t6 = {4,1};
	test_t t7 = {9,3};
	test_t t8 = {9,4};
	test_t t9 = {3,3};
	test_t t10 = {12,1};
	test_t *t11 = {0};

	REQUIRE(0 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)&t1);
	REQUIRE(1 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)&t2);
	REQUIRE(2 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)&t5);
	REQUIRE(3 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)&t3);
	REQUIRE(4 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)&t4);
	REQUIRE(5 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)&t6);
	REQUIRE(6 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)&t7);
	REQUIRE(7 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)&t8);
	REQUIRE(8 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)&t9);
	REQUIRE(9 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)&t10);
	REQUIRE(10 == PQSize(user_pqueue));



	REQUIRE(NOT_EMPTY == PQIsEmpty(user_pqueue));

	t11 = PQDepqueue(user_pqueue);
	REQUIRE(12 == t11->priority);
	REQUIRE(1 == t11->queue_num);
	t11 = PQDepqueue(user_pqueue);
	REQUIRE(11 == t11->priority);
	REQUIRE(1 == t11->queue_num);
	t11 = PQDepqueue(user_pqueue);
	REQUIRE(10 == t11->priority);
	REQUIRE(1 == t11->queue_num);
	t11 = PQDepqueue(user_pqueue);
	REQUIRE(10 == t11->priority);
	REQUIRE(2 == t11->queue_num);

	REQUIRE(NOT_EMPTY == PQIsEmpty(user_pqueue));
	
	t11 = PQDepqueue(user_pqueue);
	REQUIRE(9 == t11->priority);
	REQUIRE(1 == t11->queue_num);
	t11 = PQDepqueue(user_pqueue);
	REQUIRE(9 == t11->priority);
	REQUIRE(2 == t11->queue_num);
	t11 = PQDepqueue(user_pqueue);
	REQUIRE(9 == t11->priority);
	REQUIRE(3 == t11->queue_num);
	t11 = PQDepqueue(user_pqueue);
	REQUIRE(9 == t11->priority);
	REQUIRE(4 == t11->queue_num);

	REQUIRE(NOT_EMPTY == PQIsEmpty(user_pqueue));

	PQDestroy(user_pqueue);
	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t StageTwoTest(void)
{
	/*destroy, size, peek*/
	
	PQueue_t *user_pqueue = PQCreate(CompareFunc);

	REQUIRE(0 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)5);
	REQUIRE(1 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)10);
	REQUIRE(2 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)5);
	REQUIRE(3 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)23);
	REQUIRE(4 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)1);
	REQUIRE(5 == PQSize(user_pqueue));

	REQUIRE((void*)23 == PQDepqueue(user_pqueue));
	REQUIRE(NOT_EMPTY == PQIsEmpty(user_pqueue));

	PQDestroy(user_pqueue);

	return PASSED;
}
/*--------------------------------------------------------*/


static test_status_t StageThreeTest(void)
{
	/*clear, erase*/

	PQueue_t *user_pqueue = PQCreate(CompareFunc2);
	test_t t1 = {10,1};
	test_t t2 = {10,2};
	test_t t3 = {9,1};
	test_t t4 = {9,2};
	test_t t5 = {11,1};
	test_t t6 = {4,1};
	test_t t7 = {9,3};
	test_t t8 = {9,4};
	test_t t9 = {3,3};
	test_t t10 = {12,1};
	test_t *t11 = {0};

	REQUIRE(0 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)&t1);
	REQUIRE(1 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)&t2);
	REQUIRE(2 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)&t5);
	REQUIRE(3 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)&t3);
	REQUIRE(4 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)&t4);
	REQUIRE(5 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)&t6);
	REQUIRE(6 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)&t7);
	REQUIRE(7 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)&t8);
	REQUIRE(8 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)&t9);
	REQUIRE(9 == PQSize(user_pqueue));
	PQEnpqueue(user_pqueue,(void*)&t10);
	REQUIRE(10 == PQSize(user_pqueue));



	REQUIRE(NOT_EMPTY == PQIsEmpty(user_pqueue));

	t11 = PQErase(user_pqueue, IsMatch2, &t1);
	REQUIRE(10 == t11->priority);
	REQUIRE(1 == t11->queue_num);
	t11 = PQErase(user_pqueue, IsMatch2, &t1);
	REQUIRE(10 == t11->priority);
	REQUIRE(2 == t11->queue_num);

	t11 = PQDepqueue(user_pqueue);
	REQUIRE(12 == t11->priority);
	REQUIRE(1 == t11->queue_num);
	t11 = PQDepqueue(user_pqueue);
	REQUIRE(11 == t11->priority);
	REQUIRE(1 == t11->queue_num);
	t11 = PQDepqueue(user_pqueue);
	REQUIRE(9 == t11->priority);
	REQUIRE(1 == t11->queue_num);
	


	PQDestroy(user_pqueue);

	return PASSED;
}
/*--------------------------------------------------------*/

int CompareFunc(const void *data, const void *data_to_compare)
{
	return (*(int*)&data - *(int*)&data_to_compare);
}

int CompareFunc2(const void *data, const void *data_to_compare)
{
	test_t *t1 = (test_t*)data;
	test_t *t2 = (test_t*)data_to_compare;
	return (t1->priority - t2->priority);
}


int IsMatch(const void *data, const void *data_to_compare)
{
	return (*(int*)&data == *(int*)&data_to_compare);
}

int IsMatch2(const void *data, const void *data_to_compare)
{
	test_t *t1 = (test_t*)data;
	test_t *t2 = (test_t*)data_to_compare;
	return (t1->priority == t2->priority);
}






/*
#include "../utils/test.h" 
#include "../heap/heap.h"
#include "pqueue.h" 


#define TRUE (1)
#define FALSE (0)

int compare(const void *data,const void *date_to_compare);
int MatchInt(const void *data, const void *data_to_compare);
static test_status_t StageOneTest(void);
static test_status_t StageTwoTest(void);
static test_status_t StageThreeTest(void);

int main()
{
	RUNTEST(StageOneTest);
	
	RUNTEST(StageTwoTest);
	
	RUNTEST(StageThreeTest);

	
	return 0; 
}


static test_status_t StageOneTest(void)
{
	int num1 = 1;
	int num2 = 5;
	int num3 = 5;
	int num4 = 7;
	int num5 = 3;
	int num6 = 2;
	PQueue_t *pq = PQCreate(compare);
	REQUIRE(TRUE == PQIsEmpty(pq));
	PQEnpqueue(pq, (void *)&num1);
	REQUIRE(FALSE == PQIsEmpty(pq));
	REQUIRE(0 == PQEnpqueue(pq, (void *)&num2));
	REQUIRE(0 == PQEnpqueue(pq, (void *)&num3));
	REQUIRE(0 == PQEnpqueue(pq, (void *)&num4));
	REQUIRE(0 == PQEnpqueue(pq, (void *)&num5));
	REQUIRE(0 == PQEnpqueue(pq, (void *)&num6));

	REQUIRE(1 == *(int *)PQDepqueue(pq));
	REQUIRE(2 == *(int *)PQDepqueue(pq));
	REQUIRE(3 == *(int *)PQDepqueue(pq));
	REQUIRE(5 == *(int *)PQDepqueue(pq));
	REQUIRE(5 == *(int *)PQDepqueue(pq));
	REQUIRE(7 == *(int *)PQDepqueue(pq));

	REQUIRE(TRUE == PQIsEmpty(pq));
	PQDestroy(pq);
	return PASSED;

}
static test_status_t StageTwoTest(void)
{
	int num1 = 1;
	int num2 = 5;
	int num3 = 5;
	int num4 = 7;
	int num5 = 3;
	int num6 = 2;
	PQueue_t *pq = PQCreate(compare);
	REQUIRE(TRUE == PQIsEmpty(pq));
	PQEnpqueue(pq, (void *)&num1);
	REQUIRE(1 == PQSize(pq));
	REQUIRE(FALSE == PQIsEmpty(pq));
	REQUIRE(0 == PQEnpqueue(pq, (void *)&num2));
	REQUIRE(2 == PQSize(pq));
	REQUIRE(0 == PQEnpqueue(pq, (void *)&num3));
	REQUIRE(3 == PQSize(pq));
	REQUIRE(0 == PQEnpqueue(pq, (void *)&num4));
	REQUIRE(4 == PQSize(pq));
	REQUIRE(0 == PQEnpqueue(pq, (void *)&num5));
	REQUIRE(5 == PQSize(pq));
	REQUIRE(0 == PQEnpqueue(pq, (void *)&num6));

	REQUIRE(6 == PQSize(pq));
	REQUIRE(1 == *(int *)PQPeek(pq));
	REQUIRE(1 == *(int *)PQDepqueue(pq));
	REQUIRE(2 == *(int *)PQPeek(pq));
	REQUIRE(2 == *(int *)PQDepqueue(pq));
	REQUIRE(3 == *(int *)PQPeek(pq));
	REQUIRE(3 == *(int *)PQDepqueue(pq));
	REQUIRE(5 == *(int *)PQPeek(pq));
	REQUIRE(5 == *(int *)PQDepqueue(pq));
	REQUIRE(5 == *(int *)PQPeek(pq));
	REQUIRE(5 == *(int *)PQDepqueue(pq));
	REQUIRE(7 == *(int *)PQPeek(pq));
	REQUIRE(7 == *(int *)PQDepqueue(pq));
	REQUIRE(0 == PQSize(pq));
	REQUIRE(TRUE == PQIsEmpty(pq));
	PQDestroy(pq);
	return PASSED;

}
int MatchInt(const void *data, const void *data_to_compare)
{

	return *(int *)data == *(int *)data_to_compare;
}

static test_status_t StageThreeTest(void)
{
	int num1 = 1;
	int num2 = 5;
	int num3 = 5;
	int num4 = 7;
	int num5 = 3;
	int num6 = 2;
	int num_not_to_find = 9;
	PQueue_t *pq = PQCreate(compare);
	REQUIRE(TRUE == PQIsEmpty(pq));
	PQEnpqueue(pq, (void *)&num1);
	REQUIRE(1 == PQSize(pq));
	REQUIRE(FALSE == PQIsEmpty(pq));
	REQUIRE(0 == PQEnpqueue(pq, (void *)&num2));
	REQUIRE(2 == PQSize(pq));
	REQUIRE(0 == PQEnpqueue(pq, (void *)&num3));
	REQUIRE(3 == PQSize(pq));
	REQUIRE(0 == PQEnpqueue(pq, (void *)&num4));
	REQUIRE(4 == PQSize(pq));
	REQUIRE(0 == PQEnpqueue(pq, (void *)&num5));
	REQUIRE(5 == PQSize(pq));
	REQUIRE(0 == PQEnpqueue(pq, (void *)&num6));
	REQUIRE(num4 == *(int *)PQErase(pq, MatchInt,(void *)&num4));
	REQUIRE(5 == PQSize(pq));
	REQUIRE(NULL == PQErase(pq, MatchInt, (void *)&num_not_to_find));
	PQClear(pq);

	REQUIRE(TRUE == PQIsEmpty(pq));
	PQDestroy(pq);
	return PASSED;
}

int compare(const void *data,const void *date_to_compare)
{
	return ( *(int *)date_to_compare - *(int *)data);
}
*/