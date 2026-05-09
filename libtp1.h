#ifndef LIBTP1
#define LIBTP1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>

//************************************************************************************
//                          -- TYPE DEFINITIONS --
//************************************************************************************

// -- Binary Search Tree node : stores one word --
typedef struct TRNode {
    struct TRNode *right;
    char *string;
    struct TRNode *left;
} TRNode;

// -- Linked List node : stores one letter bucket + its BST --
typedef struct Node {
    char value;             // -- uppercase first letter of words in this bucket --
    struct Node *address;   // -- pointer to the next LL node --
    struct TRNode *tree;    // -- root of the BST for this letter --
} Node;

// -- Represents one loaded file and all its parsed paragraphs --
typedef struct {
    char filename[256];     // -- path of the file --
    int size;               // -- number of paragraphs parsed --
    Node **Filetab;         // -- dynamic array of paragraph structures --
} Afile;

// -- Identifies one paragraph by its file index and paragraph index --
typedef struct {
    int numpar;             // -- paragraph number (1-based, user input) --
    int numfile;            // -- file number (1-based, user input) --
} Apara;

//************************************************************************************
//                     -- ABSTRACT MACHINE : LINKED LIST --
//************************************************************************************

void  Ass_val(Node *p, char val);
void  Ass_adr(Node *p, Node *add);
Node *Next(Node *p);
char  Value(Node *p);
void  Allocate(Node **p, char *word);

//************************************************************************************
//                     -- ABSTRACT MACHINE : BST --
//************************************************************************************

void    TRNodeAlloc(TRNode **p);
char   *TreeValue(TRNode *p);
TRNode *LC(TRNode *head);
TRNode *RC(TRNode *head);
void    AssTreeVal(TRNode *head, char *val);
void    Ass_LC(TRNode *head, TRNode *child);
void    Ass_RC(TRNode *head, TRNode *child);

//************************************************************************************
//                         -- WORD PREPROCESSING --
//************************************************************************************

char *NoPunctWord(char *word);

//************************************************************************************
//                         -- INSERTION OPERATIONS --
//************************************************************************************

void InsertInBST(TRNode *root, char *val);
void InsertInLL(Node **head, char *string);

//************************************************************************************
//                         -- FILE PARSING --
//************************************************************************************

Node *ParaToStruct(FILE *file);
void  FileToStruct(FILE *file, Node **Filetab[], int *size);

//************************************************************************************
//                         -- SEARCH OPERATIONS --
//************************************************************************************

bool SearchInPara(Node *para, char *word);
void search_in_BST(TRNode **root, char *word, TRNode **n, TRNode **parent);

//************************************************************************************
//                         -- DELETION OPERATIONS --
//************************************************************************************

void delete_from_BST(TRNode **root, char *word, Node *llhead);
void DeleteByPtr(Node **h, Node *Ptr, Node *prev);
void DeleteInPara(Node **para, char *word);

//************************************************************************************
//                         -- SET OPERATIONS --
//************************************************************************************

// -- Union --
void UnionTrav_PreOrd(TRNode *Root, Node **result);
void Union2Para(Node *struct1, Node **res);
void UnionNPara(Apara *arr, int arrsize, Afile *TAB, Node **res);

// -- Intersection --
void collectWords(TRNode *root, char ***arr, int *size);
void Inter2Para(Node *struct1, Node **res);
void InterNPara(Apara *arr, int arrsize, Afile *TAB, Node **res);

// -- Difference --
void DiffTrav_PreOrd(TRNode *Root, Node **result);
void Diff2Para(Node *struct1, Node **res);
void DiffNPara(Apara *arr, int arrsize, Afile *TAB, Node **res);

// -- Symmetric Difference --
void SyDiff2Para(Node *struct1, Node **res);
void SyDiffNPara(Apara *arr, int arrsize, Afile *TAB, Node **res);

//************************************************************************************
//                         -- COUNTING & STATISTICS --
//************************************************************************************

int   NumTRNodes(TRNode *Root);
int   NumStructNodes(Node *struc);
int   Allpara(Afile TAB[], int size);
float Similarity(Apara *arr, int arrsize, Afile *TAB);

//************************************************************************************
//                         -- STOPWORDS --
//************************************************************************************

void loadStopwords(Node **sw);

//************************************************************************************
//                         -- MEMORY MANAGEMENT --
//************************************************************************************

void freeBST(TRNode *root);
void freeStruct(Node **head);

//************************************************************************************
//                         -- PRINT & DISPLAY --
//************************************************************************************

void PrintTree_preord(TRNode *Root);
void PrintStruct(Node *struc);
void printTree(struct TRNode *root, char *prefix, int isLeft);

//************************************************************************************
//                         -- FILE OUTPUT --
//************************************************************************************

void FillFileTrav_preord(TRNode *Root, FILE *file);
void fillfile(Node *struc, char *path);

//************************************************************************************
//                         -- INTERFACE & ANIMATIONS --
//************************************************************************************

void Color(int text, int bg);
void loading_bar();
void landing_card();
void welcome();
void loadingAnimation();

#endif