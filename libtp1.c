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

void Ass_val(Node *p, char val) {
    p->value = val;
}

//--------------------------------------------------

void Ass_adr(Node *p, Node *add) {
    p->address = add;
}

//--------------------------------------------------

Node *Next(Node *p) {
    return (p->address);
}

//--------------------------------------------------

char Value(Node *p) {
    return (p->value);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-

void TRNodeAlloc(TRNode **p) {
    *p = (TRNode *)malloc(sizeof(TRNode));
    (*p)->right = NULL;
    (*p)->left = NULL;
}

//--------------------------------------------------

char *TreeValue(TRNode *p) {
    return p->string;
}

//--------------------------------------------------

TRNode * LC(TRNode *head) {
    return head->left;
}

//--------------------------------------------------

TRNode * RC(TRNode *head) {
    return head->right;
}

//--------------------------------------------------

void AssTreeVal(TRNode *head, char *val) {
    head->string = val;
}

//--------------------------------------------------

void Ass_LC(TRNode *head, TRNode *child) {
    head->left = child;
}

//--------------------------------------------------

void Ass_RC(TRNode *head, TRNode *child) {
    head->right = child;
}

//--------------------------------------------------

void Allocate(Node **p, char *word) {
    *p = (Node *)malloc(sizeof(Node));
    Ass_val(*p,toupper(word[0]));
    Ass_adr(*p, NULL);
    TRNodeAlloc(&((*p)->tree));
    AssTreeVal(((*p)->tree), word);
}

//--------------------------------------------------

char *NoPunctWord(char *word) {
    int i = 0, j = 0;
    char *tmp;
    while (word[i] != '\0') {
        if (isalnum((unsigned char)word[i])) {
            tmp = word + j;
            *tmp = word[i];
            j++;
        }
        i++;
    }
    tmp = word +j;
    *tmp = '\0';
    return word;
}