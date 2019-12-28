#include"cust_node.h"


struct Queue
{
    node *first,*last;
    int currentTime,totalQueueTime,queueWaitTime,queueSize;

};
typedef struct Queue queu;

int isQueueEmpty(queu *q)
{
    return (q->first==NULL && q->last==NULL);
}

void enqueue(node * nod,queu *q)
{
    if(isQueueEmpty(q))
    {
        q->first=nod;
        q->last=q->first;
    }
    else
    {
        q->last->nextP=nod;
        q->last=nod;
    }

    q->queueSize++;
}

customer * dequeue(int *emptyFlag,queu *q)
{
    if(q->first==NULL)
    {
        //node nod = q->first;
        //first=NULL;
        q->last=NULL;
        *emptyFlag=1;
        return NULL;
    }
    else
    {
        node *nod=q->first;
        q->first=q->first->nextP;
        //not need to change node pointer
        //nod->nextP=NULL;
        *emptyFlag=0;
        return nod->custData;

    }

}


void displayQueue(node * disP)
{

    while(disP!=NULL)
    {
        printf("%d",disP->custData->arriveTime);
        printf(" -- ");
        disP=disP->nextP;
    }
    printf("\n");
}

void displayQueueInternalData(node * disP,int x)
{

    while(disP!=NULL)
    {
        printf("Customer NO:%d \033[0m",x);
        printf("\n----\nArrive Time: %d \nCustomer Time: %d\nWait Time: %d",disP->custData->arriveTime,disP->custData->custTime,disP->custData->waitCustTime);
        printf("\n-----------------\n");
        disP=disP->nextP;
        x++;
    }
    printf("\n");
}
