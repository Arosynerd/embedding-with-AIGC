#include "task2.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"

#include "st7735.h"
#include "queue.h"
#include "shareFunctions.h"
extern QueueHandle_t Lcd_Semaphore_Handle;
extern TaskHandle_t alarmConfig_handle;
extern TaskHandle_t gameTask_handle;
extern TaskHandle_t settingTask_handle;
extern TaskHandle_t task3_handle;
uint8_t pointer = 0;
void menutask(void *pvParameters) {
  static uint8_t count = 0;
  static uint8_t status = 0;
  BaseType_t err;
  for (;;) {
    err = xSemaphoreTake(Lcd_Semaphore_Handle, NULL); // 获取信号量
    if(err == pdTRUE){
      count++;
      if(count == 2){
        count = 0;
        printf("2 times happened\r\n");
        switch (pointer)
        {
        case 0:
          giveandShift(alarmConfig_handle);
          break;
        case 1:
          // Handle case 1
          giveandShift(alarmConfig_handle);
          break;
        case 2:
          // Handle case 2
          break;
        case 3:
          // Handle case 3
          break;
        case 4:
          // Handle case 4
          giveandShift(gameTask_handle);
          break;
        case 5:
          // Handle case 5
          giveandShift(settingTask_handle);
          break;
        default:printf("err!!!");
          break;
        }
        
      }
      printf("count == %d\r\n", count);
    }
    if(pointer == 0){
      if(status == 0)ST7735_DrawString(10, 10, "Back", ST7735_WHITE, ST7735_BLACK, &Font_11x18);
      else ST7735_DrawString(10, 10, "Back", ST7735_BLACK, ST7735_WHITE, &Font_11x18);
      status = status == 0 ? 1 : 0;
    }else{
      ST7735_DrawString(10, 10, "Back", ST7735_WHITE, ST7735_BLACK, &Font_11x18);
    }
    if(pointer == 1){
      if(status == 0)ST7735_DrawString(10, 30, "Alarm", ST7735_WHITE, ST7735_BLACK, &Font_11x18);
      else ST7735_DrawString(10, 30, "Alarm", ST7735_BLACK, ST7735_WHITE, &Font_11x18);
      status = status == 0? 1 : 0; 
    }else{
      ST7735_DrawString(10, 30, "Alarm", ST7735_WHITE, ST7735_BLACK, &Font_11x18);
    }
    if(pointer == 2){
      if(status == 0)ST7735_DrawString(10, 50, "Counter", ST7735_WHITE, ST7735_BLACK, &Font_11x18);
      else ST7735_DrawString(10, 50, "Counter", ST7735_BLACK, ST7735_WHITE, &Font_11x18);
      status = status == 0 ? 1 : 0;
    }else{
      ST7735_DrawString(10, 50, "Counter", ST7735_WHITE, ST7735_BLACK, &Font_11x18);
    }
    if(pointer == 3){
      if(status == 0)ST7735_DrawString(10, 70, "offCounter", ST7735_WHITE, ST7735_BLACK, &Font_11x18);
      else ST7735_DrawString(10, 70, "offCounter", ST7735_BLACK, ST7735_WHITE, &Font_11x18);
      status = status == 0 ? 1 : 0;
    }else{
      ST7735_DrawString(10, 70, "offCounter", ST7735_WHITE, ST7735_BLACK, &Font_11x18);
    }
    if(pointer == 4){
      if(status == 0)ST7735_DrawString(10, 90, "DinoGame", ST7735_WHITE, ST7735_BLACK, &Font_11x18);
      else ST7735_DrawString(10, 90, "DinoGame", ST7735_BLACK, ST7735_WHITE, &Font_11x18);
      status = status == 0 ? 1 : 0;
    }else{
      ST7735_DrawString(10, 90, "DinoGame", ST7735_WHITE, ST7735_BLACK, &Font_11x18);
    }
    if(pointer == 5){
      if(status == 0)ST7735_DrawString(10, 110, "Settings", ST7735_WHITE, ST7735_BLACK, &Font_11x18);
      else ST7735_DrawString(10, 110, "Settings", ST7735_BLACK, ST7735_WHITE, &Font_11x18);
      status = status == 0 ? 1 : 0;
    }else{
      ST7735_DrawString(10, 110, "Settings", ST7735_WHITE, ST7735_BLACK, &Font_11x18);
    }
    vTaskDelay(800);
  }
}

