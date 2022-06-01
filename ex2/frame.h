#ifndef __FRAME__
#define __FRAME__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"


typedef struct frameInfo {
    int maxFrames;
    int currFrames;
    char method[3];
    pageHash* hash1;
    pageHash* hash2;
    struct frameLRU* first;
    struct frameLRU* last;
    struct frame2nd* clock;
    struct frame2nd* head;
    int pagefault;
    int writeCounter;
}frameInfo;

typedef struct frameLRU {
    int pageNum;
    int process;
    char action;
    struct frameLRU* next;
    struct frameLRU* prev;
}frameLRU;

typedef struct frame2nd {
    int frameNum;
    int pageNum;
    short refBit;
    char action;
    int process;
    struct frame2nd* next;
}frame2nd;

void addPage(frameInfo* info, int pageNum,char act,int process);

void frameModder2nd(frame2nd* frame,int page, int proc, char act, int frNum, frame2nd* next );

void addFrame(frameInfo* info, int pageNum, char act,int process, int* newframePos, int* oldPage, int* oldPro );

void  addFrameLRU(frameInfo* info, int page, char act,int proc,  int* oldPage, int* oldPro);

void frameModderLRU(frameLRU* frame,int page, int proc, char act);

void addFrame2nd(frameInfo* info, int pageNum, char act,int proc, int* newframePos, int* oldPage, int* oldPro );

frameInfo* createFrameInfo(int max, char* algo , pageHash* h1, pageHash* h2 );

void printFrames( frameInfo* info);

frame2nd* searchFrame2nd(int page,frameInfo* info);

void frameDestroy(frameInfo* info);

frameLRU* searchFrameLRU(int page,frameInfo* info);


#endif
