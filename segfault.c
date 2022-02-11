#include<stdio.h>
#include<stdlib.h>

void CrashNow(int*);

int main()
{
        printf("Hello, world!\n");
        int intTheAnswer;
        intTheAnswer = 42;

        int* ptrTheAnswer;
        ptrTheAnswer = &intTheAnswer;

        printf("Goodbye, cruel world!\n");
        CrashNow(ptrTheAnswer);

        return 0;

}

void CrashNow(int* theAnswer)
{
        // change the answer to something else
        theAnswer = 0; // BUGBUG: there's a typo here..
        printf("The answer is %d\n", *theAnswer);
        return;
}