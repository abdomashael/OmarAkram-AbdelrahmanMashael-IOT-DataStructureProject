
struct customer
{
    //int id;
    int arriveTime,custTime,waitCustTime,totalCustTime;

};
typedef struct customer customer;

struct Node
{
    customer *custData;
    struct Node * nextP;
    struct Node * previousP ;

};
typedef struct Node node;

customer * extend(customer* p,int siz)
{
    customer * ptr = malloc(siz*sizeof(customer));

    for (int idx = 0; idx<siz-1; idx++)
    {
        ptr[idx].arriveTime= p[idx].arriveTime;
        ptr[idx].custTime= p[idx].custTime;
        ptr[idx].totalCustTime= p[idx].totalCustTime;
        ptr[idx].waitCustTime= p[idx].waitCustTime;
    }

    return ptr;
}

