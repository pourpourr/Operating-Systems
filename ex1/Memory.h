#ifndef __MEMORY__
#define __MEMORY__

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include "P1.h"

typedef struct {
    char message[20];
    int posibol;
    int hashValue;
    int ChangeCounter;
    int sem_id;
    int badNews;


} SMemory;

void CopyString(SMemory* mem,char* str);

int makeSpace(int key);

unsigned long hash( char *str);


int getSemId(SMemory* mem);
void setSemId(SMemory* mem,int id);


#endif
