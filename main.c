#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
#define MAX_PATH_LEANGTH 60
#define MAX_PUFFER_SIZE 1024
#define CSV_TRENNZEICHEN ";"
enum datatyp {NONE,CSV,JSON,XML};

struct Node{
    struct Node* next;
    char* data;
} ;

void initQueqe(struct Node** queqe){
    *queqe = (struct Node*) malloc(sizeof(struct Node));
    (*queqe) -> next = NULL;
    (*queqe) -> data = NULL;
}

void destructQueqe(struct Node** queqe){

    while((*queqe) != NULL){
        struct Node* pos = (*queqe);
        while (pos != NULL && pos->next!=NULL){
            pos = pos->next;
        }
        free(pos);
        pos = NULL;
    }

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
        strcpy(outputPath,argv[outFlagPos + 1]);
        //printf("in=%s\tout=%s",inputPath,outputPath);
        return true;
    }
    return false;
}



void readerCSV(FILE* file,struct Node* head){
    char buffer[MAX_PUFFER_SIZE];
    memset(buffer,'\0',MAX_PUFFER_SIZE);
    int buffLength;
    char* data;
    char* nextline = (char*) malloc(sizeof(char)*2);
    strcpy(nextline,"\n");
    while(fgets(buffer,MAX_PUFFER_SIZE,file)){     //Problem: immer false
        if(!(buffLength = strlen(buffer)))
            continue;
        buffer[buffLength-1] = '\0';
        char* token = strtok(buffer, CSV_TRENNZEICHEN);
        while (token) {
            data = (char*) malloc(strlen(token)*sizeof(char)+1);
            strcpy(data,token);
            addNode(head,(char*) malloc(strlen(token)*sizeof(char)+1));
        }
        addNode(head,nextline);

    }

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
    memset(inPath,'\0',MAX_PATH_LEANGTH);
    memset(outPath,'\0',MAX_PATH_LEANGTH);

    if (!getPaths(argc, argv, inPath, outPath)){
        fprintf(stderr,"\nPlease provide an input an output parameter");
        return 1;
    }

    FILE* inFile = fopen(inPath,"r");
    FILE* outFile = fopen(outPath,"w");

    if(!inFile){
        fprintf(stderr,"ERROR: Could not open Inputfile\n");
        return 1;
    }
    if(!outFile){
        fprintf(stderr,"ERROR: Could not open Outputfile\n");
        return 2;
    }


    char inTemp[strlen(inPath)+1];
    char outTemp[strlen(outPath)+1];
    strcpy(inTemp,inPath);
    strcpy(outTemp,outPath);


    strtok(inTemp,".");
    char* inDataTyp = strtok(NULL,".");
    strtok(outTemp,".");
    char* outDataTyp = strtok(NULL,".");

    struct Node* head = NULL;
    initQueqe(&head);

    if(!strcmp(inDataTyp,"csv")){
        reader[CSV](inFile,head);
    } else if(!strcmp(inDataTyp,"json")) {
        reader[JSON](inFile,head);
    } else if(!strcmp(inDataTyp,"xml")) {
        reader[XML](inFile,head);
    } else {
        reader[NONE](inFile,head);
    }

    printf("data: %s",head->data);

    if(!strcmp(outDataTyp,"csv")){
        writer[CSV](outFile,head);
    } else if(!strcmp(outDataTyp,"json")) {
        writer[JSON](outFile,head);
    } else if(!strcmp(outDataTyp,"xml")) {
        writer[XML](outFile,head);
    } else {
        writer[NONE](outFile,head);
    }

    destructQueqe(head);





    return 0;
}

