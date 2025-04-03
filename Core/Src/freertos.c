/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "rtc.h"
#include "st7735.h"
#include "gameTask.h"
#include "task2.h"
#include "task3.h"
#include "showTaskInfo.h"
#include "alarmConfigTask.h"
#include "queue.h"
#include "otherTasks.h"
#include "settingTask.h"

#include "tim.h"
#include "shareFunctions.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
void timerfordino_callback(TimerHandle_t xTimer);
void timerforbuzzer_callback(TimerHandle_t xTimer);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

RTC_TimeTypeDef AlarmTime; // 用于存储闹钟时间
uint8_t alarmEnabled = 0; // 闹钟启用标志
/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
//常驻任务
TaskHandle_t _keysoundTask_handle;

TaskHandle_t gameTask_handle;
TaskHandle_t menu_handle;
TaskHandle_t showTaskInfo_handle;
TaskHandle_t task3_handle;//receive task queue
TaskHandle_t alarmConfig_handle;
TaskHandle_t settingTask_handle;

QueueHandle_t Lcd_Semaphore_Handle;
QueueHandle_t Queue_Semaphore_Handle;

TimerHandle_t timerFordino;
TimerHandle_t timerForbuzzer;


/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  /* place for user code */
}
/* USER CODE END GET_TIMER_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  vSemaphoreCreateBinary(Lcd_Semaphore_Handle);
  Queue_Semaphore_Handle = xSemaphoreCreateBinary();
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 256);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  taskENTER_CRITICAL();
  timerFordino = xTimerCreate("timerFordino", 100, pdTRUE, (void *) 1, timerfordino_callback);
  timerForbuzzer = xTimerCreate("timerForbuzzer", 100, pdFALSE, (void *) 2, timerforbuzzer_callback);
  if(timerFordino == NULL) {
   printf("Timer creation failed\r\n"); 
  }
  xTaskCreate(gameTask, "gameTask", 128, NULL, 2, &gameTask_handle);
  xTaskCreate(menutask, "menutask", 128, NULL, 2, &menu_handle);
  xTaskCreate(task3, "task3", 256, NULL, 3, &task3_handle);
  xTaskCreate(alarmConfigTask, "alarmConfig", 128, NULL, 2, &alarmConfig_handle);
  xTaskCreate(settingTask, "settingTask", 128, NULL, 2, &settingTask_handle);
  //常驻任务
  xTaskCreate(_keysoundTask, "_keysoundTask", 128, NULL, 1, &_keysoundTask_handle);
  //xTaskCreate(showTaskInfo, "showTaskInfo", 128, NULL, 3, &showTaskInfo_handle);
  // 创建任务3
  BaseType_t xReturned = xTaskCreate(showTaskInfo, "showTaskInfo", 128, NULL, 3, &showTaskInfo_handle);
  if( xReturned != pdPASS ) {
    // 任务创建失败，打印错误信息
    printf("Task 3 creation failed! Error code: %d\r\n", xReturned);
  }
  //test
  // BaseType_t res = xTimerStart(timerFordino, 0);
  // if(res != pdPASS) {
  //   printf("Timer start failed\r\n");
  // }else{
  //   printf("Timer start success\r\n");
  // }
  //test
  vTaskSuspend(gameTask_handle);
  vTaskSuspend(menu_handle);
  vTaskSuspend(alarmConfig_handle);
  vTaskSuspend(settingTask_handle);
  vTaskDelete(NULL);
  taskEXIT_CRITICAL();
 
  //删除本任务
  
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
//外部中断回调函数
uint64_t pretime = 0;
uint8_t rightPress = 0;
extern uint8_t pointer;
extern uint8_t dino_state;
extern uint8_t keyPress;
extern uint8_t gamestatus;
extern uint8_t alarm_pointer;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  uint64_t nowtime;
  BaseType_t err;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  if(GPIO_Pin == GPIO_PIN_0) {
    //中断处理
    nowtime = HAL_GetTick();
    if(nowtime - pretime < 150)return;
    keyPress = 1;
    printf("pressed keyPress == %d\r\n",keyPress);
    err = xSemaphoreGiveFromISR(Lcd_Semaphore_Handle, &xHigherPriorityTaskWoken); // 释放信号量
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    if(err!= pdTRUE){
      printf("Key1 xSemaphoreGive failed\r\n");  
    }
    else{
      printf("Key1 xSemaphoreGive success\r\n"); 
    }

  }
  else if(GPIO_Pin == GPIO_PIN_1) {
    nowtime = HAL_GetTick();
    if(nowtime - pretime < 100)return;
    if(pointer < 5)pointer++;
    else pointer = 0;

    dino_state = 2;
    printf("pressed!!!!");
    alarm_pointer++;
    if(alarm_pointer == 4)alarm_pointer = 0;
    keyPress = 1;
    gamestatus = 0;
  }
  else if(GPIO_Pin == GPIO_PIN_2) {
    nowtime = HAL_GetTick();
    if(nowtime - pretime < 100)return;
      err = xSemaphoreGiveFromISR(Queue_Semaphore_Handle, &xHigherPriorityTaskWoken); // 释放信号量
      portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
      if(err!= pdTRUE){
        printf("Key2 xSemaphoreGive failed\r\n");  
      }
      else{
        printf("Key2 xSemaphoreGive success\r\n"); 
      }
  }

  pretime = nowtime;
  
}
extern float score;
void timerfordino_callback(TimerHandle_t xTimer){
  score += 0.1;
  //屏幕右上角显示分数
  char str[10];
  sprintf(str, "%.1f", score);
  ST7735_DrawString(80, 10, str,ST7735_YELLOW, ST7735_BLACK,&Font_7x10);
}
void timerforbuzzer_callback(TimerHandle_t xTimer){
  
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
  mypwmstop();
}
/* USER CODE END Application */

