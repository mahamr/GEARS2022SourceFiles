#include<stdio.h>
#include<stdlib.h>

void iRecurse(int*);

int main()
{
        printf("Hello, world!\n");

        int index = 15;

        iRecurse(&index);

        return 0;
}

void iRecurse(int* indexPtr)
{
        if(*indexPtr > 10)
        {
                printf("current value: %d\n", *indexPtr);
                (*indexPtr)--;
                printf("new value: %d\n", *indexPtr);
        }

        iRecurse(indexPtr);

        return;
}