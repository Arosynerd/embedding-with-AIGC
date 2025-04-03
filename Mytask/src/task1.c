#include "gameTask.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"

#include "st7735.h"
#include "fonts.h"
#include "shareFunctions.h"
#define DINO_X (10 + 48)

extern TimerHandle_t timerFordino;
extern TaskHandle_t task3_handle;
extern QueueHandle_t Lcd_Semaphore_Handle;
float score = 0.0;
uint16_t arr[3] = {80, 70, 80};
uint16_t arrDelay[3] = {100, 300, 100};
uint8_t dino_state = 0; // 恐龙的状态，0为正常，1为跳跃
uint8_t gamestatus = 0;
void gameTask(void *pvParameters)
{

    static uint8_t block_x = 90; // 障碍物的初始x坐标
    static uint8_t dino_y = 100; // 障碍物的初始y坐标
    static uint8_t dino_state_state = 0;
    static uint8_t count = 0;
    BaseType_t err;
    // 开启定时器
    err = xTimerStart(timerFordino, 0);
    if (err == NULL)
        printf("timer start failed\r\n");
    while (1)
    {
        if (gamestatus == 0)
        {
            if (dino_state == 0)
            {
                ST7735_Draw(10, dino_y, ST7735_WHITE, ST7735_BLACK, &Dino1_32x32);
                dino_state = 1;
            }
            else if (dino_state == 1)
            {
                ST7735_Draw(10, dino_y, ST7735_WHITE, ST7735_BLACK, &Dino2_32x32);
                dino_state = 0;
            }
            else if (dino_state == 2)
            {
                if (dino_state_state == 0)
                {
                    ST7735_Clean(10, dino_y, ST7735_WHITE, ST7735_BLACK, 32, 32);
                    ST7735_Draw(10, dino_y -= 50, ST7735_WHITE, ST7735_BLACK, &Dino1_32x32); // 飞空
                }
                else if (dino_state_state <= 3)
                    ; // 滞空
                else if (dino_state_state == 4)
                { // 下来
                    ST7735_Clean(10, dino_y, ST7735_WHITE, ST7735_BLACK, 32, 32);
                    ST7735_Draw(10, dino_y += 50, ST7735_WHITE, ST7735_BLACK, &Dino2_32x32);
                    dino_state_state = 0; // 复位
                    dino_state = 0;
                }
                if (dino_state != 0)
                    dino_state_state++;
            }

            // vTaskDelay(500);
            // ST7735_Draw(10, 100, ST7735_WHITE, ST7735_BLACK, &Dino2_32x32);
            // vTaskDelay(500);
            // test
            // ST7735_Draw(10, 100, ST7735_WHITE, ST7735_BLACK, &Dino1_32x32);
            // vTaskDelay(100);
            // ST7735_Clean(10, 100, ST7735_WHITE, ST7735_BLACK, 32, 32);
            // ST7735_Draw(10, 80, ST7735_WHITE, ST7735_BLACK, &Dino2_32x32);
            // vTaskDelay(100);
            // ST7735_Clean(10, 80, ST7735_WHITE, ST7735_BLACK, 32, 32);
            // ST7735_Draw(10, 70, ST7735_WHITE, ST7735_BLACK, &Dino1_32x32);
            // vTaskDelay(300);
            // //下落
            // ST7735_Clean(10, 70, ST7735_WHITE, ST7735_BLACK, 32, 32);
            // ST7735_Draw(10, 80, ST7735_WHITE, ST7735_BLACK, &Dino2_32x32);
            // vTaskDelay(100);
            // ST7735_Clean(10, 80, ST7735_WHITE, ST7735_BLACK, 32, 32);
            // ST7735_Draw(10, 90, ST7735_WHITE, ST7735_BLACK, &Dino1_32x32);
            // vTaskDelay(100);
            // ST7735_Clean(10, 90, ST7735_WHITE, ST7735_BLACK, 32, 32);
            // ST7735_Draw(10, 100, ST7735_WHITE, ST7735_BLACK, &Dino2_32x32);
            // vTaskDelay(100);

            // test

            // 绘制障碍物
            // test block
            ST7735_Draw(block_x -= 10, 100, ST7735_WHITE, ST7735_BLACK, &Block_32x32);
            if (block_x == 50)
            {
                if (dino_y == 100)
                {
                    printf("you lose game!!!\r\n");
                    err = xTimerStop(timerFordino, 0);
                    if (err == NULL)
                    {
                        printf("timer stop failed\r\n");
                    }
                    else
                    {
                        // 中心输出分数
                        char str[10];
                        sprintf(str, "YOUR SCORE :%.1f", score);
                        ST7735_DrawString(80, 10, str, ST7735_YELLOW, ST7735_BLACK, &Font_7x10);
                        gamestatus = 1;
                    }
                }
            }
            if (block_x == 0)
            {
                block_x = 90;
                ST7735_Clean(0, 100, ST7735_WHITE, ST7735_BLACK, 32, 32);
            }
        }
        else{

        }
        err = xSemaphoreTake(Lcd_Semaphore_Handle, NULL); // 获取信号量
        if(err == pdTRUE){
          count++;
          if(count == 2){
            count = 0;
            printf("2 times happened\r\n");
            giveandShift(task3_handle);
          }
          printf("count == %d\r\n", count);
        }
        else{
        //   printf("dinotask Semaphore not taken\r\n");
        }
        vTaskDelay(500);
        printf("dino_y == %d\r\n", dino_y);
        // test blcok
    }
}
// 32*32
