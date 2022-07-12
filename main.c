
#include "pal.h"
#include "FreeRTOS.h"
#include "task.h"

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* Simple test task */
StaticTask_t xTaskBuffer;
StackType_t xStack[200];

void vTestTask(void* pvParameters) 
{
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main(void) 
{
    TaskHandle_t xHandle = NULL;

    vPalInit();

    /* Create the task without using any dynamic memory allocation. */
    xHandle = xTaskCreateStatic(
                    vTestTask,       /* Function that implements the task. */
                    "NAME",          /* Text name for the task. */
                    200,      /* Number of indexes in the xStack array. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    xStack,          /* Array to use as the task's stack. */
                    &xTaskBuffer );  /* Variable to hold the task's data structure. */

    configASSERT(xHandle == NULL);

    vTaskStartScheduler();

    while(1);
}