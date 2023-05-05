#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
#define MAX_PATH_LEANGTH 60
enum datatyp {NONE,CSV,JSON,XML};

struct Node{
    struct Node* next;
    char* data;
} ;

void initQueqe(struct Node* queqe){
    queqe->next = NULL;
    queqe -> data = NULL;
}

void addNode(struct Node* queqe, char* data){
    struct Node* new = (struct Node*) malloc(sizeof(struct Node));
    new->data = data;
    new->next = NULL;

    struct Node* pos = queqe;
    while (pos->next!=NULL)
        pos = pos->next;

    pos->next = new;
}

char* popNode(struct Node* queqe){
    struct Node* pos = queqe;
    if(pos == NULL)
        return NULL;
    while (pos->next!=NULL)
        pos = pos->next;
    char* out = pos->data;
    free(pos);
    pos = NULL;
    return out;
}







bool getPaths(int argc, char** argv, const char* inputPath, const char* outputPath)
{
    int inFlagPos = -1;
    int outFlagPos = -1;

    for (int i = 0; i < argc; i++)
    {
        printf("%s ",argv[i]);
        if (strcmp(argv[i], "-i") == 0)
            inFlagPos = i;
        else if (strcmp(argv[i], "-in") == 0)
            inFlagPos = i;
        else if (strcmp(argv[i], "-o") == 0)
            outFlagPos = i;
        else if (strcmp(argv[i], "-out") == 0)
            outFlagPos = i;
    }
    printf("\ni=%i\to=%i" ,inFlagPos,outFlagPos);
    if (inFlagPos > -1 && outFlagPos > -1)
    {
        strcpy(inputPath,argv[inFlagPos + 1]);
        strcpy(outputPath,argv[inFlagPos + 1]);
        //printf("in=%s\tout=%s",inputPath,outputPath);
        return true;
    }
    return false;
}

void readCsv(FILE* in){
    char line[1024];
    int length;
    while (fgets(line,1023,in)){
        if((length = strlen(line)))
            continue;
        line[length-1] = '\0';
    }
}

void readerCSV(FILE* file,struct Node* head){

}
void readerJSON(FILE* file,struct Node* head){

}
void readerXML(FILE* file,struct Node* head){

}
void writerCSV(FILE* file,struct Node* head){

}
void writerJSON(FILE* file,struct Node* head){

}
void writerXML(FILE* file,struct Node* head){

}


void (*reader[])(FILE* file,struct Node* head) = {NULL,readerCSV,readerJSON,readerXML};

void (*writer[])(FILE* file,struct Node* head) = {NULL,writerCSV,writerJSON,writerXML};

int main(int argc, char** argv)
{

    char inPath[MAX_PATH_LEANGTH];
    char outPath[MAX_PATH_LEANGTH];

    if (!getPaths(argc, argv, inPath, outPath)){
        fprintf(stderr,"\nPlease provide an input an output parameter");
        return 1;
    }

    FILE* inFile = fopen(inPath,"r");
    FILE* outFile = fopen(outPath,"w");


    strtok(inPath,".");
    char* inDataTyp = strtok(NULL,".");
    strtok(outPath,".");
    char* outDataTyp = strtok(NULL,".");

    struct Node* head = (struct Node*) malloc(sizeof(struct Node));
    initQueqe(head);

    if(!strcmp(inDataTyp,"csv")){
        reader[CSV](inFile,head);
    } else if(!strcmp(inDataTyp,"json")) {
        reader[JSON](inFile,head);
    } else if(!strcmp(inDataTyp,"xml")) {
        reader[XML](inFile,head);
    } else {
        reader[NONE](inFile,head);
    }


    if(!strcmp(outDataTyp,"csv")){
        writer[CSV](inFile,head);
    } else if(!strcmp(outDataTyp,"json")) {
        writer[JSON](inFile,head);
    } else if(!strcmp(outDataTyp,"xml")) {
        writer[XML](inFile,head);
    } else {
        writer[NONE](inFile,head);
    }





    return 0;
}

