#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "libtp1.h"

//---------------------------------------
typedef Node*  tabty ; 

/*//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

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

//--------------------------------------------------*/

void InsertInBST(TRNode *root,char* val) {
    TRNode *p = root;
    TRNode *prev = root;
    while (p != NULL) {      
        prev = p;
        if (_stricmp(TreeValue(p),val)>0) {
            p = LC(p);
        }
        else {
            p = RC(p);
        }
    }
    if (_stricmp(TreeValue(prev),val)>0) {
        TRNodeAlloc(&p);
        Ass_LC(prev, p);
        AssTreeVal(p, val);
    }
    else {
        TRNodeAlloc(&p);
        Ass_RC(prev, p);
        AssTreeVal(p, val);
    }
}

//--------------------------------------------------------------------------------------------------

void printTree(struct TRNode* root, char* prefix, int isLeft) {
    if (root == NULL) return;

    printf("%s", prefix);
    printf("%s", isLeft ? "|-- " : "\\-- ");
    printf("%s\n", root->string);

    char newPrefix[1024];
    snprintf(newPrefix, sizeof(newPrefix), "%s%s",
             prefix, isLeft ? "|   " : "    ");

    printTree(root->left, newPrefix, 1);
    printTree(root->right, newPrefix, 0);
}

//---------------------------------------------------------------------------------------

void InsertInLL(Node **head,char *string){
    Node* p=*head,*prev=*head;
    if (*head == NULL) {
        Allocate(head, string);
        return;
    }
    while((p!=NULL)&&(Value(p) <= toupper(string[0]))){
        prev=p;
        if((toupper(string[0]))==Value(p)){
            InsertInBST(p->tree,string) ;                       //8888888888888888888888888
            return;
        }
        p=Next(p);
    }
    if (p==(*head))
    {
        Allocate(&p,string); 
        *head=p; 
        Ass_adr(p,prev);
    }
    else{
        Allocate(&p,string); 
        Ass_adr(p,Next(prev));
        Ass_adr(prev,p);
    }
    return;
}

//--------------------------------------------------

Node * ParaToStruct(FILE *file) {
    Node *head = NULL;
    char line[1024];
    char* word;
    char *tab;
    do {
        if(fgets(line, sizeof(line), file) == NULL || strcmp(line, "\n") == 0){
            return head;
        }
        word = strtok(line, " \t\n");
        tab=malloc(strlen(word)*sizeof(char));
        strcpy(tab, word);
        while (word != NULL) {
            InsertInLL(&head,tab);
            word = strtok(NULL, " \t\n");
            if (word != NULL) {
                tab=malloc(strlen(word)*sizeof(char));
                strcpy(tab, word);
            }
        }
        
    } while (strcmp(line, "\n") != 0);
    return head;
}
//--------------------------------------------------------------

void FileToStruct(FILE *file,tabty *Filetab[], int *size) {      //needs variable passage
    tabty *tmp;
    tabty tmp2;
    do {
        *Filetab = realloc(*Filetab,((*size)+1)*sizeof(Node *));
        tmp2 = ParaToStruct(file);
        if (tmp2 != NULL) {
            tmp = (*Filetab) + (*size);
            *tmp = tmp2;
            (*size)++;
        }
    } while (!feof(file)); 
}

//--------------------------------------------------------------

bool SearchInPara(Node *para, char *word) {
    Node *q = para;
    TRNode *p;
    while (q != NULL && toupper(word[0]) <= Value(q)) {
        if ( toupper(word[0]) == Value(q)) {
            TRNode *p = q->tree;
            while (p != NULL) { 
                if (_stricmp(TreeValue(p),word) == 0) {
                    return true;
                }   
                else {
                    if (_stricmp(TreeValue(p),word)>0) {
                        p = LC(p);
                    }
                    else {
                        p = RC(p);
                    }
                }  
            }
        }
        q = Next(q);
    }
    return false;
}

//--------------------------------------------------------------

Node* Union2para(Node * para1, Node * para2) {
    Node * head;
    
    return head;
}

//************************************************************************************

int main () {
    tabty *Filetab;
    Filetab = malloc(sizeof(tabty));
    int size = 0;
    char filename[10] = "file1.txt";
    FILE *f = fopen(filename, "r");
    if (f == NULL) { return 1; }
    FileToStruct(f, &Filetab, &size);
    fclose(f);
    // printing
    int i;
    tabty p;
        printf("\ncheck\n");
    printf("\nsize = %d\n", size);

    for(i=0;i<size;i++) {
        p = Filetab[i];
        while(p != NULL) {
            printTree(p->tree, "", 0);
            printf("\n--------------------------------------------------------------\n");
            p = Next(p);
        }
        printf("\n ******** \n");
    }
    printf("\ncheck\n");
    if (SearchInPara(Filetab[0], "anes") == 1) {
        printf("exist !");
    }
    else {
        printf("doesn't exist");
    }
    printf("\ncheck\n");
    return 0;
}