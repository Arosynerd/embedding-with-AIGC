#include "showTaskInfo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
//mydefine
#include "usart.h"

char task_info[500];
void showTaskInfo(void *pvParameters) {
    while(1){
        vTaskList(task_info);
        //printf("%s", task_info);
        //printf("========================================\r\n");
        vTaskDelay(2000);
    }

}

