#include "hash.h"


pageHash* HashCreate(int pageNum, int proc ){

    pageHash* item= malloc(sizeof(pageHash));
    item->numOfPages=pageNum;
    item->process=proc;
    item->pages= malloc(pageNum*(sizeof(node*)));
    int i;
    for (i=0; i<pageNum; i++){
        item->pages[i]=NULL;
    }
    return item;
}

void HashDestroy(pageHash* item){
    int i;
    for (i=0; i<item->numOfPages; i++){
        listDestroy(&(item->pages[i]));
    }
    free(item->pages);
    free(item);
}

node* searchEntry(int pageNum, pageHash* item){

    int i;
    node* nodePtr;

    for (i=0;  i< item->numOfPages; i++){
        nodePtr=item->pages[i];
        while(nodePtr!=NULL){
            if (nodePtr->pageNum == pageNum) return nodePtr;
            nodePtr=nodePtr->next;

        }
    }
    return NULL;

}

void printHash(pageHash* item) {
    int i;
    for (i=0; i<item->numOfPages; i++){
        printf("cell with key= %d ==============\n",i);
        printList(item->pages[i]);
    }
}

node* addNode(int pageNum,node** first ){

    node* nodePtr= malloc(sizeof(node));
    nodePtr->pageNum=pageNum;
    nodePtr->framePos=-1;
    if ((*first)==NULL){
        nodePtr->next=NULL;
        *first=nodePtr;
    }else{
        nodePtr->next=*first;
        *first=nodePtr;
    }
    return nodePtr;
}

void printList(node* first){

    if(first!= NULL){
        node* nodePtr= first;
        printf("first node pageNum = %d , framePos = %d\n",nodePtr->pageNum, nodePtr->framePos );
        while(nodePtr->next!=NULL){
            nodePtr=nodePtr->next;
            printf("next node pageNum = %d , framePos = %d\n",nodePtr->pageNum, nodePtr->framePos);
        }
    }
}

void listDestroy(node** first){

    if(*first!= NULL){
        node* nodePtr= *first;
        node* temp= nodePtr->next;
        free(nodePtr);
        while(temp!=NULL){
            nodePtr=temp;
            temp=temp->next;
            free(nodePtr);
        }
        *first=NULL;
    }
}

unsigned long hash(int num, pageHash* item){

    unsigned long hash = 5016;
    unsigned char* str= malloc(15*sizeof(char));
    unsigned char* backup=str;
    int c;

    sprintf(str, "%d", num);
    while (c = *str++)
        hash = ((hash << 5) + hash) + c;
    free(backup);
    return abs(hash%(item->numOfPages));
}
