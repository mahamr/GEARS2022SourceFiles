#include<stdio.h>
#include<stdlib.h>
#include<time.h>

char* allocMem1(long);
char* allocMem2(long);
void deallocMem(char*);
void fillItUp(long, char*);

int main()
{
        printf("Hello, world!\n");

        srand((unsigned int) time(NULL));

        int i;

        for(i=0 ; i<50 ; i++)
        {
                long randNum = (rand() * sizeof(char)) % 512;

                char *memBlock;

                if(i%2 == 0 && i%3 == 0)
                {
                        memBlock = allocMem1(randNum);
                        fillItUp(randNum, memBlock);
                        continue;
                }
                else
                {
                        memBlock = allocMem2(randNum);
                        fillItUp(randNum, memBlock);
                }
                deallocMem(memBlock);
        }
        return 0;
}

char * allocMem1(long size)
{
        return (char *) malloc(size);
}

char * allocMem2(long size)
{
        return (char *) malloc(size);
}

void deallocMem(char * ptrToMemBlock)
{
        free(ptrToMemBlock);
        return;
}

void fillItUp(long howManyBytes, char * theBlock)
{
        int i;
        for(i=0 ; i<howManyBytes; i++)
        {
                theBlock[i] = 'z';
        }
}