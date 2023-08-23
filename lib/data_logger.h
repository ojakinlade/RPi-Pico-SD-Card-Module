#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#define SDCARD_BUFFER_SIZE    256

extern void SDCard_Init(void);
extern void SDCard_Write(const char* filename, const char* data);
extern void SDCard_Read(const char* filename,char* sdCardBuf,size_t bufferSize);

#endif /*DATA_LOGGER_H*/