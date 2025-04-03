#include "shareFunctions.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "st7735.h"
#include "queue.h"

#include "tim.h"

extern QueueHandle_t Lcd_Semaphore_Handle;

TIM_OC_InitTypeDef sConfigOC = {
  .OCMode = TIM_OCMODE_PWM1,
  .Pulse = 1,
  .OCPolarity = TIM_OCPOLARITY_LOW,
  .OCNPolarity = TIM_OCNPOLARITY_HIGH,
  .OCFastMode = TIM_OCFAST_DISABLE,
  .OCIdleState = TIM_OCIDLESTATE_RESET,
  .OCNIdleState = TIM_OCNIDLESTATE_RESET
};

void semaphoregive(void);
void giveandShift(TaskHandle_t task)
{
  semaphoregive();
  // 清屏
  ST7735_FillScreen(ST7735_BLACK);
  vTaskResume(task);
  vTaskSuspend(NULL);
}
void semaphoregive(void)
{
  BaseType_t err;
  err = xSemaphoreGive(Lcd_Semaphore_Handle); // 释放信号量
  if (err != pdTRUE)
  {
    printf("task xSemaphoreGive failed\r\n");
  }
  else
  {
    printf("task xSemaphoreGive success\r\n");
  }
}
void mypwmstart(void)
{
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
}
void mypwmstop(void){
  sConfigOC.OCMode = TIM_OCMODE_FORCED_INACTIVE;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
}
void handleSemaphore(task_Def *task, TaskHandle_t taskhandle)
{
  task->err = xSemaphoreTake(Lcd_Semaphore_Handle, NULL); // 获取信号量
  if (task->err == pdTRUE)
  {
    task->count++;
    if (task->count == 2)
    {
      task->count = 0;
      printf("2 times happened\r\n");
      if(task->callback != NULL){//回调函数不为空
        task->callback(); 
        //reset
        task->callback = NULL;
        printf("callback executed++++++++++++++\r\n");
       }
       else{
       }
      giveandShift(taskhandle);
      
    }
    printf("count == %d\r\n", task->count);
  }
  else
  {
    printf("task Semaphore not taken\r\n");
  }

  
}
void setAlarmTime(uint8_t hour, uint8_t minute, uint8_t second)
{
  RTC_AlarmTypeDef sAlarm = {0};

  sAlarm.AlarmTime.Hours = hour;
  sAlarm.AlarmTime.Minutes = minute;
  sAlarm.AlarmTime.Seconds = second; /* 设置下次闹钟提醒时间是当前时间的3s之后 */
  sAlarm.Alarm = RTC_ALARM_A;

  // 再次启动闹钟中断事件
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

  printf("设置闹钟时间为：%02X:%02X:%02X\n", hour, minute, second);
}

