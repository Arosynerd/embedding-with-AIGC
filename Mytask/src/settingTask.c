#include "settingTask.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"

#include "shareFunctions.h"
#include "st7735.h"
extern TaskHandle_t task3_handle;
extern QueueHandle_t Queue_Semaphore_Handle;
settings_Def setting = {
    .buzzer = OFF,
};
void settingTask(void *pvParameters)
{
    task_Def taskdef = {TASK_ERR_DEFAULT,TASK_COUNT_DEFAULT};
    while (1)
    {
        printf("settingTask运行....\r\n");
        handleSemaphore(&taskdef,task3_handle);
        ST7735_DrawString(5, 10, "Buzzer sound", ST7735_WHITE, ST7735_BLACK,&Font_7x10);
        if(setting.buzzer == ON)
        ST7735_DrawString(100,10,"NO",ST7735_GREEN,ST7735_BLACK,&Font_7x10);
        else
        ST7735_DrawString(100,10,"OFF",ST7735_RED,ST7735_BLACK,&Font_7x10);
        taskdef.err = xSemaphoreTake(Queue_Semaphore_Handle,NULL);
        if(taskdef.err == pdTRUE){
            if(setting.buzzer == ON){
            setting.buzzer = OFF;
            ST7735_Clean(100,10,ST7735_WHITE,ST7735_BLACK,21,10);
            ST7735_DrawString(100,10,"OFF",ST7735_RED,ST7735_BLACK,&Font_7x10);
            }else{
            setting.buzzer = ON;
            ST7735_Clean(100,10,ST7735_WHITE,ST7735_BLACK,21,10);
            ST7735_DrawString(100,10,"ON",ST7735_GREEN,ST7735_BLACK,&Font_7x10);
            }
        }
        else{
            // printf("settingTask 信号量获取失败\r\n");
        }
        vTaskDelay(1000);
    }
}
