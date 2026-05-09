#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <conio.h>
#include "libtp1.h"

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

Node *ParaToStruct(FILE *file) {
    Node *head = NULL;
    char line[1024];
    char *word, *tab;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strcmp(line, "\n") == 0 || strcmp(line, "\r\n") == 0)
            break;                          // blank line = end of paragraph
        word = strtok(line, " \t\n\r");
        while (word != NULL) {
            NoPunctWord(word);
            if (strlen(word) > 0) {
                tab = malloc(strlen(word) + 1);
                strcpy(tab, word);
                InsertInLL(&head, tab);
            }
            word = strtok(NULL, " \t\n\r");
        }
    }
    return head;
}

void FileToStruct(FILE *file, Node* *Filetab[], int *size) {
    Node *tmp2;
    Node **tmp;
    while (!feof(file)) {                   // check before reading
        tmp2 = ParaToStruct(file);
        if (tmp2 != NULL) {
            *Filetab = realloc(*Filetab, (*size + 1) * sizeof(Node *));
            tmp = (*Filetab) + (*size);
            *tmp = tmp2;
            (*size)++;
        }
    }
}
//--------------------------------------------------------------

/*void FileToStruct(FILE *file,Node* *Filetab[], int *size) {      //needs variable passage
    Node* *tmp;
    Node* tmp2;
    do {
        *Filetab = realloc(*Filetab,((*size)+1)*sizeof(Node *));
        tmp2 = ParaToStruct(file);
        if (tmp2 != NULL) {
            tmp = (*Filetab) + (*size);
            *tmp = tmp2;
            (*size)++;
        }
    } while (!feof(file)); 
}*/

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

void UnionTrav_PreOrd(TRNode *Root, Node **result) {
    if (Root != NULL) {
        if (!SearchInPara(*result, TreeValue(Root))) {
            char *copy = strdup(TreeValue(Root));
            InsertInLL(result, copy);
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

void UnionNPara(Apara *arr, int arrsize, Afile *TAB, Node **res) {
    for (int i = 0; i< arrsize ; i++) {
        Union2Para(TAB[arr[i].numfile-1].Filetab[arr[i].numpar-1], res);
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
    (*arr)[(*size)++] = strdup(TreeValue(root));
    collectWords(LC(root), arr, size);
    collectWords(RC(root), arr, size);
}

void Inter2Para(Node *struct1, Node **res) {
    char **words = NULL;
    int size = 0;
    Node *p = *res;
    while (p != NULL) {
        collectWords(p->tree, &words, &size);
        p = Next(p);
    }
    for (int i = 0; i < size; i++) {
        if (!SearchInPara(struct1, words[i])) {
            DeleteInPara(res, words[i]);
        }
        free(words[i]);
    }
    free(words);
}

//--------------------------------------------------------------

void InterNPara(Apara *arr, int arrsize, Afile *TAB, Node **res) {
    Union2Para(TAB[arr[0].numfile-1].Filetab[arr[0].numpar-1], res);
    for (int i = 1; i< arrsize ; i++) {
        Inter2Para(TAB[arr[i].numfile-1].Filetab[arr[i].numpar-1], res);
    }
}

//--------------------------------------------------------------

void PrintTree_preord(TRNode * Root) {      //used to do the union of a tree in a para struct with the result 
    if (Root != NULL) {
        printf("%s | ", TreeValue(Root));
        PrintTree_preord(LC(Root));
        PrintTree_preord(RC(Root));
    }
}

//--------------------------------------------------------------

void PrintStruct(Node * struc){
    Node *p = struc;
    while (p != NULL) {
        PrintTree_preord(p->tree);
        printf("\n");
        p = Next(p);
    }
}

//--------------------------------------------------------------




//************************************************************************************

void Color(int text, int bg) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), bg * 16 + text);
}

/* ── 1. LOADING BAR ───────────────────────────────────────────── */
void loading_bar() {
    char *labels[] = {
        "Initializing...              ",
        "Loading memory allocator...  ",
        "Building linked-list engine..",
        "Loading BST module...        ",
        "Reading paragraph structures.",
        "Compiling set operations...  ",
        "Finalizing...                ",
        "Done.                        "
    };
    int pcts[] = {5, 20, 35, 50, 65, 80, 95, 100};
    int total_bar = 50;
    int i, j;

    for (i = 0; i < 8; i++) {
        system("cls");
        Color(7, 0);
        printf("\n\n\n\n\n\n\n\n\n");
        printf("\t\t\t\t\t  LOADING\n\n");

        int filled = (pcts[i] * total_bar) / 100;
        printf("\t\t\t\t\t [");
        Color(10, 0);
        for (j = 0; j < filled; j++)  printf("|");
        Color(8, 0);
        for (j = filled; j < total_bar; j++) printf("-");
        Color(7, 0);
        printf("] %3d%%\n\n", pcts[i]);
        printf("\t\t\t\t\t  %s\n", labels[i]);
        Sleep(500);
    }
    Sleep(400);
}

/* ── 2. LANDING CARD ──────────────────────────────────────────── */
void landing_card() {
    system("cls");
    printf("\n\n");
    Color(3, 0);
    printf("\t   /$$$$$$$$  /$$$$$$  /$$$$$$      \n");
    printf("\t  | $$_____/ /$$__  $$|_  $$_/      \n");
    printf("\t  | $$      | $$  \\__/  | $$        \n");
    printf("\t  | $$$$$   |  $$$$$$   | $$        \n");
    printf("\t  | $$__/    \\____  $$  | $$        \n");
    printf("\t  | $$       /$$  \\ $$  | $$        \n");
    printf("\t  | $$$$$$$$|  $$$$$$/ /$$$$$$      \n");
    printf("\t  |________/ \\______/ |______/      \n");
    printf("\t                                    \n");
    printf("\t                                    \n");
    printf("\t  Ecole nationale Superieure d'Informatique\n\n");

    Color(7, 0);
    printf("\t  ---------------------------------------------------------------\n");
    printf("\t  |                                                             |\n");
    printf("\t  |  Module    :  Algorithmics & Dynamic Data Structures        |\n");
    printf("\t  |  Lab Work  :  TP1 - Set Theory on Linguistic Structures     |\n");
    printf("\t  |                                                             |\n");
    printf("\t  |  Member 01 :  GHILASSEN Abdelhalim                          |\n");
    printf("\t  |  Member 02 :  MEDDOUR Mohamed Adib                          |\n");
    printf("\t  |                                                             |\n");
    printf("\t  |  Section   :  C          Group : 10                         |\n");
    printf("\t  |  Year      :  2025 / 2026                                   |\n");
    printf("\t  |                                                             |\n");
    printf("\t  |  Supervised by : Mrs. CHADER Asma                           |\n");
    printf("\t  |                                                             |\n");
    printf("\t  ---------------------------------------------------------------\n\n");

    Color(6, 0);
    printf("\t\t\t     Press any key to continue . . .\n");
    Color(7, 0);
    getch();
}

/* ── MAIN ENTRY POINT ─────────────────────────────────────────── */
void welcome() {
    loading_bar();
    landing_card();
    system("cls");
}

//************************************************************************************

int main () {

    //.. declarations
    char *menu[] = {
        "   ------------------------------------",
        "   |           - WELCOME -            |",
        "   ------------------------------------",
        "   1- Add Files                        ",
        "      2- Choose the paragraphs         ",
        "         3- Choose operation           ",
        "            4- EXIT                    "
    };
    char *OPmenu[] = {
        "   ------------------------------------",
        "     -- Choose an operation :   ",
        "   ------------------------------------",
        "         1- Union                      ",
        "           2- Intersection             ",
        "             3- Difference             ",
        "               4- Symetric Difference  ",
        "                 5- Back to MENU       "
    };
    int i,j,choice,OPchoice;
    int filesnum,parsnum;
    Afile *TABf = NULL;
    Apara *TABp = NULL;
    FILE *file;
    char* tmp_filename;
    bool bool1=false,bool2=false;
    Node *Uni = NULL;
    Node *Inter = NULL;

    

    welcome();
    do
    {
        do
        {
            system("cls");
            for (i = 0; i < 7; i++) {
                printf("\n%s", menu[i]);
                Sleep(200);
            }
            printf("\n   Your Choice : ");
            scanf("%d", &choice);
            if (choice > 4 || choice < 1) {
                system("cls");
                printf("\n   Incorrect choice !");
                Sleep(1000);
            }
        } while (choice > 4 || choice < 1);
        switch (choice)
        {
        case 1:
            do
            {
                system("cls");
                printf("How many files you want to apply operations on ?\n");
                printf("--> ");
                scanf("%d", &filesnum);
            } while (filesnum <= 0);
            //TAB = malloc(sizeof(Afile));
            printf("Enter the path of each file : \n");
            for (i = 0; i < filesnum; i++) {
                TABf = realloc(TABf, (i+1)*sizeof(Afile));
                do
                {   
                    printf("--> ");
                    scanf("%s", TABf[i].filename);
                    file = fopen(TABf[i].filename, "r");
                    if (file == NULL) {
                        printf(" Can't open this file !! try again \n");
                    }
                } while (file == NULL);
                TABf[i].Filetab = NULL;
                TABf[i].size = 0;
                FileToStruct(file, &(TABf[i].Filetab),&(TABf[i].size));
                fclose(file);
            }
            bool1=true;
            printf("\nDone !\n");
            Sleep(1500);
            break;
        case 2:
            if(bool1){
                system("cls");
                printf("how many paragraphes will you use :");
                scanf("%d",&parsnum);
                TABp = malloc(parsnum*sizeof(Apara));
                for(i=0;i<parsnum;i++){
                    system("cls");
                    printf("i = %d\n", i);
                    printf("\n\nthe files that you had entred are :\n\n");
                    for(j=0;j < filesnum;j++){
                        printf("%d --> %s\n",(j+1),TABf[j].filename);
                    }
                    if (i != 0) {
                        printf("the paragraphs chosed : ");
                        for (j = 0; j<i; j++){
                            printf("(%d : %d) | ",TABp[j].numfile,TABp[j].numpar);
                        }
                        printf("\n");
                    }
                    do
                    {
                        printf("enter the file : ");
                        scanf("%d",&TABp[i].numfile);
                    } while (TABp[i].numfile <= 0 || TABp[i].numfile > filesnum);
                    do
                    {
                        printf("enter the paragraph : ");
                        scanf("%d",&TABp[i].numpar);
                    } while (TABp[i].numpar <= 0 || TABp[i].numpar > TABf[(TABp[i].numfile)-1].size);
                }
                printf("the paragraphs chosed : ");
                for (j = 0; j<i; j++){
                    printf("(%d : %d) | ",TABp[j].numfile,TABp[j].numpar);
                }
                bool2 = true;
                printf("\nDone !\n");
            }
            else{
                printf("   you can't enter para befor files pleas do choice 1- Add files first");    
            };
            Sleep(2000);
            break;
        case 3:
            if(bool1 && bool2){
                do
                {
                    system("cls");
                    for (i = 0; i < 8; i++) {
                        printf("\n%s", OPmenu[i]);
                        Sleep(200);
                    }
                    printf("\n   Your Choice : ");
                    scanf("%d", &OPchoice);
                    if (OPchoice > 5 || OPchoice < 1) {
                        system("cls");
                        printf("\n   Incorrect choice !");
                        Sleep(1000);
                    }
                } while (OPchoice > 5 || OPchoice < 1);
                switch (OPchoice)
                {
                case 1:
                    UnionNPara(TABp,parsnum,TABf,&Uni);
                    printf(" The result of the Union is : \n");
                    PrintStruct(Uni);
                    printf("\nDone !\n");
                    Sleep(3000);
                    break;
                case 2:
                    InterNPara(TABp, parsnum, TABf, &Inter);
                    printf(" The result of the Intersection is : \n");
                    PrintStruct(Inter);
                    printf("\nDone !\n");
                    Sleep(3000);
                    break;
                case 3:
                    /* code */
                    break;
                case 4:
                    /* code */
                    break;
                case 5:
                    /* code */
                    break;
                default:
                    break;
                }
            }
            else {
                printf("    You can't perform operations yet !\n     there is no data !\n");
                Sleep(3000);
            }
        break;
        case 4:
            /* code */
        break;
        
        default:
            break;
        }
    } while (choice != 4);
    
    


































    /*
    //Filetab Init
    Node* *Filetab;
    Filetab = malloc(sizeof(Node*));
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
    //--------------------------------
    Node* p;
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