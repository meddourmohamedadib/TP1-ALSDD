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

//--------------------------------------------------------------

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

//--------------------------------------------------------------

void collectWords(TRNode *root, char ***arr, int *size) {
    if (root == NULL) return;
    *arr = realloc(*arr, (*size + 1) * sizeof(char *));
    (*arr)[(*size)++] = strdup(TreeValue(root));
    collectWords(LC(root), arr, size);
    collectWords(RC(root), arr, size);
}

//--------------------------------------------------------------

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

void PrintTree_preord(TRNode * Root) {      
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
        printf("|-  ");
        PrintTree_preord(p->tree);
        printf("  -|");
        printf("\n");
        p = Next(p);
    }
}

//--------------------------------------------------------------

void DiffTrav_PreOrd(TRNode * Root, Node ** result) {      //used to do the intersection of a tree in a para struct with the result
    if (Root != NULL) {
        if (SearchInPara(*result, TreeValue(Root)) == true) {
            DeleteInPara(result, TreeValue(Root));
        }
        DiffTrav_PreOrd(Root->left, result);
        DiffTrav_PreOrd(Root->right, result);
    }
}

//--------------------------------------------------------------

void Diff2Para(Node *struct1, Node **res){     //it do the Difference of the para "struct1" with the result
    Node *p = struct1;
    while(p != NULL){
        DiffTrav_PreOrd(p->tree, res);
        p = Next(p);
    }
}

//--------------------------------------------------------------

void DiffNPara(Apara *arr, int arrsize, Afile *TAB, Node **res) {
    Union2Para(TAB[arr[0].numfile-1].Filetab[arr[0].numpar-1], res);
    for (int i = 1; i< arrsize ; i++) {
        Diff2Para(TAB[arr[i].numfile-1].Filetab[arr[i].numpar-1], res);
    }
}

//--------------------------------------------------------------

void SyDiff2Para(Node *struct1, Node **res) {
    Node *p1 = NULL;
    Union2Para(*res, &p1);
    Inter2Para(struct1, &p1);
    Union2Para(struct1, res);
    Diff2Para(p1, res);
}

//--------------------------------------------------------------

void SyDiffNPara(Apara *arr, int arrsize, Afile *TAB, Node **res) {
    for (int i = 0; i< arrsize ; i++) {
        SyDiff2Para(TAB[arr[i].numfile-1].Filetab[arr[i].numpar-1], res);
    }
}

//-------------------------------------------------------------

void freeBST(TRNode *root) {
    if (root == NULL) return;
    freeBST(LC(root));
    freeBST(RC(root));
    free(root->string);
    free(root);
}

//-------------------------------------------------------------

void freeStruct(Node **head) {
    Node *p = *head;
    while (p != NULL) {
        Node *next = Next(p);
        freeBST(p->tree);
        free(p);
        p = next;
    }
    *head = NULL;
}

//-------------------------------------------------------------

int NumTRNodes(TRNode * Root) { 
    if (Root != NULL) {
        return NumTRNodes(LC(Root)) + NumTRNodes(RC(Root)) + 1;
    }
    else {
        return 0;
    }
}

//--------------------------------------------------------------

int NumStructNodes(Node * struc){
    Node *p = struc;
    int sum = 0;
    while (p != NULL) {
        sum += NumTRNodes(p->tree);
        p = Next(p);
    }
    return sum;
}

//--------------------------------------------------------------

int Allpara(Afile TAB[], int size) {
    int sum = 0;
    for (int i = 0; i<size; i++){
        sum += TAB[i].size;
    }
    return sum;
}

//--------------------------------------------------------------

/* ── STOPWORDS ────────────────────────────────────────────────────────────────
   Loads ~200 common English stopwords directly into the structure.
────────────────────────────────────────────────────────────────────────────── */

static const char *STOPWORDS[] = {
    /* A */
    "a", "about", "above", "across", "after", "afterwards", "again", "against",
    "all", "almost", "alone", "along", "already", "also", "although", "always",
    "am", "among", "amongst", "an", "and", "another", "any", "anyhow", "anyone",
    "anything", "anyway", "anywhere", "are", "around", "as", "at",
    /* B */
    "be", "because", "been", "before", "beforehand", "behind", "being", "below",
    "beside", "besides", "between", "beyond", "both", "but", "by",
    /* C */
    "can", "cannot", "cant", "could", "couldnt",
    /* D */
    "did", "didnt", "do", "does", "doesnt", "doing", "done", "dont", "down",
    "during",
    /* E */
    "each", "either", "else", "elsewhere", "enough", "even", "ever", "every",
    "everyone", "everything", "everywhere", "except",
    /* F */
    "few", "for", "former", "formerly", "from", "further",
    /* G */
    "get", "got",
    /* H */
    "had", "hadnt", "has", "hasnt", "have", "havent", "having", "he", "hed",
    "hell", "hence", "her", "here", "hereafter", "hereby", "herein", "heres",
    "hereupon", "hers", "herself", "him", "himself", "his", "how", "however",
    /* I */
    "i", "id", "if", "ill", "im", "in", "indeed", "into", "is", "isnt", "it",
    "itd", "itll", "its", "itself", "ive",
    /* J */
    "just",
    /* L */
    "last", "latter", "latterly", "least", "less", "let", "like", "likely",
    /* M */
    "many", "may", "me", "meanwhile", "might", "more", "moreover", "most",
    "mostly", "much", "must", "my", "myself",
    /* N */
    "namely", "neither", "never", "nevertheless", "next", "no", "nobody",
    "none", "nor", "not", "nothing", "now", "nowhere",
    /* O */
    "of", "off", "often", "on", "once", "only", "onto", "or", "other",
    "others", "otherwise", "our", "ours", "ourselves", "out", "over", "own",
    /* P */
    "per", "perhaps",
    /* Q */
    "quite",
    /* R */
    "rather", "re",
    /* S */
    "same", "seem", "seemed", "seeming", "seems", "several", "she", "shed",
    "shell", "shes", "should", "shouldnt", "since", "so", "some", "somehow",
    "someone", "something", "sometime", "sometimes", "somewhere", "still",
    "such",
    /* T */
    "than", "that", "thatll", "thats", "the", "their", "theirs", "them",
    "themselves", "then", "thence", "there", "thereafter", "thereby",
    "therefore", "therein", "thereupon", "these", "they", "theyd", "theyll",
    "theyre", "theyve", "this", "those", "though", "through", "throughout",
    "thru", "thus", "to", "together", "too", "toward", "towards",
    /* U */
    "under", "until", "up", "upon", "us",
    /* V */
    "very",
    /* W */
    "was", "wasnt", "we", "wed", "well", "were", "werent", "weve", "what",
    "whatever", "when", "whence", "whenever", "where", "whereafter", "whereas",
    "whereby", "wherein", "whereupon", "wherever", "whether", "which", "while",
    "who", "whoever", "whole", "whom", "whose", "why", "will", "with",
    "within", "without", "wont", "would", "wouldnt",
    /* Y */
    "yet", "you", "youd", "youll", "your", "youre", "yours", "yourself",
    "yourselves", "youve",
    NULL   /* sentinel — marks end of list */
};

//--------------------------------------------------------------

void loadStopwords(Node **sw) {
    int i = 0;
    while (STOPWORDS[i] != NULL) {
        char *copy = strdup(STOPWORDS[i]); 
        InsertInLL(sw, copy);
        i++;
    }
}

//--------------------------------------------------------------

float Similarity(Apara *arr, int arrsize, Afile *TAB) {
    Node *uni   = NULL;
    Node *inter = NULL;
    Node *StopWords = NULL;
    loadStopwords(&StopWords);
    // compute union
    UnionNPara(arr, arrsize, TAB, &uni);
    Diff2Para(StopWords, &uni);
    // compute intersection
    InterNPara(arr, arrsize, TAB, &inter);
    Diff2Para(StopWords, &inter);

    float u = NumStructNodes(uni);
    float n = NumStructNodes(inter);

    freeStruct(&uni);
    freeStruct(&inter);

    if (u == 0) return 0.0f;
    return n / u;
}

//--------------------------------------------------------------

void FillFileTrav_preord(TRNode * Root, FILE *file) {      //used to do the union of a tree in a para struct with the result 
    if (Root != NULL) {
        fprintf(file, "%s | ",TreeValue(Root));
        FillFileTrav_preord(LC(Root), file);
        FillFileTrav_preord(RC(Root), file);
    }
}

//--------------------------------------------------------------

void fillfile(Node * struc, char *path){
    Node *p = struc;
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("Could not create file!\n");
        return;
    }
    while (p != NULL) {
        FillFileTrav_preord(p->tree, file);
        fprintf(file, "\n");
        p = Next(p);
    }
    fclose(file);
    printf("  Result saved to '%s'\n", path);
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

/* ── SMALL LOADING BAR ─────────────────────────────────────────── */

void loadingAnimation() {
    printf("Loading");
    for (int i = 0; i < 5; i++) {
        printf(".");
        fflush(stdout); // force print immediately
        Sleep(300);
    }
    printf("\n");
}

//************************************************************************************

//--------------------------------------------------------------

int main () {

    

    //.. declarations
    char *menu[] = {
        "   +----------------------------------+",
        "   |           - WELCOME -            |",
        "   +----------------------------------+",
        "   1- Upload Files                        ",
        "      2- Choose paragraphs         ",
        "         3- Choose operation           ",
        "            4- EXIT                    "
    };
    char *OPmenu[] = {
        "   +----------------------------------+",
        "   |       Choose an operation :      |",
        "   +----------------------------------+",
        "         1- Union                      ",
        "           2- Intersection             ",
        "             3- Difference             ",
        "               4- Symetric Difference  ",
        "                 5- Similarity Check   ",
        "                   6- Back to MENU       "
    };
    char *SWmenu[] = {
        "   Do you want to include Stopwords in the operation ?",
        "       1 -> Yes",
        "          2 -> No"
    };
    char *savemenu[] = {
        "   Do you want to save the result in a file ?",
        "       1 -> Yes",
        "          2 -> No"
    };
    int i,j,choice,OPchoice,Schoice,SWchoice=0;
    int filesnum,parsnum;
    float SimRatio = 0;
    Afile *TABf = NULL;
    Apara *TABp = NULL;
    FILE *file;
    char* tmp_filename;
    bool bool1=false,bool2=false, SWbool = false;
    Node *Uni = NULL;
    Node *Inter = NULL;
    Node *Diff = NULL;
    Node* SyDiff=NULL;
    Node* StopW = NULL;
    char* path;

    

    welcome();
    do
    {
        do
        {
            system("cls");
            for (i = 0; i < 7; i++) {
                printf("\n%s", menu[i]);
                Sleep(100);
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
            free(TABf);
            printf("Enter the path of each file : \n");
            printf("[example : C:\\Users\\user\\Desktop\\filename.txt]\n\n");
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
            bool2 = false;
            printf("\n  -> Done !\n");
            Sleep(2000);
            break;
        case 2:
            if(bool1){
                free(TABp);
                do
                {
                    system("cls");
                    printf("how many paragraphes will you use :");
                    scanf("%d",&parsnum);
                } while (parsnum > Allpara(TABf, filesnum) || parsnum < 1);
                TABp = malloc(parsnum*sizeof(Apara));
                for(i=0;i<parsnum;i++){
                    system("cls");
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
                printf("-------------------------------------------------");
                printf("\nthe paragraphs chosed : ");
                for (j = 0; j<i; j++){
                    printf("(%d : %d) | ",TABp[j].numfile,TABp[j].numpar);
                }
                bool2 = true;
                printf("\n  -> Done !\n");
            }
            else{
                printf("   you can't enter para befor files pleas do choice 1- Add files first");    
            };
            printf("    \nPress Any key to continue");
            getch();
            break;
        case 3:
            if(bool1 && bool2){
                do
                {
                    system("cls");
                    for (i = 0; i < 9; i++) {
                        printf("\n%s", OPmenu[i]);
                        Sleep(100);
                    }
                    printf("\n\n   Your Choice : ");
                    scanf("%d", &OPchoice);
                    if (OPchoice > 6 || OPchoice < 1) {
                        system("cls");
                        printf("\n   Incorrect choice !");
                        Sleep(1000);
                    }
                } while (OPchoice > 6 || OPchoice < 1);
                switch (OPchoice)
                {
                case 1:
                    loadingAnimation();
                    freeStruct(&Uni);
                    //stopwords inclusion
                    do
                    {
                        for (i = 0; i < 3; i++) {
                            printf("\n%s", SWmenu[i]);
                            Sleep(100);
                        }
                        printf("\n   Your Choice : ");
                        scanf("%d", &SWchoice);
                        if (SWchoice > 2 || SWchoice < 1) {
                            system("cls");
                            printf("\n   Incorrect choice !\n");
                            Sleep(1000);
                        }
                    } while (SWchoice > 2 || SWchoice < 1);
                    UnionNPara(TABp,parsnum,TABf,&Uni);
                    if (SWchoice == 2) {
                        if(!SWbool){
                            loadStopwords(&StopW);
                            SWbool = true;
                        }
                        Diff2Para(StopW, &Uni);
                    }
                    printf(" The result of the Union is : \n");
                    PrintStruct(Uni);
                    printf("\n          -> Done !\n");
                    //-----------------------------------------
                    // saving the result menu
                    do
                    {
                        for (i = 0; i < 3; i++) {
                            printf("\n%s", savemenu[i]);
                            Sleep(100);
                        }
                        printf("\n   Your Choice : ");
                        scanf("%d", &Schoice);
                        if (Schoice > 2 || Schoice < 1) {
                            system("cls");
                            printf("\n   Incorrect choice !");
                            Sleep(1000);
                        }
                    } while (Schoice > 2 || Schoice < 1);
                    if (Schoice == 1) {
                        printf("\nenter the path : ");
                        printf("\n[example : C:\\Users\\user\\Desktop\\filename.txt]\n");
                        printf("--> ");
                        scanf("%s",&path);
                        fillfile(Uni, path);
                    }   
                    printf("    \nPress Any key to continue");
                    getch();
                    break;
                case 2:
                    loadingAnimation();
                    freeStruct(&Inter);
                    //stopwords inclusion
                    do
                    {
                        for (i = 0; i < 3; i++) {
                            printf("\n%s", SWmenu[i]);
                            Sleep(100);
                        }
                        printf("\n   Your Choice : ");
                        scanf("%d", &SWchoice);
                        if (SWchoice > 2 || SWchoice < 1) {
                            system("cls");
                            printf("\n   Incorrect choice !\n");
                            Sleep(1000);
                        }
                    } while (SWchoice > 2 || SWchoice < 1);
                    InterNPara(TABp, parsnum, TABf, &Inter);
                    if (SWchoice == 2) {
                        if(!SWbool){
                            loadStopwords(&StopW);
                            SWbool = true;
                        }
                        Diff2Para(StopW, &Inter);
                    }
                    printf("\n  --> The result of the Intersection is : <--\n");
                    PrintStruct(Inter);
                    printf("\n          -> Done !\n");
                    // saving the result menu
                    do
                    {
                        for (i = 0; i < 3; i++) {
                            printf("\n%s", savemenu[i]);
                            Sleep(100);
                        }
                        printf("\n   Your Choice : ");
                        scanf("%d", &Schoice);
                        if (Schoice > 2 || Schoice < 1) {
                            system("cls");
                            printf("\n   Incorrect choice !");
                            Sleep(1000);
                        }
                    } while (Schoice > 2 || Schoice < 1);
                    if (Schoice == 1) {
                        printf("\nenter the path : ");
                        printf("\n[example : C:\\Users\\user\\Desktop\\filename.txt]\n");
                        printf("--> ");
                        scanf("%s",&path);
                        fillfile(Inter, path);
                    }
                    printf("    \nPress Any key to continue");
                    getch();
                    break;
                case 3:
                    loadingAnimation();
                    freeStruct(&Diff);
                    DiffNPara(TABp, parsnum, TABf, &Diff);
                    printf("\n  --> The result of the Difference is : <--\n");
                    PrintStruct(Diff);
                    printf("\n          -> Done !\n");
                    // saving the result menu
                    do
                    {
                        for (i = 0; i < 3; i++) {
                            printf("\n%s", savemenu[i]);
                            Sleep(100);
                        }
                        printf("\n   Your Choice : ");
                        scanf("%d", &Schoice);
                        if (Schoice > 2 || Schoice < 1) {
                            system("cls");
                            printf("\n   Incorrect choice !");
                            Sleep(1000);
                        }
                    } while (Schoice > 2 || Schoice < 1);
                    if (Schoice == 1) {
                        printf("\nenter the path : ");
                        printf("\n[example : C:\\Users\\user\\Desktop\\filename.txt]\n");
                        printf("--> ");
                        scanf("%s",&path);
                        fillfile(Diff, path);
                    }
                    printf("    \nPress Any key to continue");
                    getch();
                    break;
                case 4:
                    loadingAnimation();
                    freeStruct(&SyDiff);
                    SyDiffNPara(TABp, parsnum, TABf, &SyDiff);
                    printf("\n  --> The result of the Symetric Difference is : <--\n");
                    PrintStruct(SyDiff);
                    printf("\n          -> Done !\n");
                    // saving the result menu
                    do
                    {
                        for (i = 0; i < 3; i++) {
                            printf("\n%s", savemenu[i]);
                            Sleep(100);
                        }
                        printf("\n   Your Choice : ");
                        scanf("%d", &Schoice);
                        if (Schoice > 2 || Schoice < 1) {
                            system("cls");
                            printf("\n   Incorrect choice !");
                            Sleep(1000);
                        }
                    } while (Schoice > 2 || Schoice < 1);
                    if (Schoice == 1) {
                        printf("\nenter the path : ");
                        printf("\n[example : C:\\Users\\user\\Desktop\\filename.txt]\n");
                        printf("--> ");
                        scanf("%s",&path);
                        fillfile(SyDiff, path);
                    }
                    printf("    \nPress Any key to continue");
                    getch();
                    break;
                case 5:
                    SimRatio = Similarity(TABp, parsnum, TABf);
                    printf("\n+--------------------------------------+\n");
                    printf("|  the paragraphs are %.2f %% similar  |\n", SimRatio*100.0f);
                    printf("+--------------------------------------+\n");
                    printf("    \nPress Any key to continue");
                    getch();
                    break;
                default:
                    break;
                }
            }
            else {
                printf("    You can't perform operations yet !\n     Missing DATA !\n");
                Sleep(2000);
            }
        break;
        case 4:
            freeStruct(&Uni);
            freeStruct(&Inter);
            freeStruct(&Diff);
            freeStruct(&SyDiff);
            free(TABf);
            free(TABp);
        break;
        
        default:
            break;
        }
    } while (choice != 4);
    printf("\nClosing");
    for (int i = 0; i < 5; i++) {
        printf(".");
        fflush(stdout); 
        Sleep(300);
    }
    printf("\n");
    printf("\nGood Bye !!");
    return 0;
}