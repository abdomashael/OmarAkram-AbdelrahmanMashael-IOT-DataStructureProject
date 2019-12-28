#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <windows.h>
#include <dos.h>
#include <dir.h>
#include "queue.h"

#define SIZE 4



void SetColor(int ForgC)
{
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    //We use csbi for the wAttributes word.
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        //Mask out all but the background attribute, and add in the forgournd     color
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}
void gotoxy(int x,int y)
{
    COORD coord= {0,0};
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

node *firstP =NULL,*lastP=NULL;

int isEmpty()
{
    return (firstP==NULL && lastP==NULL);
}
node * create(customer *cust )
{
    node *node1 = (node*) malloc(1*sizeof(node));
    node1->custData = cust;
    node1->nextP = NULL;
    node1->previousP = NULL;
}

void add(node* newNode)
{
    if(firstP==NULL)
    {
        firstP = lastP = newNode;
    }
    else
    {
        lastP->nextP=newNode;
        newNode->previousP = lastP;
        lastP=newNode;
    }

}

void display()
{
    node * disP = firstP;
    SetColor(5);
    printf("Arrival Time\tTransaction Time\n");
    SetColor(15);
    while(disP!=NULL)
    {
        printf("%d\t\t\t%d\n",disP->custData->arriveTime,disP->custData->custTime);
        disP=disP->nextP;
    }
}

node* search(int arriveTime)
{
    node *searchP= firstP;
    while(searchP!=NULL)
    {
        if(searchP->custData->arriveTime == arriveTime)
        {
            return searchP;
        }
        searchP=searchP->nextP;
    }
    return NULL;
}



void deletet(node * deleteP)
{

    if(deleteP == firstP)
    {
        if(deleteP==lastP)
        {
            firstP=NULL;
            lastP=NULL;
        }
        else
        {
            firstP = firstP->nextP;
            firstP->previousP=NULL;
        }
    }
    else if(deleteP==lastP)
    {
        lastP=lastP->previousP;
        lastP->nextP=NULL;
    }
    else
    {
        deleteP->previousP->nextP=deleteP->nextP;
        deleteP->nextP->previousP=deleteP->previousP;

    }

    deleteP->nextP=NULL;
    deleteP->previousP=NULL;
    free(deleteP);
}

void insertAfter(node* currentNode,node* insertedNode)
{
    if(currentNode==lastP)
    {
        lastP->nextP=insertedNode;
        insertedNode->previousP=lastP;
        lastP=insertedNode;
    }
    else
    {
        insertedNode->nextP=currentNode->nextP;
        insertedNode->previousP=currentNode;
        currentNode->nextP->previousP=insertedNode;
        currentNode->nextP=insertedNode;
    }
}

void insertBefore(node* currentNode,node* insertedNode)
{
    if(currentNode==firstP)
    {
        firstP->previousP=insertedNode;
        insertedNode->nextP=currentNode;
        firstP=insertedNode;
    }
    else
    {
        insertedNode->nextP=currentNode;
        insertedNode->previousP=currentNode->previousP;
        currentNode->previousP->nextP=insertedNode;
        currentNode->previousP=insertedNode;
    }
}

void moveCustomersToQueues(queu* arr)
{
    int time=1,id=0,size =1;
    customer* cust = (customer*) malloc(100*sizeof(customer));
    while(!isEmpty())
    {
        node* nod=search(time);
        if(nod!=NULL)
        {
            int minTimeArr =0;
            for(int x=1; x<3; x++)
            {
                if(arr[x].currentTime<arr[x-1].currentTime)
                {
                    minTimeArr = x;
                }
            }
            cust[id].arriveTime=nod->custData->arriveTime;
            cust[id].custTime=nod->custData->custTime;

            if(arr[minTimeArr].currentTime!=0)
            {
                cust[id].waitCustTime= arr[minTimeArr].currentTime;
                arr[minTimeArr].queueWaitTime+=cust[id].waitCustTime;
            }
            else
            {
                cust[id].waitCustTime= 0;

            }

            cust[id].totalCustTime = nod->custData->custTime + cust[id].waitCustTime;


            enqueue(create(&cust[id]),&arr[minTimeArr]);

            arr[minTimeArr].currentTime =arr[minTimeArr].currentTime+ cust[id].custTime;
            arr[minTimeArr].totalQueueTime =arr[minTimeArr].totalQueueTime+ cust[id].custTime;

            deletet(nod);
            id++;
        }

        for(int x=0; x<3; x++)
        {
            SetColor(3);
            printf("\n Teller %d : ",x+1);
            SetColor(15);
            displayQueue(arr[x].first);
            printf("\n( Queue remaining Time = %d )\n===========\n",arr[x].currentTime);

        }
        SetColor(4);
        printf("\n==========================================\n\n Press Enter to Continue ..");
        SetColor(15);
        _getch();


        //system("cls");
        printf("\n");

        nod = search(time);
        if(nod!=NULL)
        {
            if(nod->custData->arriveTime != time)
            {
                for(int x=0; x<3; x++)
                    if(arr[x].currentTime>0)
                        arr[x].currentTime--;
                time++;
            }
        }
        else
        {
            for(int x=0; x<3; x++)
                if(arr[x].currentTime>0)
                    arr[x].currentTime--;
            time++;
        }

        //disP=disP->nextP;

        //if(disP!=NULL)
    }


}




void swap(customer* x,customer* y)
{
    customer temp= *x;
    *x=*y;
    *y=temp;
}
void bubbleSort()
{
    int sorted=0;
    node* sortP=firstP;

    int size=1,flag=0;
    for(int counter=0; counter<size&&!(sorted); counter++)
    {
        sorted=1;
        sortP=firstP;
        while(sortP->nextP!=NULL)
        {
            sorted=0;
            if(!flag)
                size++;

            if((sortP->custData->arriveTime) >= (sortP->nextP->custData->arriveTime))
                swap(sortP->custData,sortP->nextP->custData);

            sortP=sortP->nextP;

        }
        if(flag!=1)
            flag=1;

    }



}


void displayFinalResualt(queu * queArr, int* out)
{
    *out=1;
    int avg=0,waitAvg=0;
    for (int x=0; x<3; x++)
    {
        if(queArr[x].queueSize !=0)
        {
            avg=queArr[x].totalQueueTime/queArr[x].queueSize;
            waitAvg = queArr[x].queueWaitTime/queArr[x].queueSize;
            //printf("%d    %d    %d",avg ,queArr[x].totalQueueTime, queArr[x].queueSize);
        }
        else
        {
            avg =0;
        }

        printf("\033[0;32m");
        printf("\n\n_____________________________\nQueue No: %d\n_____________________________\n",x+1);
        \
        printf("\033[0m");
        displayQueueInternalData(queArr[x].first,1);

        printf("\033[0;34m");
        printf("Queue Total Transaction Time: %d\nQueue Average Transaction Time: %d\nQueue Total Wait Time: %d\nQueue Average Wait Time: %d",queArr[x].totalQueueTime,avg,queArr[x].queueWaitTime,waitAvg);
        printf("\033[0m");


    }


}



int main()

{
    int size =1;
    customer* cust = malloc(size*sizeof(customer));
    int custCounter=0;


    char selection[SIZE][30]= {"New Customer","Display List","Display Bank Tellers & Report","Exit"};
    char c_location=0,isExtend;
    char out =0;


    for(int idx=0; idx<SIZE; idx++)
    {
        if(idx==c_location)
        {
            SetColor(3);
        }
        else
        {
            SetColor(15);
        }
        gotoxy(50,idx+10);
        printf("%s",selection[idx]);
    }

    do
    {
        isExtend=_getch();

        if(isExtend == -32)
        {
            isExtend=_getch();
            if(isExtend==80)
            {
                c_location++;
                if(c_location>3)
                {
                    c_location=0;
                }
                system("cls");

                for(int idx=0; idx<SIZE; idx++)
                {
                    if(idx==c_location)
                    {
                        SetColor(3);
                    }
                    else
                    {
                        SetColor(15);
                    }
                    gotoxy(50,idx+10);
                    printf("%s",selection[idx]);
                }

            }
            if(isExtend==72)
            {
                c_location--;
                if(c_location<0)
                {
                    c_location=3;
                }
                system("cls");

                for(int idx=0; idx<SIZE; idx++)
                {
                    if(idx==c_location)
                    {
                        SetColor(3);
                    }
                    else
                    {
                        SetColor(15);
                    }
                    gotoxy(50,idx+10);
                    printf("%s",selection[idx]);
                }

            }
        }
        else

            if (isExtend == 13)
            {
                switch(c_location)
                {
                //new Employee
                case 0:
                    system("cls");
                    /////////////////////////////////////////////////
                    node * nod;
                    int x;

                    fflush(stdin);

                    do
                    {
                        printf("Enter customer arrival time :");
                        scanf("%d",&x);
                        cust[custCounter].arriveTime = x;
                        if(cust[custCounter].arriveTime<=0)
                        {
                            system("cls");
                            SetColor(4);
                            printf("Enter a Positive number...\n");
                            SetColor(15);
                        }
                    }
                    while(cust[custCounter].arriveTime<=0);

                    do
                    {
                        printf("\nEnter customer transaction time :");
                        scanf("%d",&x);
                        cust[custCounter].custTime = x;
                        if(cust[custCounter].custTime<=0)
                        {
                            system("cls");
                            SetColor(4);
                            printf("Enter a Positive number...\n");
                            SetColor(15);
                        }
                    }
                    while(cust[custCounter].custTime<=0);

                    nod = create(&cust[custCounter]);
                    add(nod);

                    custCounter++;

                    size++;
                    cust = extend(cust,size);
                    c_location=0;
                    break;
                case 1:
                    //display list
                    system("cls");
                    display();
                    c_location=1;
                    _getch();
                    break;
                case 2:
                    //Display tellers
                    system("cls");
                    queu* queArr= (queu *)(malloc(3*sizeof(queu)));
                    for (int x=0; x<3; x++)
                    {
                        queArr[x].first=NULL;
                        queArr[x].last=NULL;
                        queArr[x].currentTime=0;
                        queArr[x].totalQueueTime=0;
                        queArr[x].queueWaitTime=0;
                        queArr[x].queueSize=0;

                    }

                    moveCustomersToQueues(queArr);

                    displayFinalResualt(queArr,&out);


                    c_location=2;
                    _getch();
                    break;

                case 3:
                    out = 1;
                    break;
                }
                if(out !=1)
                {
                    system("cls");
                    for(int idx=0; idx<SIZE; idx++)
                    {
                        if(idx==c_location)
                        {
                            SetColor(3);
                        }
                        else
                        {
                            SetColor(15);
                        }
                        gotoxy(50,idx+10);
                        printf("%s",selection[idx]);
                    }
                }
            }
    }
    while(out !=1);

    printf("\n");

    SetColor(15);

}
