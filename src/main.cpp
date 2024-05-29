#include "UserThread.h"
#include <stdio.h>
#include<unistd.h>
void F1(void * arg)
{
    printf("%s\n","1");
    printf("%s\n","11");
    printf("%s\n","111");
    printf("%s\n","1111");

}

void F2(void * arg)
{

    for(int i = 0 ;i<2;i++) {

        printf("%s\n", "2");
        userThread_Yield(*(ThreadScheduler *) arg);
    }

}

void F3(void *arg)
{

    for(int i = 0 ;i<2;i++) {

        printf("%s\n", "3");
        userThread_Yield(*(ThreadScheduler *) arg);
    }

}

void Test2()
{
    ThreadScheduler s;

    int id1 = userThread_Create(s,F2,&s);
    int id2 = userThread_Create(s,F3,&s);
    s.showThreadInfo();
    while(!schedule_Finished(s)){
        userThread_Resume(s,id2);
        userThread_Resume(s,id1);
    }
    printf("main function finished");

}

void Test3_2()
{
    ThreadScheduler s;

    int id1 = userThread_Create(s,F2,&s);
    int id2 = userThread_Create(s,F3,&s);
    //s.showThreadInfo();
    while(!schedule_Finished(s)){
        s.blockThread(id1);
        //s.showThreadInfo();
        userThread_Resume(s,id1);
        userThread_Resume(s,id2);
        s.noBlockThread(id1);
        //s.showThreadInfo();
        userThread_Resume(s,id1);

    }
    printf("main function finished");

}
void Test3_1()
{
    ThreadScheduler s;

    int id1 = userThread_Create(s,F2,&s);
    int id2 = userThread_Create(s,F3,&s);
    s.showThreadInfo();
    while(!schedule_Finished(s)){
        userThread_Resume(s,id2);
        userThread_Resume(s,id1);
    }
    printf("main function finished");

}
void Test4_1()
{
    ThreadScheduler s;

    int id1 = userThread_Create(s,F2,&s);
    int id2 = userThread_Create(s,F3,&s);
    //s.showThreadInfo();
//    while(!schedule_Finished(s)){
//        userThread_Resume(s,id1);
//        userThread_Resume(s,id2);
//    }
    RR(s,10);
    printf("main function finished");

}
void Test4_2()
{
    ThreadScheduler s;

    int id1 = userThread_Create(s,F2,&s,1);
    int id2 = userThread_Create(s,F3,&s,2);
    //s.showThreadInfo();
//    while(!schedule_Finished(s)){
//        userThread_Resume(s,id1);
//        userThread_Resume(s,id2);
//    }
    PriorityS(s);
    printf("main function finished");

}
void Test4_3()
{
    ThreadScheduler s;
    int id1 = userThread_Create(s,F2,&s,1);
    int id2 = userThread_Create(s,F3,&s,2);
    FIFO(s);
    printf("main function finished");
}
void Test5()
{
    ThreadScheduler s;
    int id1 = userThread_Create(s,F2,&s,1);
    int id2 = userThread_Create(s,F3,&s,2);
    s.deleteThread(id1);
    s.showThreadInfo();
    FIFO(s);
    printf("main function finished");
}
//void timetest(){
//
////    std::time_t now = std::time(nullptr);
////    std::tm* local_time = std::localtime(&now);//4.get time
////    std::tm* local_time2= std::localtime(&now);//4.
////    std::cout<<local_time-><<endl
//}
int main()
{

//    context_test();
//    printf("----------------");
    //Test2();
//   Test3_1();
//    Test3_2();
//    Test4_1();
//    Test4_2();
//   Test4_3();
    Test5();

    return 0;
}