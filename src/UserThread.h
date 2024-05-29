#include<iostream>
#include <ucontext.h>
#include <vector>
#include <ctime>
#include <ctime>
#include <stdio.h>
#include<queue>
#include<vector>
#define DEFAULT_STACK_SZIE (1024*128)
#define MAX_UTHREAD_SIZE   1024
enum ThreadState{FREE,RUNNING ,READY,BLOCKED};
struct ThreadSchedule;
typedef void (*Function)(void *arg);

typedef struct UserThread
{
    //1.thread ID
    int ID ;
    //2.priority
    int priority;

    // PTD getpid();
    pid_t PID ;
    //4.createtime
    // 获取当前时间的 Unix 时间戳
    std::time_t create_second ;//= std::time(nullptr);

    // 5.将 Unix 时间戳转换为本地时间
    std::tm* create_time ;//= std::localtime(&now);
    //
    ucontext_t ctx;
    Function func;
    void *arg;
    //6.thread state
    enum ThreadState state;
    char stack[DEFAULT_STACK_SZIE];
    void helpShowThreadInfo(){
        std::cout <<"Thread ID"<<ID<< std::endl ;
        std::cout <<"priority"<<priority<<std::endl ;
        std::cout <<"Process ID"<<PID<<std::endl ;
        // 获取年、月、日、时、分、秒等信息
        int year = create_time->tm_year + 1900;
        int month = create_time->tm_mon + 1;
        int day = create_time->tm_mday;
        int hour = create_time->tm_hour;
        int minute = create_time->tm_min;
        int second = create_time->tm_sec;
        // 输出当前时间
        std::cout << "Create time: " << year << "-" << month << "-" << day << " "
                  << hour << ":" << minute << ":" << second << std::endl;
        std::cout <<"State"<<state<<std::endl ;

    }
    void helpSetPriority(int pr= 1)
    {
        this->priority = pr;
    }

}UserThread;

typedef struct ThreadSchedule
{
    ucontext_t main;
    int running_thread;
    UserThread *threads;
    int max_index; // 曾经使用到的最大的index + 1

    ThreadSchedule():running_thread(-1), max_index(0) {
        threads = new UserThread[MAX_UTHREAD_SIZE];
        for (int i = 0; i < MAX_UTHREAD_SIZE; i++) {
            threads[i].state = FREE;
        }
    }
//    void setThreadState(int ThreadID ,ThreadState st)
//    {
//        this->threads[ThreadID].state =st ;
//
//    }
    void blockThread(int ThreadID)
    {
        if(this->threads[ThreadID].state!=FREE)
        {
            this->threads[ThreadID].state = BLOCKED ;

        }
        
    }
    void noBlockThread(int ThreadID)
    {
        if(this->threads[ThreadID].state==BLOCKED)
        {
            this->threads[ThreadID].state = READY;

        }

    }
    void showThreadInfo(){
        std::cout<<"the all threads info is as follows:"<<std::endl;
        for (int i = 0 ; i <max_index ; i++ )
        {
            if (threads[i].state!=FREE)
            {
                threads[i].helpShowThreadInfo() ;

            }

        }


    }
    void setPriority(int id ,int pr= 1)
    {
        this->threads[id].helpSetPriority(pr) ;
    }
    void deleteThread(int id)
    {
        if(this->running_thread == id){std::cerr<<"the running thread is not allowed to be deleted !" ;}
        else
        {
            this->threads[id].state =FREE ;
        }
//        this->showThreadInfo();
    }
    ~ThreadSchedule() {
        delete [] threads;
    }
}ThreadScheduler;

/*Create a user's thread
*        return the index of the created thread in schedule
*/
int  userThread_Create(ThreadScheduler &schedule,Function func,void *arg,int priority =  0);

/* yield */
void userThread_Yield(ThreadScheduler &schedule,ThreadState state = READY);

/* resume the thread which index equal id*/
void userThread_Resume(ThreadScheduler &schedule,int id);

int  schedule_Finished(const ThreadScheduler &schedule);

//compare priority
struct uthreadCMPPS{
    bool operator()(const UserThread t1, const UserThread t2) const {
        return t1.priority < t2.priority;
    }
};
//compare priority
struct uthreadCMPPS2{
    bool operator()(const UserThread t1, const UserThread t2) const {
        return t1.create_second> t1.create_second;
    }
};
void RR(ThreadScheduler &schedule,int gaptime = 10);
void PriorityS(ThreadScheduler &schedule);
void FIFO(ThreadScheduler &schedule);




