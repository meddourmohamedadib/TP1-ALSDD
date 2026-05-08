#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "libtp1.h"

//---------------------------------------
typedef Node*  tabty ; 

typedef struct
{
    int size;
    tabty *Filetab;
} Afile;

#include <string.h>
#include <ctype.h>

/*char *NoPunctWord(char *word) {
    int i = 0, j = 0;
    char tmp[strlen(word)];
    strcpy(tmp, word);
    while (word[i] != '\0') {
        if (isalnum((unsigned char)word[i])) {
            tmp[j++] = word[i];
        }
        i++;
    }
    tmp[j] = '\0';
    word = tmp;
    return word;
}*/

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

//--------------------------------------------------------------------------------------------------

void InsertInBST(TRNode *root,char* val) {
    TRNode *p = root;
    TRNode *prev = root;
    //val = NoPunctWord(val);
    while (p != NULL) {
        if(_stricmp(TreeValue(p),val)==0){
            return;
        }
        else {
            prev = p;
            if (_stricmp(TreeValue(p),val)>0) {
                p = LC(p);
            }
            else {
                p = RC(p);
            }
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
        while (word != NULL) {
            NoPunctWord(word);              // filter in-place
            if (strlen(word) > 0) {         // skip pure-punctuation tokens
                tab = malloc(strlen(word) + 1);
                strcpy(tab, word);
                InsertInLL(&head, tab);
            }
            word = strtok(NULL, " \t\n");
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
    while (q != NULL && toupper(word[0]) >= Value(q)) {
        if ( toupper(word[0]) == Value(q) && q->tree != NULL) {
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

void UnionTrav_PreOrd(TRNode * Root, Node ** result) {      //used to do the union of a tree in a para struct with the result 
    if (Root != NULL) {
        if (SearchInPara(*result, TreeValue(Root)) == false) {
            InsertInLL(result, TreeValue(Root));
        }
        UnionTrav_PreOrd(LC(Root), result);
        UnionTrav_PreOrd(RC(Root), result);
    }
}

//--------------------------------------------------------------

void Union2Para(Node *struct1, Node **res){     //it do the union of the para "struct1" with the result
    Node *p = struct1;
    while(p != NULL){
        UnionTrav_PreOrd(p->tree, res);
        p = Next(p);
    }
}

//--------------------------------------------------------------




void search_in_BST(TRNode** root, char* word,TRNode **n,TRNode **parent) {
    *n=*root;
    *parent=NULL;
    while((*n)!=NULL){
        if(_stricmp(TreeValue(*n),word)==0){
            return;
        }
        *parent=*n;
        if(_stricmp(TreeValue(*n),word)>0){
           *n=LC(*n);
        }
        else{
            *n=RC(*n);
        }
    }
}



void delete_from_BST(TRNode** root, char* word, Node *llhead){
    TRNode* n=NULL;
    TRNode* parent=NULL;
    search_in_BST(root,word,&n,&parent);
    if(n==NULL){
        return;
    }
    if(LC(n)==NULL && RC(n)==NULL){
        if(parent==NULL){
            *root=NULL;
            free(n);
            llhead->tree = NULL;
        }
        else{
            if(LC(parent)==(n)){
                Ass_LC(parent, NULL);
            }
            else{
                Ass_RC(parent, NULL);
            }
            free(n);
        }
    }
    else{
        if(LC(n)==NULL && RC(n)!=NULL){
            if(parent==NULL){
                *root=RC(n);
                free(n);
            }
            else{
                if(LC(parent)==(n)){
                    Ass_LC(parent, RC(n));
                }
                else{
                    Ass_RC(parent, RC(n));
                }
                free(n);
            }
        }
        else {
             if(LC(n)!=NULL && RC(n)==NULL){
                if(parent==NULL){
                    *root=LC(n);
                    free(n);
                }else{
                    if(LC(parent)==(n)){
                         Ass_LC(parent, LC(n));
                    }else{
                        Ass_RC(parent, LC(n));
                    }
                    free(n);
                }

            }
            else {
                TRNode* tmp = RC(n);
                parent = n;
                while(LC(tmp)!=NULL){
                    parent=tmp;
                    tmp=LC(tmp);
                }
                char *val = strdup(TreeValue(tmp));  
                AssTreeVal(n, val);
                if(tmp == RC(n)){
                    Ass_RC(n,RC(tmp));
                }else{
                    Ass_LC(parent,RC(tmp));
                }
                free(tmp->string);
                free(tmp);
            }
        }
    }
}

    
//--------------------------------------------------------------------------------------------------

void DeleteByPtr(Node **h, Node * Ptr, Node *prev) {
    Node *p,*q;
    if (Ptr == *h) {
        *h = Next(*h);
    }
    else {
        Ass_adr(prev,Next(Ptr));
    }
    free(Ptr);
}

//--------------------------------------------------------------------------------------------------

void DeleteInPara(Node **para, char *word) {
    Node *q = *para;
    Node *prev = NULL;
    while (q != NULL && toupper(word[0]) >= Value(q)) {
        if ( toupper(word[0]) == Value(q)) {
            delete_from_BST(&(q->tree), word, q);
            // If BST is now empty, remove the LL node
            if (q->tree == NULL) {
                DeleteByPtr(para, q, prev);
            }
            break;
        }
        prev = q;
        q = Next(q);
    }
}

//--------------------------------------------------
/*
void interTrav_PreOrd(TRNode ** Root, Node * struct1, Node ** result) {      //used to do the intersection of a tree in a para struct with the result
    if (*Root != NULL) {
        if (SearchInPara(struct1, TreeValue(*Root)) == false) {
            DeleteInPara(result, TreeValue(*Root));
        }
        if (*Root != NULL) {
            interTrav_PreOrd(&((*Root)->left), struct1, result);
            interTrav_PreOrd(&((*Root)->right), struct1, result);
        }
    }
}

//--------------------------------------

void inter2Para(Node *struct1, Node **res){     //it do the intersection of the para "struct1" with the result
    Node *p = *res;
    while(p != NULL){
        interTrav_PreOrd(&(p->tree), struct1, res);
        p = Next(p);
    }
}*/


//--------------------------------------------------------------------------------------------------



// Helper: collect all strings from a BST into a char* array
void collectWords(TRNode *root, char ***arr, int *size) {
    if (root == NULL) return;
    *arr = realloc(*arr, (*size + 1) * sizeof(char *));
    (*arr)[(*size)++] = TreeValue(root);
    collectWords(LC(root), arr, size);
    collectWords(RC(root), arr, size);
}

void inter2Para(Node *struct1, Node **res) {
    // Step 1: collect all words from res
    char **words = NULL;
    int size = 0;
    Node *p = *res;
    while (p != NULL) {
        collectWords(p->tree, &words, &size);
        p = Next(p);
    }
    // Step 2: delete from res anything not in struct1
    for (int i = 0; i < size; i++) {
        if (!SearchInPara(struct1, words[i])) {
            DeleteInPara(res, words[i]);
        }
    }
    free(words);
}



//************************************************************************************

int main () {
    //Filetab Init
    tabty *Filetab;
    Filetab = malloc(sizeof(tabty));
    int size = 0;

    //file config
    char filename[10] = "file1.txt";
    FILE *f = fopen(filename, "r");
    if (f == NULL) { return 1; }

    FileToStruct(f, &Filetab, &size);
    fclose(f);
    /*
    //Union test
    Node *Union = NULL;
    Union2Para(Filetab[0], &Union);
    Union2Para(Filetab[1], &Union);*/
    //--------------------------------
    tabty p;
    Node* intersection = NULL;
    Union2Para(Filetab[0], &intersection);

    printf("\n\n The first para is  is : \n");
    p = intersection;
    while(p != NULL) {
        printTree(p->tree, "", 0);
        printf("\n--------------------------------------------------------------\n");
        p = Next(p);
    }

    inter2Para(Filetab[1], &intersection);


    printf("\n\n\n//////////////////////////////////////////////////\n\n\n");


    //Union2Para(Filetab[0], &Union);
    // printing
    int i;
    for(i=0;i<size;i++) {
        p = Filetab[i];
        while(p != NULL) {
            printTree(p->tree, "", 0);
            printf("\n--------------------------------------------------------------\n");
            p = Next(p);
        }
        printf("\n ************************************ \n");
    }
    printf("\n\n The intersection is : \n");
    p = intersection;
    while(p != NULL) {
        printTree(p->tree, "", 0);
        printf("\n--------------------------------------------------------------\n");
        p = Next(p);
    } 
    /*
    if (SearchInPara(Filetab[1], "yani") == true) {
        printf("\n\nexits\n\n");
    }
    else {
        printf("\n\ndoesn't exit !\n\n");
    }
    char* tmp="abd?ou.:";
    printf("\nchech\n");
    printf("\n--------------------------------------------------------------\n");
    //printf("%s --> %s", tmp, NoPunctWord(tmp));
    printf("\nchech\n");*/
    return 0;
}