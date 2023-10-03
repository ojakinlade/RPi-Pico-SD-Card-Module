#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "sd_card.h"
#include "ff.h"
#include "data_logger.h"

#define EXPECTED_DATA_LEN   104

FRESULT fr;/**< File operation result */
FATFS fs; /**< File system structure */
FIL fil; /**< File object */
int ret; /**< Return value from file operations */
char buf[SDCARD_BUFFER_SIZE]; /**< Temporary buffer for reading lines */

/**
 * @brief Initializes the SD card driver. 
 * 
 */
void SDCard_Init(void)
{
    // Initialize SD card
    if (!sd_init_driver()) {
        printf("ERROR: Could not initialize SD card\r\n");
        while (true);
    }
}

/**
 * @brief Unmounts the SD card drive and filesystem.
 * 
 */
static void SDCard_Unmount(void)
{
    //Unmount drive
    f_unmount("0:");
}

/**
 * @brief Mounts the SD card drive and filesystem.
 * 
 */
static void SDCard_Mount(void)
{
    // Mount drive
    fr = f_mount(&fs, "0:", 1);
    if (fr != FR_OK) {
        printf("ERROR: Could not mount filesystem (%d)\r\n", fr);
        while (true);
    }
}

/**
 * @brief Write data to a file on the SD card.
 * 
 * This function opens a file in append mode, writes data to it, and then 
 * closes the file.
 * 
 * @param filename The name of the file to write to.
 * @param data  The data to write to the file.
 */
void SDCard_Write(const char* filename, const char* data)
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

/**
 * @brief Read data from a file on the SD card and store it in a buffer.
 * 
 * This function reads lines from a file, concatenates them into a buffer,
 * and then closes the file.
 * 
 * @param filename The name of the file to read from.
 * @param sdCardBuf The buffer to store the read data. 
 * @param bufferSize  The size of the buffer.
 */
void SDCard_Read(const char* filename,char* sdCardBuf,size_t bufferSize)
{
    SDCard_Mount();
    fr = f_open(&fil, filename, FA_READ);
    if(fr != FR_OK)
    {
        printf("ERROR: Could not open file (%d)\r\n", fr);
        while(true);
    }
    printf("Reading from file '%s':\r\n",filename);
    printf("---\r\n");
    memset(sdCardBuf,'\0',bufferSize);
    while(f_gets(buf, sizeof(buf), &fil))
    {
        printf(buf);
        strncat(sdCardBuf,buf,(bufferSize - strlen(sdCardBuf) - 1));
    }
    printf("---\r\n");
    //Close file
    fr = f_close(&fil);
    if(fr != FR_OK)
    {
        printf("ERROR: Could not close file (%d)", fr);
        while(true);
    }
    SDCard_Unmount();
}

void SDCard_DeleteFile(const char* filename)
{
    SDCard_Mount();
    if(f_unlink(filename) == FR_OK)
    {
        printf("%s deleted\n",filename);
    }
    SDCard_Unmount(); 
}

void SDCard_DataQueue_Init(Queue_t* queue)
{
    queue->front = -1;
    queue->rear = -1;
    queue->size = 0;
}

int SDCard_DataQueue_IsFull(Queue_t* queue)
{
    return queue->size == MAX_QUEUE_SIZE;
}

int SDCard_DataQueue_IsEmpty(Queue_t* queue)
{
    return queue->size == 0;
}

int SDCard_DataQueue_Enqueue(Queue_t* queue, char* data)
{
    if(!SDCard_DataQueue_IsFull(queue))
    {
        if(queue->rear == -1)
        {
            queue->front = 0;
            queue->rear = 0;
            printf("Queue Init Success\n");
        }
        else
        {
            queue->rear = (queue->rear + 1) % MAX_QUEUE_SIZE;
        }
        strcpy((char*)queue->data[queue->rear], data);
        printf("%s\n",queue->data[queue->rear]);
        queue->size++;
        printf("Queue size: %d\n", queue->size);
        printf("Data added to queue\n");
        return queue->size;
    }
    else
    {
        printf("Queue is full.\r\n");
        return -1;
    }
}

void SDCard_DataQueue_Dequeue(Queue_t* queue, char* data)
{
    if(!SDCard_DataQueue_IsEmpty(queue))
    {
       strcpy(data,(char*)queue->data[queue->front]);
       queue->size--;
       if (queue->front == queue->rear)
       {
           queue->front = -1;
           queue->rear = -1;
       }
       else
       {
           queue->front = (queue->front + 1) % MAX_QUEUE_SIZE;
       }
    }
    else
    {
        printf("Queue is empty.\r\n");
        data[0] = '\0';
    }
}

void ParseDataIntoQueue(Queue_t* queue, char* data)
{
    char dataLine[256] = {0};
    int i;
    int index = 0;
    int numOfLines = strlen(data) / EXPECTED_DATA_LEN;
    SDCard_DataQueue_Init(queue);
    while(numOfLines != 0)
    {
        for(i = index; i < EXPECTED_DATA_LEN + index; i++)
        {
            dataLine[i - index] = data[i];
        }
        index = i + 1;
        SDCard_DataQueue_Enqueue(queue,dataLine);
        numOfLines--;
    }
}
