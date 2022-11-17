/*
 * tasks.c
 *
 *  Created on: Oct 20, 2022
 *      Author: nog8370
 */

#include "tasks.h"
#include "display.h"
#include "cmsis_os.h"
#include "main.h"
#include "stdbool.h"



void DisplayTask() {
    LCD_Init();
    LCD_ClearDisplay(C_WHITE);
    LCD_drawPic(1);

    LCD_WriteString("Meldungen in der Queue:", 10, 10, C_BLUE, C_WHITE);

    volatile char receive_buff[50];
    volatile uint32_t queuesize = 0;


    while (1) {


        queuesize = osMessageQueueGetCount(displayQueue);

        for( int i = 0; i < queuesize; i++) {

            for(int j = 0; j < 50; j++) receive_buff[j] = NULL;

            osSemaphoreAcquire(displaySem, osWaitForever);
            osMessageQueueGet(displayQueue, &receive_buff, NULL, 0U);
            osSemaphoreRelease(displaySem);

            LCD_WriteString("->", 1, 35 + (20 * i), C_BLUE, C_WHITE);
            LCD_WriteString(receive_buff, 25, 35 + (20 * i), C_BLUE, C_WHITE);
            osDelay(pdMS_TO_TICKS(500));

        }
        if (queuesize == 0) {

            LCD_drawPic(1);
            LCD_WriteString("Meldungen in der Queue:", 10, 10, C_BLUE, C_WHITE);
        }

        osDelay(pdMS_TO_TICKS(7000));
    };
}

void HandleUserButton() {
    volatile  bool pressedbefore = false;
    while (1) {
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1 && pressedbefore == false) {


            osSemaphoreAcquire(displaySem, osWaitForever);
            osMessageQueuePut(displayQueue, "User-Taste!", 0U, 0U);
            osSemaphoreRelease(displaySem);

            pressedbefore = true;
        } else {
            pressedbefore = false;
        }
        osDelay(100);
    }
}