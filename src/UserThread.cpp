#include <unistd.h>
#include "UserThread.h"
//#include <stdio.h>

void userThread_Resume(ThreadScheduler &schedule , int id)
{
    if(id < 0 || id >= schedule.max_index){
        return;
    }//judge max_index

    UserThread *t = &(schedule.threads[id]);

    if (t->state == READY ) {
        t->state = RUNNING ;//set running
        schedule.running_thread  = id ;

        swapcontext(&(schedule.main),&(t->ctx));
    }//run the thread
}

void userThread_Yield(ThreadScheduler &schedule,ThreadState state )
{
    //schedule.showThreadInfo();
    if(schedule.running_thread != -1 ){
        UserThread *t = &(schedule.threads[schedule.running_thread]);
        t->state = state;
        schedule.running_thread = -1;
        swapcontext(&(t->ctx),&(schedule.main));
        puts("main");
    }
}

void userThread_Body(ThreadScheduler *ps)
{
    int id = ps->running_thread;

    if(id != -1){
        UserThread *t = &(ps->threads[id]);

        t->func(t->arg);

        t->state = FREE;

        ps->running_thread = -1;
    }
}

int userThread_Create(ThreadScheduler &schedule,Function func,void *arg,int priority )
{

    int id = 0;

    for(id = 0; id < schedule.max_index; ++id ){
        if(schedule.threads[id].state == FREE){
            break;
        }
    }
    if (id == schedule.max_index) schedule.max_index++;
    UserThread *t = &(schedule.threads[id]);
    t->ID = id ;//1.thread id
    t->priority = priority ; //2.priority
    t->PID = getpid() ;//3.PID
    std::time_t now = std::time(nullptr);
    std::tm* local_time = std::localtime(&now);//4.get time
    t->create_second = now;
    t->create_time = local_time ;
    t->func = func;
    t->arg = arg;
    t->state = READY;
    getcontext(&(t->ctx));
    t->ctx.uc_stack.ss_sp = t->stack;
    t->ctx.uc_stack.ss_size = DEFAULT_STACK_SZIE;
    t->ctx.uc_stack.ss_flags = 0;
    t->ctx.uc_link = &(schedule.main);
    //schedule.running_thread = id;
    makecontext(&(t->ctx),(void (*)(void))(userThread_Body),1,&schedule);
    //std::cout<<"thread create finished the info is as follows:"<<std::endl;
    //t->helpShowThreadInfo();
    //swapcontext(&(schedule.main), &(t->ctx));
    return id;
}

int schedule_Finished(const ThreadScheduler &schedule)
{
    if (schedule.running_thread != -1){
        return 0;
    }else{
        for(int i = 0; i < schedule.max_index; ++i){
            if(schedule.threads[i].state != FREE){
                return 0;
            }
        }
    }

    return 1;
}
void RR(ThreadScheduler &schedule,int gaptime ){

    while(!schedule_Finished(schedule)){
        for (int j = 0; j < schedule.max_index; j++) {
            std::clock_t clock_start = std::clock();
            while (schedule.threads[j].state == READY) {
                userThread_Resume(schedule, j);
                //schedule.showThreadInfo();
                std::clock_t clock_end = std::clock();
                std::cout<<"gap"<<(clock_end - clock_start)<< std::endl;
                if (clock_end - clock_start > gaptime) {puts("break");break;}
            }
        }
    }
}
void PriorityS(ThreadScheduler &schedule){
    std::priority_queue<UserThread,std::vector<UserThread>,uthreadCMPPS> pq;//1.put data into priorityqueue
    for (int j = 0; j < schedule.max_index; j++) {

        if (schedule.threads[j].state == READY) {
            pq.push(schedule.threads[j]);

        }
    }
    while(!pq.empty())
    {
        UserThread  tmp= pq.top();
        int id = tmp.ID ;
        pq.pop();
        while(schedule.threads[id].state!=FREE) {
            userThread_Resume(schedule, id);
        }
    }
}
void FIFO(ThreadScheduler &schedule) {
    std::priority_queue<UserThread, std::vector<UserThread>, uthreadCMPPS2> pq;//1.put data into priorityqueue//1.put data into priorityqueue


    for (int j = 0; j < schedule.max_index; j++) {

        if (schedule.threads[j].state == READY) {
            pq.push(schedule.threads[j]);

        }
    }
    while (!pq.empty()) {
        UserThread tmp = pq.top();
        int id = tmp.ID;
        pq.pop();
        while (schedule.threads[id].state != FREE) {
            userThread_Resume(schedule, id);
        }
    }
}
