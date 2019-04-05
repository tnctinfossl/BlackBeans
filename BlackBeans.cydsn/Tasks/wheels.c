#include "wheels.h"
#include <FreeRTOS/FreeRTOS.h>
#include <assert.h>
#include <FreeRTOS/task.h>
#include <Driver/timer.h>
#include <stdlib.h>
static const size_t stack_depth=64;
static const UBaseType_t priority=3;
#define forever() for(;;)

void task_wheels_init(task_wheels_t* task,
    uint32_t interval_us,
    q1516_t (*encoder)(void*),void* encoder_object,
    q1516_t (*motor)(void*,int),void* motor_object){
    assert(task);        
    //assert(motor);
    //assert(encoder);
    
    task->interval_us=interval_us;
    task->encoder=encoder;
    task->encoder_object=encoder_object;
    task->motor=motor;
    task->motor_object=motor_object;
}
    
void task_wheels_lanch(task_wheels_t* task,const char* name){
    assert(task);
    xTaskCreate(task_wheels_process,name,stack_depth,task,priority,NULL);
}
int idx=0;
int logs[16];



void task_wheels_process(void* _task){
    assert(_task);
    
    task_wheels_t* task=_task; 
    portTickType last_tick=xTaskGetTickCount();
    const portTickType interval_tick = 100/portTICK_PERIOD_US;
    tick_t tick=profiling_timer_now();
    //uint32_t us;

    forever(){
       logs[idx=(idx+1)&0xf] = profiling_timer_duration_us(&tick);
       vTaskDelayUntil(&last_tick,interval_tick);
    }
}
    