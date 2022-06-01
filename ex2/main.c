#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "frame.h"
#include <string.h>
#define _GNU_SOURCE
#define HASH_SIZE 500
#define MAXREADS 20000
int main(int argc, char *argv[]){
    if(argc!=4){
        printf("Wrond imput \n" );
        printf("type :./os2  <algo> <frames in ram> <reads per process>\n" );
        printf("algo -> 'LRU' or '2nd'\n" );
        printf("frames ->  unsigned int\n" );
        printf("reads per process ->  unsigned int\n" );
        return -1;

    }else{
        char algo[3];
        int q,frames;
        strcpy(algo,argv[1]);
        frames=atoi(argv[2]);
        q=atoi(argv[3]);
        if((strcmp(algo,"LRU")!=0 && strcmp(algo,"2nd")!=0) || q<=0 || frames<=0){
            printf("Wrond imput \n" );
            printf("type :./os2  <algo> <frames in ram> <reads per process>\n" );
            printf("algo -> 'LRU' or '2nd'\n" );
            printf("frames ->  unsigned int\n" );
            printf("reads per process ->  unsigned int\n" );
            return -1;
        }
        FILE* file1;
        FILE* file2;
        char act;
        int address,i,k;

        printf("==================================\n" );
        printf("algo %s\n", argv[1] );
        printf("frames in ram %d\n", frames );
        printf("reads per process %d\n", q );
        printf("MAXREADS %d\n", MAXREADS );
        printf("==================================\n" );
        file1=fopen("gcc.trace","r");
        file2=fopen("bzip.trace","r");
        pageHash* hash1= HashCreate(HASH_SIZE,0);
        pageHash* hash2= HashCreate(HASH_SIZE,1);
        frameInfo* frameTable= createFrameInfo( frames, argv[1],hash1,hash2);

        k=0;
        while(k<MAXREADS){
            for(i=0; i<q; i++){
                fscanf(file1, "%x %c",&address,&act);
                address=address>>12;
                addPage(frameTable,address,act,0);
            }
            for(i=0; i<q; i++){
                fscanf(file2, "%x %c",&address,&act);
                address=address>>12;
                addPage(frameTable,address,act,1);
            }
            k+=2*q;
        }
        fclose(file1);
        fclose(file2);
        HashDestroy(hash1);
        HashDestroy(hash2);
        frameDestroy(frameTable);
        return 0;

    }

}
