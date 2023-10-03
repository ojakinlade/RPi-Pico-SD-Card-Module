#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "data_logger.h"

const char* filename = "log.txt";
char message[200] = "{\"timestamp\":\"24-07-2023 08:28:35\",\"sensor_id\":\"Test_01\",\"lat\":\"0.000000\",\"lng\":\"0.000000\",\"status\":\"1\"}\n";
char SDCard_ReadBuf[SDCARD_BUFFER_SIZE] = {0};

int main()
{
    stdio_init_all();
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN,true);
    //Initialize SD card
    Queue_t SDCardDataQueue;
    char SDCardData[4096] = {0};
    char dataLine[256] = {0};
    SDCard_Init();
    
    while(true)
    {     
        SDCard_Write(filename, message);
        sleep_ms(500);
        SDCard_Write(filename, message);
        sleep_ms(500);
        SDCard_Write(filename, message);
        sleep_ms(500);
        SDCard_Write(filename, message);
        sleep_ms(500);
        SDCard_Write(filename, message);
        sleep_ms(500);
        //Read from the SD Card
        SDCard_Read(filename,SDCardData,sizeof(SDCardData));
        ParseDataIntoQueue(&SDCardDataQueue,SDCardData);
        while(!SDCard_DataQueue_IsEmpty(&SDCardDataQueue))
        {
            SDCard_DataQueue_Dequeue(&SDCardDataQueue,dataLine);
            printf("Data sent: %s\n",dataLine);
            sleep_ms(100);
        }
        sleep_ms(30000);
    }
}