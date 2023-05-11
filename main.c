#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#define MAX_PATH_LEANGTH 60
#define MAX_PUFFER_SIZE 1024
#define MAX_NUMBER_OF_HEADLINES 20
#define CSV_TRENNZEICHEN ";"

int queueSize = 0;
enum datatyp {NONE,CSV,JSON,XML};

struct Node{
    struct Node* next;
    char* data;
} ;

bool isQueqeEmpty(){
    if(queueSize > 0)
        return false;
    return true;
}

void initQueqe(struct Node** queue){
    *queue = (struct Node*) malloc(sizeof(struct Node));
    (*queue) -> next = NULL;
    (*queue) -> data = NULL;
}

void destructQueqe(struct Node** queue){

    if(isQueqeEmpty())
        return;
    while((*queue) != NULL){
        struct Node* pos = (*queue);
        struct Node* prevPos = NULL;
        struct Node* prevPrevPos = NULL;
        while (pos!=NULL){
            prevPrevPos = prevPos;
            prevPos = pos;
            pos = pos->next;
        }
        if(prevPos != NULL)
            free(prevPos);
        if(prevPrevPos != NULL)
            prevPrevPos->next = NULL;
    }

}
void addNode(struct Node* queue, char* data){
    queueSize++;
    if(queue->data == NULL){
        queue->data = data;
        return;
    }
    struct Node* new = (struct Node*) malloc(sizeof(struct Node));
    new->data = data;
    new->next = NULL;

    struct Node* pos = queue;
    while (pos->next!=NULL)
        pos = pos->next;

    pos->next = new;
}

char* popNode(struct Node** queqe){
    if(isQueqeEmpty())
        return NULL;
    queueSize--;
    char* out = (*queqe)->data;
    struct Node* old = (*queqe);
    (*queqe) = (*queqe)->next;
    free(old);
    return out;
}


bool getPaths(int argc, char** argv, const char* inputPath, const char* outputPath)
{
    int inFlagPos = -1;
    int outFlagPos = -1;

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-i") == 0)
            inFlagPos = i;
        else if (strcmp(argv[i], "-in") == 0)
            inFlagPos = i;
        else if (strcmp(argv[i], "-o") == 0)
            outFlagPos = i;
        else if (strcmp(argv[i], "-out") == 0)
            outFlagPos = i;
    }
    if (inFlagPos > -1 && outFlagPos > -1)
    {
        strcpy(inputPath,argv[inFlagPos + 1]);
        strcpy(outputPath,argv[outFlagPos + 1]);
        return true;
    }
    return false;
}


void readerCSV(FILE* file,struct Node* head){
    char buffer[MAX_PUFFER_SIZE];
    memset(buffer,'\0',MAX_PUFFER_SIZE);
    int buffLength;
    char* data;
    while(fgets(buffer,MAX_PUFFER_SIZE,file)){
        if(!(buffLength = strlen(buffer)))
            continue;
        if(buffer[buffLength-1] == '\n')
            buffer[buffLength-1] = '\0';
        char* token = strtok(buffer, CSV_TRENNZEICHEN);
        while (token) {
            data = (char*) malloc(strlen(token)*sizeof(char)+1);
            strcpy(data,token);
            addNode(head,data);
            token = strtok(NULL, CSV_TRENNZEICHEN);
        }
        char* nextline = (char*) malloc(sizeof(char)*2);
        strcpy(nextline,"\n");
        addNode(head,nextline);
        memset(buffer,'\0',MAX_PUFFER_SIZE);
    }
}

void readerJSON(FILE* file,struct Node* head){
     fprintf(stderr,"\nERROR: Not jet implemented!");
}
void readerXML(FILE* file,struct Node* head){
     fprintf(stderr,"\nERROR: Not jet implemented!");
}
void writerCSV(FILE* file,struct Node* head){
     fprintf(stderr,"\nERROR: Not jet implemented!");
}
void writerJSON(FILE* file,struct Node* head){
    fprintf(file,"[\n\t{\n");

    char* headlines[MAX_NUMBER_OF_HEADLINES];
    memset(headlines,0,MAX_NUMBER_OF_HEADLINES*sizeof(char*));
    char* headline = NULL;
    int counterHeadlines = 0;

    while (counterHeadlines < MAX_NUMBER_OF_HEADLINES && (headline = popNode(&head)) != NULL && strcmp(headline,"\n") != 0) {
        headlines[counterHeadlines] = headline;
        counterHeadlines++;
    }

    char* data = NULL;
    int counter = 0;
    while((data = popNode(&head)) != NULL){
        if(strcmp(data,"\n") == 0){
            counter = 0;
            if(!isQueqeEmpty())
                fprintf(file,"\t},\n\t{\n");
            free(data);
            data = NULL;
            continue;
        }
        if(counter < counterHeadlines-1)
            fprintf(file,"\t\t\"%s\": \"%s\",\n",headlines[counter],data);
        else
            fprintf(file,"\t\t\"%s\": \"%s\"\n",headlines[counter],data);
        free(data);
        data = NULL;
        counter++;
    }
    fprintf(file,"\t}\n]\n");
}

void writerXML(FILE* file,struct Node* head){
    fprintf(file,"<Table>\n\t<Entry>\n");

    char* headlines[MAX_NUMBER_OF_HEADLINES];
    memset(headlines,0,MAX_NUMBER_OF_HEADLINES*sizeof(char*));
    char* headline = NULL;
    int counterHeadlines = 0;

    while (counterHeadlines < MAX_NUMBER_OF_HEADLINES && (headline = popNode(&head)) != NULL && strcmp(headline,"\n") != 0) {
        headlines[counterHeadlines] = headline;
        counterHeadlines++;
    }

    char* data = NULL;
    int counter = 0;
    while((data = popNode(&head)) != NULL){
        if(strcmp(data,"\n") == 0){
            counter = 0;
            if(!isQueqeEmpty())
                fprintf(file,"\t</Entry>\n\t<Entry>\n");
            free(data);
            data = NULL;
            continue;
        }

        fprintf(file,"\t\t<%s>%s</%s>\n",headlines[counter],data,headlines[counter]);
        free(data);
        data = NULL;
        counter++;
    }
    fprintf(file,"\t</Entry>\n</Table>\n");
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
        return 2;
    }
    if(!outFile){
        fprintf(stderr,"ERROR: Could not open Outputfile\n");
        return 3;
    }


    char inTemp[MAX_PATH_LEANGTH];
    char outTemp[MAX_PATH_LEANGTH];
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



    if(!strcmp(outDataTyp,"csv")){
        writer[CSV](outFile,head);
    } else if(!strcmp(outDataTyp,"json")) {
        writer[JSON](outFile,head);
    } else if(!strcmp(outDataTyp,"xml")) {
        writer[XML](outFile,head);
    } else {
        writer[NONE](outFile,head);
    }

    destructQueqe(&head);
    fclose(inFile);
    fclose(outFile);

    return 0;
}

