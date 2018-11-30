#ifndef TASKSCHEDULER_H_
#define TASKSCHEDULER_H_

#include <RobotDefinitions.h>

struct TaskContext_str
{
    int REQURENCY;
    int OFFSET;
    int ReqCnt;
    void (*execute)(void);
};

extern struct TaskContext_str TASK_List[NR_OF_TASKS];

void TaskScheduler(void);

#endif