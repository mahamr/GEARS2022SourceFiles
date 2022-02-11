#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>

void DoSomethingA(long);
void DoSomethingB(long);
void DoSomethingC(long);

int main()
{
        printf("Hello, world!\n");
        printf("PID: %d\n", getpid());

        time_t startTime = time(NULL);
        time_t endTime;

        srandom(time(NULL));

        int loopCount = 0;
        do
        {
                long randNum = random();
                loopCount++;

                int i;
                for( i=0; i<10000; i++ )
                {
                        if( i%2 == 0)
                                DoSomethingB(randNum);
                        else
                                DoSomethingA(randNum);

                        if( i%3 == 0)
                                DoSomethingC(randNum);
                }

                endTime = time(NULL);

        }while( (int) difftime(endTime, startTime) < 20 );

        printf("took %.0f seconds with %d loops\n", difftime(endTime, startTime), loopCount);

        return 0;
}

void DoSomethingA(long rando)
{
        int i;
        for( i=0; i<2000; i++ )
        {
                rando * rando * 123456 / 4 * i;
        }
}

void DoSomethingB(long rando)
{
        int i;
        for( i=0; i<250; i++ )
        {
                i * rando * 845836565633 + (rando * 89345732867 / 80456874580);
        }
}

void DoSomethingC(long rando)
{
        int i;
        for( i=0; i<1500; i++ )
        {
                i * rando * rando / 123456 * rando * rando;
        }
}