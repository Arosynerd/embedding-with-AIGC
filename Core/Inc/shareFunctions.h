#ifndef SHAREFUNCTIONS_H
#define SHAREFUNCTIONS_H
#include "FreeRTOS.h"
#include "task.h"
#include "rtc.h"
#define TASK_COUNT_DEFAULT 0
#define TASK_ERR_DEFAULT 0
#define ON 1
#define OFF 0
typedef struct {
    BaseType_t err;
    uint8_t count;
    void (*callback)(void);
} task_Def;
typedef struct {
    uint8_t buzzer;
    // uint8_t setting2;
    // uint16_t setting3;
    // float setting4;
} settings_Def;
void giveandShift(TaskHandle_t task);
void handleSemaphore(task_Def *task,TaskHandle_t taskhandle);
void mypwmstart(void);
void mypwmstop(void);
void setAlarmTime(uint8_t hour, uint8_t minute, uint8_t second);



#endif // SHAREFUNCTIONS_H


