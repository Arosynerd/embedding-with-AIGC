#include "task1.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
void task1(void *pvParameters)
{
    while (1)
    {
        printf("task1运行....\r\n");
        vTaskDelay(1000);
    }
}
