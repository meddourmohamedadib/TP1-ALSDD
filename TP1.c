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

char *NoPunctWord(char *word) {         //it still has a prob
    int i = 0, j = 0;
     printf("\nchech\n");
    while (word[i] != '\0') {
         printf("\nchech\n");
        if (isalnum((unsigned char)word[i]) == true) {
            word[j++] = word[i];
        }
        i++;
    }
     printf("\nchech\n");
    word[j] = '\0';
    printf("\nchech\n");
    return word;
}

/*void InsertInBST(TRNode *root,char* val) {
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
}*/

//--------------------------------------------------------------------------------------------------

void InsertInBST(TRNode *root,char* val) {
    TRNode *p = root;
    TRNode *prev = root;
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
    while (q != NULL && toupper(word[0]) >= Value(q)) {
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

    //Union test
    Node *Union = NULL;
    Union2Para(Filetab[0], &Union);
    Union2Para(Filetab[1], &Union);
    //Union2Para(Filetab[0], &Union);
    // printing
    int i;
    tabty p;
    for(i=0;i<size;i++) {
        p = Filetab[i];
        while(p != NULL) {
            printTree(p->tree, "", 0);
            printf("\n--------------------------------------------------------------\n");
            p = Next(p);
        }
        printf("\n ************************************ \n");
    }
    printf("\n\n The union is : \n");
    p = Union;
    while(p != NULL) {
        printTree(p->tree, "", 0);
        printf("\n--------------------------------------------------------------\n");
        p = Next(p);
    } 
    
    if (SearchInPara(Filetab[1], "yani") == true) {
        printf("\n\nexits\n\n");
    }
    else {
        printf("\n\ndoesn't exit !\n\n");
    }
    char* tmp="abd?ou.:";
                    printf("\nchech\n");
            printf("\n--------------------------------------------------------------\n");
            printf("%s --> %s", tmp, NoPunctWord(tmp));
             printf("\nchech\n");
    return 0;
}