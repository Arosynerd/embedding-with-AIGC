#include "screenTask.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
void screenTask(void *pvParameters)
{
    while (1)
    {
        printf("screenTask运行....\r\n");
        vTaskDelay(1000);
    }
}
