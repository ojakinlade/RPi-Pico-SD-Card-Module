#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "data_logger.h"

const char* filename = "log.txt";
char SDCard_ReadBuf[SDCARD_BUFFER_SIZE] = {0};

int main()
{
    stdio_init_all();
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN,true);
    //Initialize SD card
    SDCard_Init();
    
    while(true)
    {     
        SDCard_Write(filename, "Testing 1,2,3\r\n");
        SDCard_Read(filename, SDCard_ReadBuf,sizeof(SDCard_ReadBuf));
        printf("SDCard Buffer:\r\n");
        printf(SDCard_ReadBuf);
        sleep_ms(10000);
    }
}