#ifndef VNC1L_H
#define VNC1L_H

#include <stdint.h>

#define PROMPT "D:\>"

#define DIR 0x01
#define CD 0x01
#define RD 0x04
#define DLD 0x05
#define MKD 0x06
#define DLF 0x07
#define WRF 0x08
#define OPW 0x09
#define CLF 0x0A
#define RDF 0x0B
#define REN 0x0C
#define OPR 0x0E
#define SEK 0x28
#define FS 0x12
#define FSE 0x93
#define IDD 0x0F
#define IDDE 0x94
#define DSN 0x2D
#define DVL 0x2E
#define DIRT 0x2F
#define PRE 0x20
#define END 0x0D

void VNC1L_init(void);

void VNC1L_command(uint32_t cmd, char* str1, char *str2, char *buf, int size);

void VNC1L_ls(char *buff, uint32_t size);
void VNC1L_cd(char* dir);
void VNC1L_openR(char* file);
void VNC1L_openW(char* file);
void VNC1L_read(char *buff, uint32_t size);
void VNC1L_write(char* buff, uint32_t size);
void VNC1L_seek(uint32_t pos);
void VNC1L_close(char* file);

#endif
