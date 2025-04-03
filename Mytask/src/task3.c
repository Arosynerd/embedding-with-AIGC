#include "task3.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"

// Function declaration for setAlarmTime
void setAlarmTime(uint8_t hour, uint8_t minute, uint8_t second);
void Give(void);
#include "st7735.h"
#include "rtc.h"
#include "stimage.h"
#include "queue.h"

RTC_DateTypeDef GetData;

RTC_TimeTypeDef GetTime;
#define NowTime_X 70
extern QueueHandle_t Lcd_Semaphore_Handle;
extern TaskHandle_t menu_handle;
extern TaskHandle_t task3_handle;
const st_image_t *img = NULL;

void task3(void *pvParameters)
{
  /* USER CODE BEGIN RTC_Task */
  static uint8_t count = 0;
  BaseType_t err;
  ST7735_FillScreen(ST7735_BLACK);
  HAL_Delay(2000);
  // test begin
  img = &icon_weather_duoyun;
  // test end
  /* Infinite loop */
  setAlarmTime(0x12, 0x01, 0x59);
  for (;;)
  {
    osDelay(1000);
    vPortEnterCritical(); // 进入临界区
    err = xSemaphoreTake(Lcd_Semaphore_Handle, NULL); // 获取信号量
    if (err == pdTRUE)
    {
      count++;
      //printf("task33333 xSemaphoreTake success\r\n");
      if (count == 2)
      {
        count = 0;
        printf("2 times happened\r\n");
        Give();
        // 清屏
        ST7735_FillScreen(ST7735_BLACK);
        vTaskResume(menu_handle);
        vTaskSuspend(NULL);
        
      }
    }
    else
    {
      //printf("task33333 xSemaphoreTake failed\r\n");
    }
    vPortExitCritical(); // 退出临界区
    /* Get the RTC current Time */
    // HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
    /* Get the RTC current Date */
    HAL_RTC_GetDate(&hrtc, &GetData, RTC_FORMAT_BIN);

    /* Get the RTC current Time */
    HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
    // Display in LCD
    // ST7735_DrawString(0, 0, "Time:", ST7735_WHITE, ST7735_BLACK, &Font_11x18);
    char buffer[20];
    sprintf(buffer, "%02d/%02d/%02d", 2000 + GetData.Year, GetData.Month, GetData.Date);
    ST7735_DrawString(0, 5, buffer, ST7735_WHITE, ST7735_BLACK, &Font_7x10);

    ST7735_DrawImage(0, 16, img->width, img->height, img->data);
    // Display fixed temperature and humidity
    sprintf(buffer, "25");
    ST7735_DrawString(50, 26, buffer, ST7735_WHITE, ST7735_BLACK, &Font_11x18);
    ST7735_DrawString(70, 26, "C", ST7735_WHITE, ST7735_BLACK, &Font_7x10);

    sprintf(buffer, "60");
    ST7735_DrawString(50, 46, buffer, ST7735_WHITE, ST7735_BLACK, &Font_11x18);
    ST7735_DrawString(70, 46, "%", ST7735_WHITE, ST7735_BLACK, &Font_7x10);

    sprintf(buffer, "26");
    ST7735_DrawString(85, 26, buffer, ST7735_WHITE, ST7735_BLACK, &Font_11x18);
    ST7735_DrawString(105, 26, "C", ST7735_WHITE, ST7735_BLACK, &Font_7x10);

    sprintf(buffer, "66");
    ST7735_DrawString(85, 46, buffer, ST7735_WHITE, ST7735_BLACK, &Font_11x18);
    ST7735_DrawString(105, 46, "%", ST7735_WHITE, ST7735_BLACK, &Font_7x10);
    //test
    printf("I am fucking here!!\r\n");
    //test
    sprintf(buffer, "%02d", GetTime.Hours);
    ST7735_DrawStringMe(0, NowTime_X, buffer, ST7735_WHITE, ST7735_BLACK, &Font_14x28);
    ST7735_DrawString(30, 82, ":", ST7735_WHITE, ST7735_BLACK, &Font_11x18);

    sprintf(buffer, "%02d", GetTime.Minutes);
    ST7735_DrawStringMe(40, NowTime_X, buffer, ST7735_WHITE, ST7735_BLACK, &Font_14x28);
    ST7735_DrawString(70, 82, ":", ST7735_WHITE, ST7735_BLACK, &Font_11x18);

    sprintf(buffer, "%02d", GetTime.Seconds);
    ST7735_DrawStringMe(80, NowTime_X, buffer, ST7735_WHITE, ST7735_BLACK, &Font_14x28);
  }
  /* USER CODE END RTC_Task */
}

// // test
// void setAlarmTime(uint8_t hour, uint8_t minute, uint8_t second)
// {
//   RTC_AlarmTypeDef sAlarm = {0};

//   sAlarm.AlarmTime.Hours = hour;
//   sAlarm.AlarmTime.Minutes = minute;
//   sAlarm.AlarmTime.Seconds = second; /* 设置下次闹钟提醒时间是当前时间的3s之后 */
//   sAlarm.Alarm = RTC_ALARM_A;

//   // 再次启动闹钟中断事件
//   if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
//   {
//     Error_Handler();
//   }

//   printf("设置闹钟时间为：%02X:%02X:%02X\n", hour, minute, second);
// }
// // test

void Give(void)
{
  BaseType_t err;
  err = xSemaphoreGive(Lcd_Semaphore_Handle); // 释放信号量
  if (err != pdTRUE)
  {
    printf("task3 xSemaphoreGive failed\r\n");
  }
  else
  {
    printf("task3 xSemaphoreGive success\r\n");
  }
}
