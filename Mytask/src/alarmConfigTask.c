#include "alarmConfigTask.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"

#include "shareFunctions.h"
#include "ST7735.h"
#include "rtc.h"

#define AlarmTime_Y 50
#define AlarmHOURSUB1 5
#define AlarmHOURSUB2 44
#define BCD_TO_DEC(bcd) (((bcd) >> 4) * 10 + ((bcd) & 0x0F))
extern QueueHandle_t Lcd_Semaphore_Handle;
extern QueueHandle_t Queue_Semaphore_Handle;
extern TaskHandle_t task3_handle;
uint8_t alarm_pointer = 0;  

// Function prototype for callback
void callback(void);
void registerAlarmCallback(task_Def *taskdef);
RTC_AlarmTypeDef salarmTask = {
  .AlarmTime.Hours = 0,
  .AlarmTime.Minutes = 0,
  .AlarmTime.Seconds = 0,
  .Alarm = RTC_ALARM_A,
};
void alarmConfigTask(void *pvParameters)
{
    task_Def taskdef = {TASK_ERR_DEFAULT,TASK_COUNT_DEFAULT};

    //注册回调函数
    registerAlarmCallback(&taskdef);
    while (1)
    {
        handleSemaphore(&taskdef,task3_handle);
        char alarmBuffer[20];
        sprintf(alarmBuffer, "%02d", BCD_TO_DEC(salarmTask.AlarmTime.Hours));
        ST7735_DrawStringMe(0, AlarmTime_Y, alarmBuffer, ST7735_WHITE, ST7735_BLACK, &Font_14x28);
        ST7735_DrawString(30, AlarmTime_Y + 12, ":", ST7735_WHITE, ST7735_BLACK, &Font_11x18);
        sprintf(alarmBuffer, "%02d", BCD_TO_DEC(salarmTask.AlarmTime.Minutes));
        ST7735_DrawStringMe(40, AlarmTime_Y, alarmBuffer, ST7735_WHITE, ST7735_BLACK, &Font_14x28);
        ST7735_DrawString(70, AlarmTime_Y + 12, ":", ST7735_WHITE, ST7735_BLACK, &Font_11x18);
        sprintf(alarmBuffer, "%02d", BCD_TO_DEC(salarmTask.AlarmTime.Seconds));
        ST7735_DrawStringMe(80, AlarmTime_Y, alarmBuffer, ST7735_WHITE, ST7735_BLACK, &Font_14x28);
        switch (alarm_pointer)
        {
        case 0:
          ST7735_DrawString(AlarmHOURSUB1, AlarmTime_Y + 30, "s", ST7735_YELLOW, ST7735_BLACK, &Font_11x18);
          ST7735_Clean(AlarmHOURSUB2 + 14, AlarmTime_Y + 30,ST7735_WHITE, ST7735_BLACK, 11, 18);
          break;
        case 1:
          ST7735_DrawString(AlarmHOURSUB1 + 14, AlarmTime_Y + 30, "s", ST7735_YELLOW, ST7735_BLACK, &Font_11x18);
          ST7735_Clean(AlarmHOURSUB1, AlarmTime_Y + 30,ST7735_WHITE, ST7735_BLACK, 11, 18);
          break;
        case 2:
          ST7735_DrawString(AlarmHOURSUB2, AlarmTime_Y + 30, "s", ST7735_YELLOW, ST7735_BLACK, &Font_11x18);
          ST7735_Clean(AlarmHOURSUB1 + 14, AlarmTime_Y + 30,ST7735_WHITE, ST7735_BLACK, 11, 18);
          break;
        case 3:
          ST7735_DrawString(AlarmHOURSUB2 + 14, AlarmTime_Y + 30, "s", ST7735_YELLOW, ST7735_BLACK, &Font_11x18);
          ST7735_Clean(AlarmHOURSUB2, AlarmTime_Y + 30,ST7735_WHITE, ST7735_BLACK, 11, 18);
          break;
        default:
          break;
        }
        //闹钟调整
        taskdef.err = xSemaphoreTake(Queue_Semaphore_Handle,NULL);
        if(taskdef.err == pdTRUE){
          printf("execute!!!!\r\n");
            uint8_t temp;
            switch (alarm_pointer)
            {
            case 0:
              temp = (((salarmTask.AlarmTime.Hours & 0xF0) >> 4) + 1) << 4; 
              salarmTask.AlarmTime.Hours &= 0x0F;
              salarmTask.AlarmTime.Hours |= temp;
              break;
            case 1:
              temp = (salarmTask.AlarmTime.Hours & 0x0F) + 1;
              salarmTask.AlarmTime.Hours &= 0xF0;
              salarmTask.AlarmTime.Hours |= (temp % 10);
              break;
            case 2:
              temp = (((salarmTask.AlarmTime.Minutes & 0xF0) >> 4) + 1) << 4;
              salarmTask.AlarmTime.Minutes &= 0x0F;
              salarmTask.AlarmTime.Minutes |= temp;
              break;
            case 3:
              temp = (salarmTask.AlarmTime.Minutes & 0x0F) + 1;
              salarmTask.AlarmTime.Minutes &= 0xF0;
              salarmTask.AlarmTime.Minutes |= (temp % 10);
              break;
            default:
              break;
            }
        }
        else{
        }
        vTaskDelay(1000);
    }
}
void registerAlarmCallback(task_Def *taskdef) {
  if (taskdef != NULL) {
    taskdef->callback = callback;
  }
}
void callback(){
  setAlarmTime(salarmTask.AlarmTime.Hours,salarmTask.AlarmTime.Minutes,salarmTask.AlarmTime.Seconds);
  printf("alarm set success\r\n");
  printf("alarm time: %02d:%02d:%02d\r\n", BCD_TO_DEC(salarmTask.AlarmTime.Hours), BCD_TO_DEC(salarmTask.AlarmTime.Minutes), BCD_TO_DEC(salarmTask.AlarmTime.Seconds));
}
