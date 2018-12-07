#include <TaskScheduler.h>
#include <LineSensor.h>
#include <Chassis.h>
#include <BumperSensor.h>
#include <SoundDetection.h>
#include <ConcurrentBumperApp.h>

struct TaskContext_str TASK_List[NR_OF_TASKS] = {
    {CHASSIS_REQ, 3, 0, ChassisTask_Run},
    {1, 0, 0, SoundDetTask_Run},
    {10, 1, 0, BumperTask_Run},
    {25, 25, 0, ConcurrentBumperApp_Run},
    {100, 2, 0, LineSensorTask_Run}};

void TaskScheduler(void)
{
    for (int taskNr = 0; taskNr < NR_OF_TASKS; taskNr++)
    {
        if (--TASK_List[taskNr].ReqCnt <= 0)
        {
            TASK_List[taskNr].execute();
            TASK_List[taskNr].ReqCnt = TASK_List[taskNr].REQURENCY;
        }
    }
}