#include "counterTask.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
void counterTask(void *pvParameters)
{
    while (1)
    {
        printf("counterTask运行....\r\n");
        vTaskDelay(1000);
    }
}
