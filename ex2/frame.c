#include "frame.h"

frameInfo* createFrameInfo(int max, char* algo , pageHash* h1, pageHash* h2 ){
    frameInfo* info= malloc(sizeof(frameInfo));
    info->maxFrames=max;
    info->currFrames=0;
    info->hash1=h1;
    info->hash2=h2;
    strcpy(info->method,algo);
    info->clock=NULL;
    info->head=NULL;
    info->first=NULL;
    info->last=NULL;
    info->pagefault=0;
    info->writeCounter=0;
    return info;
}

void frameDestroy(frameInfo* info) {

    int i;
    if(info->currFrames>0){
        printf("||||||||||||||||||||||\n" );
        printf("page faults= %d\n",info->pagefault );
        printf("writes in memory= %d \n",info->writeCounter );
        printf("||||||||||||||||||||||\n" );

        if(strcmp("2nd",info->method)==0){
            if(info->head!=NULL){
                frame2nd* framePtr= info->head;
                frame2nd* temp= (info->head)->next;
                free(framePtr);
                for(i=0;( i<info->currFrames-1 && temp!=NULL ); i++){
                        framePtr=temp;
                        temp=temp->next;
                        free(framePtr);
                }
            }
        }else{
            if(info->first!=NULL){
                frameLRU* framePtr= info->first;
                frameLRU* temp= (info->first)->next;
                free(framePtr);
                for(i=0; i<info->currFrames-1; i++){
                        framePtr=temp;
                        temp=temp->next;
                        free(framePtr);
                }
            }
        }
        free(info);
    }
}

void addPage(frameInfo* info, int pageNum,char act,int process){

    node* deleted; // the node that will eventually have framePos=-1
    unsigned long key;
    node* nodePtr;
    int frPos; //framePos of the new pageNum. This will be stored in Hash.
    int oldPageNum; // pageNum of the node deleted
    int oldPro; //the process of node dleted

    if((info->hash1)->process ==process){
        key=hash(pageNum,(info->hash1));
        nodePtr=searchEntry(pageNum,(info->hash1));
        if (nodePtr==NULL){
            info->pagefault++;
            nodePtr=addNode(pageNum,&((info->hash1)->pages[key]));
        }else{
            if(nodePtr->framePos==-1)  info->pagefault++;
        }
        addFrame(info, pageNum, act,(info->hash1)->process,&frPos,&oldPageNum,&oldPro );
        nodePtr->framePos=frPos;

    }else{
        key=hash(pageNum,(info->hash2));
        nodePtr=searchEntry(pageNum,(info->hash2));

        if (nodePtr==NULL){
             info->pagefault++;
            nodePtr=addNode(pageNum,&((info->hash2)->pages[key]));
        }else{
            if(nodePtr->framePos==-1)  info->pagefault++;
        }

        addFrame(info, pageNum, act,(info->hash2)->process,&frPos,&oldPageNum,&oldPro );
        nodePtr->framePos=frPos;
    }

    if(oldPageNum!=-1){  // oldPageNum=-1 means no frame was deleted , or existing page renewed from same process
        if (oldPageNum==-2){ //existing page  from different process
            if (oldPro==0){
                deleted=searchEntry(pageNum, (info->hash1));
            }else{
                deleted=searchEntry(pageNum, (info->hash2));
            }
        }else{
            if (oldPro==0){
                deleted=searchEntry(oldPageNum,(info->hash1));
            }else{
                deleted=searchEntry(oldPageNum,(info->hash2));
            }
        }
        deleted->framePos=-1;
    }

}

void addFrame(frameInfo* info, int pageNum, char act,int process, int* newframePos, int* oldPage, int* oldPro ){
    if(strcmp("2nd",info->method)==0){
        addFrame2nd(info,pageNum,act,process,newframePos,oldPage,oldPro);
    }else{
        addFrameLRU(info,pageNum,act,process,oldPage,oldPro);
        *newframePos=1;
    }
}

void addFrame2nd(frameInfo* info, int page, char act,int proc, int* newframePos, int* oldPage, int* oldPro ){
    if(info->head==NULL){
        frame2nd* frame=malloc(sizeof(frame2nd));
        frameModder2nd(frame,page,proc,act,0,frame);
        info->clock=frame;
        info->head=frame;
        info->currFrames++;
        *newframePos=0;
        *oldPage=-1; //no page there
        *oldPro=-1;
    }else{
        frame2nd* found;
        found=searchFrame2nd(page,info);
        if(found!=NULL){
            if(proc==found->process){
                found->refBit=1;
                if(info->currFrames==info->maxFrames)  info->clock=found;  //  when list is not full clock  points to  last
                *newframePos=found->frameNum;
                *oldPage=-1; // already inside and same process do nothing
                *oldPro=found->process;
            }else{
                *newframePos=found->frameNum;
                *oldPage=-2; // already inside but different process
                *oldPro=found->process;
                frameModder2nd(found,page,proc,act,-1,NULL);
            }
        }else{
            int i;
            if(info->currFrames<info->maxFrames){
                frame2nd* found=malloc(sizeof(frame2nd));
                frameModder2nd(found,page,proc,act,info->currFrames,info->head);
                (info->clock)->next=found;
                info->clock=found;
                info->currFrames++;
                *newframePos=found->frameNum;
                *oldPage=-1;  //no page there
                *oldPro=-1;


            }else{
                found=(info->clock)->next;
                while(found->refBit!=0){
                    found->refBit=0;
                    found=found->next;
                }
                if(found->action=='W') info->writeCounter++;
                *oldPage=found->pageNum;
                *oldPro=found->process;
                frameModder2nd(found,page,proc,act,-1,NULL);
                info->clock=found;
                *newframePos=found->frameNum;
            }
        }
    }
}

void frameModder2nd(frame2nd* frame,int page, int proc, char act, int frNum, frame2nd* next ){

    frame->refBit=1; // if we change a frame bit becomes 1
    frame->pageNum=page;
    frame->process=proc;
    frame->action=act;
    if(frNum!=-1) frame->frameNum=frNum; //frNum=-1 means we dont want to change the framePos
    if(next!=NULL)frame->next=next;     //the same with next=NULL
}

void addFrameLRU(frameInfo* info, int page, char act,int proc,  int* oldPage, int* oldPro){

    if(info->first==NULL){  // stack is empty
        frameLRU* frame=malloc(sizeof(frameLRU));
        frameModderLRU(frame,page,proc,act);
        info->first=frame;
        info->last=frame;
        info->currFrames++;
        *oldPage=-1;
        *oldPro=-1;

    }else{              // stack not empty
        frameLRU* found;
        found=searchFrameLRU(page,info);
        if (found!=NULL){
                 // page already inside so we  change its pos in the stack
            *oldPro=found->process;
            if(proc!=found->process) {
                *oldPage=-2; // already inside but different process
            }else{
                *oldPage=-1; // already inside and same process do nothing

            }
            frameModderLRU(found,page,proc,act);

            if (found!=info->first) {
                if(found!=info->last){
                    (found->prev)->next=found->next;
                    (found->next)->prev=(found->prev);
                    (info->first)->next= found;
                    (info->last)->prev= found;
                    found->prev=info->first;
                    found->next=info->last;
                    info->first=found;
                }else{
                    info->last=found->next;
                    info->first=found;
                }
            }
        }else{
            if(info->currFrames<info->maxFrames){   //not inside but stack not full so we add
                *oldPage=-1;
                *oldPro=-1;
                frameLRU* found= malloc(sizeof(frameLRU));
                frameModderLRU(found,page,proc,act);
                (info->first)->next= found;
                (info->last)->prev= found;
                found->prev=info->first;
                found->next=info->last;
                info->first=found;
                info->currFrames++;

            }else{                              //not inside and stack full so we change  last's value
                found=info->last;               // and change the last n first tags
                if(found->action=='W') info->writeCounter++;
                *oldPage=found->pageNum;
                *oldPro=found->process;
                frameModderLRU(found,page,proc,act);
                info->last=found->next;
                info->first=found;
            }
        }

    }
}

void frameModderLRU(frameLRU* frame,int page, int proc, char act){

    frame->pageNum=page;
    frame->process=proc;
    frame->action=act;

}

frame2nd* searchFrame2nd(int page,frameInfo* info){

    frame2nd* found= info->head;
    int i;
    for(i=0; i<info->currFrames; i++){
        if (found->pageNum==page) {
            return found;
        }else{
            found=found->next;
        }
    }
    return NULL;
}

frameLRU* searchFrameLRU(int page,frameInfo* info){

    frameLRU* found= info->first;
    int i;
    for(i=0; i<info->currFrames; i++){
        if (found->pageNum==page) {
            return found;
        }else{
            found=found->next;
        }
    }
    return NULL;
}

void printFrames( frameInfo* info){

    int i;
    printf("=======================\n" );
    if(strcmp("LRU",info->method)==0){
        frameLRU* frame=info->first;
        printf("info -> first %d  || info->last %d\n",(info -> first)->pageNum, (info -> last)->pageNum );
        for (i=0; i< info->currFrames; i++){
            printf("Frame %d ||page %d || next page %d || prev page %d || process= %d\n",i, frame->pageNum,(frame->next)->pageNum,(frame->prev)->pageNum,frame->process);
            frame=frame->prev;
        }
    }else{
        frame2nd* frame=info->head;
        printf("info -> head %d  || real clock %d\n",(info -> head)->pageNum, ((info -> clock)->next)->pageNum );
        for (i=0; i< info->currFrames; i++){
            printf("Frame %d ||page %d || next page %d || refBit %d ||process= %d\n",i, frame->pageNum,(frame->next)->pageNum,frame->refBit,frame->process);
            frame=frame->next;
        }
    }
    printf("=======================\n" );
}
