#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#include <stddef.h>

#define SDCARD_BUFFER_SIZE    256
#define MAX_QUEUE_SIZE        100 // Maximum number of lines to store

typedef struct
{
    char data[MAX_QUEUE_SIZE][256]; // Assuming lines have a maximum length of 255 characters
    int front, rear, size;
}Queue_t;

extern void SDCard_Init(void);
extern void SDCard_Write(const char* filename, const char* data);
extern void SDCard_Read(const char* filename,char* sdCardBuf,size_t bufferSize);
extern void ParseDataIntoQueue(Queue_t* queue, char* data);
extern void SDCard_DataQueue_Dequeue(Queue_t* queue, char* data);
extern int SDCard_DataQueue_IsEmpty(Queue_t* queue);

#endif /*DATA_LOGGER_H*/