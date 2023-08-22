#include <stdio.h>
#include "pico/stdlib.h"
#include "sd_card.h"
#include "ff.h"
#include "data_logger.h"

FRESULT fr;
FATFS fs;
FIL fil;
int ret;
char buf[150];

void SDCard_Init(void)
{
    // Initialize SD card
    if (!sd_init_driver()) {
        printf("ERROR: Could not initialize SD card\r\n");
        while (true);
    }
}

static void SDCard_Unmount(void)
{
    //Unmount drive
    f_unmount("0:");
}

static void SDCard_Mount(void)
{
    // Mount drive
    fr = f_mount(&fs, "0:", 1);
    if (fr != FR_OK) {
        printf("ERROR: Could not mount filesystem (%d)\r\n", fr);
        while (true);
    }
}

void SDCard_Write(const char *filename, const char *data)
{
    SDCard_Mount();
    fr = f_open(&fil, filename, FA_WRITE | FA_OPEN_APPEND);
    if(fr != FR_OK)
    {
        printf("ERROR: Could not open file (%d)\r\n", fr);
        while(true);
    }
    //write something to file
    ret = f_printf(&fil, data);
    if(ret < 0)
    {
        printf("ERROR: Could not write to file (%d)\r\n", ret);
        f_close(&fil);
        while(true);
    }
    //Close file
    fr = f_close(&fil);
    if(fr != FR_OK)
    {
        printf("ERROR: Could not close file (%d)", fr);
        while(true);
    }
    SDCard_Unmount();
}

void SDCard_Read(void)
{

}
