#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "data_logger.h"

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
        SDCard_Write("testFile.txt", "Testing 1,2,3\r\n");
        SDCard_Write("logger.txt", "testtttt\r\n");
        printf("Done writing to file.\r\n");
        sleep_ms(10000);
    }
}