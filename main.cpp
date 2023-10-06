#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "data_logger.h"

const char* INIT_FILE = "init.txt";
const char* dataFile = "log.txt";
char message[200] = "{\"timestamp\":\"24-07-2023 08:28:35\",\"sensor_id\":\"Test_01\",\"lat\":\"0.000000\",\"lng\":\"0.000000\",\"status\":\"1\"}\n";
char SDCard_ReadBuf[SDCARD_BUFFER_SIZE] = {0};
bool deviceOnboard = false;

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
    sleep_ms(5000);
    if(SDCard_FileExists(INIT_FILE) == FR_NO_FILE)
    {
        deviceOnboard = true;
        printf("File does not exist");
        SDCard_Write(INIT_FILE, "pai-01");
    }
    else if(SDCard_FileExists(INIT_FILE) == FR_OK)
    {
        printf("File exists");
    }
    
    while(true)
    {     
        if(deviceOnboard)
        {
            deviceOnboard = false;
            printf("Data sent to MQTT Init topic");
        }
        SDCard_Write(dataFile, message);
        sleep_ms(500);
        SDCard_Write(dataFile, message);
        sleep_ms(500);
        SDCard_Write(dataFile, message);
        sleep_ms(500);
        SDCard_Write(dataFile, message);
        sleep_ms(500);
        SDCard_Write(dataFile, message);
        sleep_ms(500);
        //Read from the SD Card
        SDCard_Read(dataFile,SDCardData,sizeof(SDCardData));
        ParseDataIntoQueue(&SDCardDataQueue,SDCardData);
        while(!SDCard_DataQueue_IsEmpty(&SDCardDataQueue))
        {
            SDCard_DataQueue_Dequeue(&SDCardDataQueue,dataLine);
            printf("Data sent: %s\n",dataLine);
            sleep_ms(100);
        }
        SDCard_DeleteFile(dataFile);
        sleep_ms(10000);
    }
}