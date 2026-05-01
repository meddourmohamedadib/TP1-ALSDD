#ifndef LIBTP1
#define LIBTP1
#include <string.h>
#include <ctype.h>

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

#endif