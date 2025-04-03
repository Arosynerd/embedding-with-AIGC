#include "otherTasks.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "tim.h"

#include "shareFunctions.h"
extern TimerHandle_t timerForbuzzer;
uint8_t keyPress = 0;
void otherTasks(void *pvParameters)
{
    while (1)
    {
        printf("otherTasks运行....\r\n");
        vTaskDelay(1000);
    }
}

extern settings_Def setting;
void _keysoundTask(void *pvParameters)
{
    BaseType_t err;
    while (1)
    {
        if (setting.buzzer == ON)
        {
            if (keyPress == 1)
            {
                keyPress = 0;
                mypwmstart();
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
                err = xTimerStart(timerForbuzzer, 0);
                if (err == NULL)
                    printf("timer start failed\r\n");
            }
        }
    }
}
