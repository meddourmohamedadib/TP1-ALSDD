#ifndef LIBTP1
#define LIBTP1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

typedef struct TRNode {
    struct TRNode *right;
    char *string;
    struct TRNode *left;
}TRNode;

//-----------------------

typedef struct Node {
    char value;
    struct Node *address; 
    struct TRNode *tree; 
} Node;

//-----------------------

typedef struct
{   
    char filename[256];
    int size;
    Node* *Filetab;
} Afile;

//-----------------------

typedef struct{
    int numpar,numfile;
}Apara;

//-----------------------

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void Ass_val(Node *p, char val);
void Ass_adr(Node *p, Node *add);
Node *Next(Node *p);
char Value(Node *p);
void TRNodeAlloc(TRNode **p);
char *TreeValue(TRNode *p);
TRNode * LC(TRNode *head);
TRNode * RC(TRNode *head);
void AssTreeVal(TRNode *head, char *val);
void Ass_LC(TRNode *head, TRNode *child);
void Ass_RC(TRNode *head, TRNode *child);
void Allocate(Node **p, char *word);
char *NoPunctWord(char *word);

#endif