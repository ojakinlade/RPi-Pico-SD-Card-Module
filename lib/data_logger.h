#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

void SDCard_Init(void);
void SDCard_Write(const char *filename, const char *data);
void SDCard_Read(void);



#endif /*DATA_LOGGER_H*/