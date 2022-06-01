#ifndef __HASH__
#define __HASH__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct node{
    int pageNum;
    int framePos;
    struct node* next;
}node;

typedef struct{

    int numOfPages;
    int process;
    node** pages;

}pageHash;

pageHash* HashCreate(int pageNum, int proc);

void HashDestroy(pageHash* item);

void addEntry(int pageNum,char act, pageHash* item);

node* searchEntry(int pageNum, pageHash* item);

void printHash(pageHash* item);

node* addNode(int pageNum,node** first );

void listDestroy(node** first);

void printList(node* first);

unsigned long hash(int num, pageHash* item);

#endif
