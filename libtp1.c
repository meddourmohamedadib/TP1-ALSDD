#include "libtp1.h"

//************************************************************************************
//              -- ABSTRACT MACHINE : LINKED LIST OPERATIONS --
//************************************************************************************

void Ass_val(Node *p, char val) {
    p->value = val;
}

void Ass_adr(Node *p, Node *add) {
    p->address = add;
}

Node *Next(Node *p) {
    return (p->address);
}

char Value(Node *p) {
    return (p->value);
}

// -- Allocates a new LL node for a word : sets its letter, links it to NULL,
//    allocates its BST root, and stores the word as the BST root value --
void Allocate(Node **p, char *word) {
    *p = (Node *)malloc(sizeof(Node));
    Ass_val(*p, toupper(word[0]));
    Ass_adr(*p, NULL);
    TRNodeAlloc(&((*p)->tree));
    AssTreeVal(((*p)->tree), word);
}

//************************************************************************************
//                  -- ABSTRACT MACHINE : BST OPERATIONS --
//************************************************************************************

void TRNodeAlloc(TRNode **p) {
    *p = (TRNode *)malloc(sizeof(TRNode));
    (*p)->right = NULL;
    (*p)->left = NULL;
}

char *TreeValue(TRNode *p) {
    return p->string;
}

TRNode *LC(TRNode *head) {
    return head->left;
}

TRNode *RC(TRNode *head) {
    return head->right;
}

void AssTreeVal(TRNode *head, char *val) {
    head->string = val;
}

void Ass_LC(TRNode *head, TRNode *child) {
    head->left = child;
}

void Ass_RC(TRNode *head, TRNode *child) {
    head->right = child;
}

//************************************************************************************
//                       -- WORD PREPROCESSING --
//************************************************************************************

// -- Filters a word in-place : keeps only alphanumeric characters,
//    discards punctuation. Returns the same pointer (modified) --
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
    tmp = word + j;
    *tmp = '\0';
    return word;
}

//************************************************************************************
//                       -- INSERTION OPERATIONS --
//************************************************************************************

// -- Inserts a word into an existing BST.
//    Uses case-insensitive comparison (_stricmp).
void InsertInBST(TRNode *root, char *val) {
    TRNode *p = root;
    TRNode *prev = root;
    while (p != NULL) {
        if (_stricmp(TreeValue(p), val) == 0) {
            return;                             
        }
        else {
            prev = p;
            if (_stricmp(TreeValue(p), val) > 0) {
                p = LC(p);                      
            }
            else {
                p = RC(p);                      
            }
        }
    }
    if (_stricmp(TreeValue(prev), val) > 0) {
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

// -- Inserts a word into the LL-of-BST structure.
//    The LL is sorted by uppercase first letter.
//    If the Node for that letter exists, delegates to InsertInBST.
//    Otherwise creates a new LL node for that letter --
void InsertInLL(Node **head, char *string) {
    Node *p = *head, *prev = *head;
    // -- empty list : allocate the first node directly --
    if (*head == NULL) {
        Allocate(head, string);
        return;
    }
    // -- walk until we find the right Node or pass it --
    while ((p != NULL) && (Value(p) <= toupper(string[0]))) {
        prev = p;
        if ((toupper(string[0])) == Value(p)) {
            InsertInBST(p->tree, string);       
            return;
        }
        p = Next(p);
    }
    // -- insert before the head (new smallest letter) --
    if (p == (*head)) {
        Allocate(&p, string);
        *head = p;
        Ass_adr(p, prev);
    }
    // -- insert between prev and p --
    else {
        Allocate(&p, string);
        Ass_adr(p, Next(prev));
        Ass_adr(prev, p);
    }
    return;
}

//************************************************************************************
//                          -- FILE PARSING --
//************************************************************************************

// -- Reads one paragraph from the file (lines separated by a blank line).
//    Each word is filtered by NoPunctWord before insertion.
//    Returns the head of the built LL-of-BST structure, or NULL if empty --
Node *ParaToStruct(FILE *file) {
    Node *head = NULL;
    char line[1024];
    char *word, *tab;

    while (fgets(line, sizeof(line), file) != NULL) {
        // -- blank line signals end of paragraph --
        if (strcmp(line, "\n") == 0 || strcmp(line, "\r\n") == 0)
            break;
        word = strtok(line, " \t\n\r");
        while (word != NULL) {
            NoPunctWord(word);                  // -- remove punctuation --
            if (strlen(word) > 0) {             // -- skip words that became empty --
                tab = malloc(strlen(word) + 1);
                strcpy(tab, word);
                InsertInLL(&head, tab);
            }
            word = strtok(NULL, " \t\n\r");
        }
    }
    return head;
}

// -- Reads all paragraphs from a file and fills the dynamic array Filetab.
//    Each non-empty paragraph returned by ParaToStruct is stored at index *size,
//    then *size is incremented --
void FileToStruct(FILE *file, Node **Filetab[], int *size) {
    Node *tmp2;
    Node **tmp;
    while (!feof(file)) {
        tmp2 = ParaToStruct(file);
        if (tmp2 != NULL) {
            *Filetab = realloc(*Filetab, (*size + 1) * sizeof(Node *));
            tmp = (*Filetab) + (*size);
            *tmp = tmp2;
            (*size)++;
        }
    }
}

//************************************************************************************
//                         -- SEARCH OPERATIONS --
//************************************************************************************

// -- Searches for a word in a LL-of-BST paragraph structure.
//    First locates the right letter Node in the LL,
//    then performs a BST search within that Node.
//    Returns true if found, false otherwise --
bool SearchInPara(Node *para, char *word) {
    Node *q = para;
    TRNode *p;
    while (q != NULL && toupper(word[0]) >= Value(q)) {
        if (toupper(word[0]) == Value(q) && q->tree != NULL) {
            // -- correct letter Node found, search the BST --
            TRNode *p = q->tree;
            while (p != NULL) {
                if (_stricmp(TreeValue(p), word) == 0) {
                    return true;               
                }
                else {
                    if (_stricmp(TreeValue(p), word) > 0) {
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

// -- Searches for a word in a BST.
//    Returns the found node in *n and its parent in *parent.
//    If not found, *n is NULL --
void search_in_BST(TRNode **root, char *word, TRNode **n, TRNode **parent) {
    *n = *root;
    *parent = NULL;
    while ((*n) != NULL) {
        if (_stricmp(TreeValue(*n), word) == 0) {
            return;                         
        }
        *parent = *n;
        if (_stricmp(TreeValue(*n), word) > 0) {
            *n = LC(*n);
        }
        else {
            *n = RC(*n);
        }
    }
}

//************************************************************************************
//                         -- DELETION OPERATIONS --
//************************************************************************************

void delete_from_BST(TRNode **root, char *word, Node *llhead) {
    TRNode *n = NULL;
    TRNode *parent = NULL;
    search_in_BST(root, word, &n, &parent);
    if (n == NULL) {
        return;                                
    }
    // -- Case 1 : leaf node --
    if (LC(n) == NULL && RC(n) == NULL) {
        if (parent == NULL) {
            *root = NULL;
            free(n);
            llhead->tree = NULL;               
        }
        else {
            if (LC(parent) == (n)) {
                Ass_LC(parent, NULL);
            }
            else {
                Ass_RC(parent, NULL);
            }
            free(n);
        }
    }
    else {
        // -- Case 2a : only right child --
        if (LC(n) == NULL && RC(n) != NULL) {
            if (parent == NULL) {
                *root = RC(n);
                free(n);
            }
            else {
                if (LC(parent) == (n)) {
                    Ass_LC(parent, RC(n));
                }
                else {
                    Ass_RC(parent, RC(n));
                }
                free(n);
            }
        }
        else {
            // -- Case 2b : only left child --
            if (LC(n) != NULL && RC(n) == NULL) {
                if (parent == NULL) {
                    *root = LC(n);
                    free(n);
                }
                else {
                    if (LC(parent) == (n)) {
                        Ass_LC(parent, LC(n));
                    }
                    else {
                        Ass_RC(parent, LC(n));
                    }
                    free(n);
                }
            }
            // -- Case 3 : two children
            else {
                TRNode *tmp = RC(n);
                parent = n;
                while (LC(tmp) != NULL) {
                    parent = tmp;
                    tmp = LC(tmp);
                }
                char *val = strdup(TreeValue(tmp)); // -- copy successor's string --
                AssTreeVal(n, val);
                if (tmp == RC(n)) {
                    Ass_RC(n, RC(tmp));
                }
                else {
                    Ass_LC(parent, RC(tmp));
                }
                free(tmp->string);              // -- free successor's old string --
                free(tmp);
            }
        }
    }
}

// -- Removes a LL node from the list by pointer.
//    Updates the head if necessary --
void DeleteByPtr(Node **h, Node *Ptr, Node *prev) {
    Node *p, *q;
    if (Ptr == *h) {
        *h = Next(*h);                         
    }
    else {
        Ass_adr(prev, Next(Ptr));
    }
    free(Ptr);
}

// -- Deletes a word from a LL-of-BST paragraph structure.
//    Finds the correct letter Node, then calls delete_from_BST.
//    If the BST becomes empty after deletion, removes the LL node too --
void DeleteInPara(Node **para, char *word) {
    Node *q = *para;
    Node *prev = NULL;
    while (q != NULL && toupper(word[0]) >= Value(q)) {
        if (toupper(word[0]) == Value(q)) {
            delete_from_BST(&(q->tree), word, q);
            // -- if BST is now empty, remove the LL node for this letter --
            if (q->tree == NULL) {
                DeleteByPtr(para, q, prev);
            }
            break;
        }
        prev = q;
        q = Next(q);
    }
}

//************************************************************************************
//                         -- SET OPERATIONS : UNION --
//************************************************************************************

// -- Recursive pre-order traversal of a BST.
//    For each node, inserts its word into *result if not already present.
//    Uses strdup to give *result ownership of the string --
void UnionTrav_PreOrd(TRNode *Root, Node **result) {
    if (Root != NULL) {
        if (!SearchInPara(*result, TreeValue(Root))) {
            char *copy = strdup(TreeValue(Root)); // -- independent copy --
            InsertInLL(result, copy);
        }
        UnionTrav_PreOrd(LC(Root), result);
        UnionTrav_PreOrd(RC(Root), result);
    }
}

// -- Computes the union of paragraph struct1 into *res.
//    Walks all LL nodes of struct1 and calls UnionTrav_PreOrd on each BST --
void Union2Para(Node *struct1, Node **res) {
    Node *p = struct1;
    while (p != NULL) {
        UnionTrav_PreOrd(p->tree, res);
        p = Next(p);
    }
}

// -- Computes the union of N selected paragraphs into *res.
//    arrsize paragraphs are identified by their (file, para) indices in arr --
void UnionNPara(Apara *arr, int arrsize, Afile *TAB, Node **res) {
    for (int i = 0; i < arrsize; i++) {
        // -- convert 1-based user indices to 0-based array indices --
        Union2Para(TAB[arr[i].numfile - 1].Filetab[arr[i].numpar - 1], res);
    }
}

//************************************************************************************
//                       -- SET OPERATIONS : INTERSECTION --
//************************************************************************************

// -- Collects all words from a BST into a char* array (with strdup copies).
//    Safe to use before deletion because the copies are independent of the BST --
void collectWords(TRNode *root, char ***arr, int *size) {
    if (root == NULL) return;
    *arr = realloc(*arr, (*size + 1) * sizeof(char *));
    (*arr)[(*size)++] = strdup(TreeValue(root)); // -- own copy, safe after deletion --
    collectWords(LC(root), arr, size);
    collectWords(RC(root), arr, size);
}

// -- Computes the intersection of *res with struct1.
//    Step 1 : copies all words from *res into a temporary array (strdup copies).
//    Step 2 : delete from *res any word not found in struct1.
//    The array prevents traversal-while-modifying bugs --
void Inter2Para(Node *struct1, Node **res) {
    char **words = NULL;
    int size = 0;
    Node *p = *res;
    // -- Step 1 : collect all current words from *res --
    while (p != NULL) {
        collectWords(p->tree, &words, &size);
        p = Next(p);
    }
    // -- Step 2 : remove words absent from struct1 --
    for (int i = 0; i < size; i++) {
        if (!SearchInPara(struct1, words[i])) {
            DeleteInPara(res, words[i]);
        }
        free(words[i]);                         // -- free the strdup copy --
    }
    free(words);
}

// -- Computes the intersection of N selected paragraphs into *res.
//    copy struct1 into *res(empty) using union function, then intersects with each remaining one --
void InterNPara(Apara *arr, int arrsize, Afile *TAB, Node **res) {
    // -- copying struct1 into *res --
    Union2Para(TAB[arr[0].numfile - 1].Filetab[arr[0].numpar - 1], res);
    // -- intersect with each remaining paragraph starting from index 1 --
    for (int i = 1; i < arrsize; i++) {
        Inter2Para(TAB[arr[i].numfile - 1].Filetab[arr[i].numpar - 1], res);
    }
}

//************************************************************************************
//                       -- SET OPERATIONS : DIFFERENCE --
//************************************************************************************

// -- Recursive pre-order traversal of struct1's BST.
//    For each word from struct1  found in *result, deletes it from *result.
//    Computes *result = *result \ struct1 --
void DiffTrav_PreOrd(TRNode *Root, Node **result) {
    if (Root != NULL) {
        if (SearchInPara(*result, TreeValue(Root)) == true) {
            DeleteInPara(result, TreeValue(Root));
        }
        DiffTrav_PreOrd(Root->left, result);
        DiffTrav_PreOrd(Root->right, result);
    }
}

// -- Computes *res = *res \ struct1 :
//    removes from *res every word that also appears in struct1 --
void Diff2Para(Node *struct1, Node **res) {
    Node *p = struct1;
    while (p != NULL) {
        DiffTrav_PreOrd(p->tree, res);
        p = Next(p);
    }
}

// -- Computes the difference of N selected paragraphs into *res.
//   copy struct1 into *res(empty) using union function, then subtracts each remaining one --
void DiffNPara(Apara *arr, int arrsize, Afile *TAB, Node **res) {
    // -- copying struct1 into *res --
    Union2Para(TAB[arr[0].numfile - 1].Filetab[arr[0].numpar - 1], res);
    // -- subtract each remaining paragraph --
    for (int i = 1; i < arrsize; i++) {
        Diff2Para(TAB[arr[i].numfile - 1].Filetab[arr[i].numpar - 1], res);
    }
}

//************************************************************************************
//                   -- SET OPERATIONS : SYMMETRIC DIFFERENCE --
//************************************************************************************

// -- Computes the symmetric difference of *res and struct1 :
//    SyDiff = (*res U struct1) \ (*res ∩ struct1)
//    Step 1 : compute the intersection (p1 = *res ∩ struct1)
//    Step 2 : add struct1 into *res (now *res = *res U struct1)
//    Step 3 : subtract the intersection from the union --
void SyDiff2Para(Node *struct1, Node **res) {
    Node *p1 = NULL;
    // -- Step 1 : p1 = *res ∩ struct1 --
    Union2Para(*res, &p1);
    Inter2Para(struct1, &p1);
    // -- Step 2 : *res = *res U struct1 --
    Union2Para(struct1, res);
    // -- Step 3 : *res = (*res U struct1) \ (*res ∩ struct1) --
    Diff2Para(p1, res);
}

// -- Computes the symmetric difference of N selected paragraphs into *res.
//    Applies SyDiff2Para cumulatively across all paragraphs --
void SyDiffNPara(Apara *arr, int arrsize, Afile *TAB, Node **res) {
    for (int i = 0; i < arrsize; i++) {
        // -- convert 1-based user indices to 0-based array indices --
        SyDiff2Para(TAB[arr[i].numfile - 1].Filetab[arr[i].numpar - 1], res);
    }
}

//************************************************************************************
//                       -- COUNTING & STATISTICS --
//************************************************************************************

// -- Counts the number of nodes in a BST  --
int NumTRNodes(TRNode *Root) {
    if (Root != NULL) {
        return NumTRNodes(LC(Root)) + NumTRNodes(RC(Root)) + 1;
    }
    else {
        return 0;
    }
}

// -- Counts the total number of words stored in a full LL-of-BST structure --
int NumStructNodes(Node *struc) {
    Node *p = struc;
    int sum = 0;
    while (p != NULL) {
        sum += NumTRNodes(p->tree);
        p = Next(p);
    }
    return sum;
}

// -- Returns the total number of paragraphs across all loaded files --
int Allpara(Afile TAB[], int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += TAB[i].size;
    }
    return sum;
}

// -- Computes the Similarity persentage between N paragraphs :
//    Similarity = |Intersection| / |Union|  (stopwords excluded from both)
//    Returns a float between 0.0 (nothing in common) and 1.0 (identical) --
float Similarity(Apara *arr, int arrsize, Afile *TAB) {
    Node *uni   = NULL;
    Node *inter = NULL;
    Node *StopWords = NULL;

    loadStopwords(&StopWords);

    // -- compute union then remove stopwords --
    UnionNPara(arr, arrsize, TAB, &uni);
    Diff2Para(StopWords, &uni);

    // -- compute intersection then remove stopwords --
    InterNPara(arr, arrsize, TAB, &inter);
    Diff2Para(StopWords, &inter);

    float u = NumStructNodes(uni);
    float n = NumStructNodes(inter);

    freeStruct(&uni);
    freeStruct(&inter);
    freeStruct(&StopWords);

    if (u == 0) return 0.0f;                   // -- avoid division by zero --
    return n / u;
}

//************************************************************************************
//                             -- STOPWORDS --
//************************************************************************************

// -- Static list of ~200 common English stopwords.
//    Grouped alphabetically. NULL sentinel marks the end --
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
    NULL                                        // -- end-of-list sentinel --
};

// -- Loads all stopwords from the static list into the LL-of-BST structure *sw.
//    Each word is strdup'd so the structure owns the memory independently --
void loadStopwords(Node **sw) {
    int i = 0;
    while (STOPWORDS[i] != NULL) {
        char *copy = strdup(STOPWORDS[i]);
        InsertInLL(sw, copy);
        i++;
    }
}

//************************************************************************************
//                          -- MEMORY MANAGEMENT --
//************************************************************************************

// -- Recursively frees all nodes of a BST, including the string in each node --
void freeBST(TRNode *root) {
    if (root == NULL) return;
    freeBST(LC(root));
    freeBST(RC(root));
    free(root->string);                         // -- free the heap-allocated word --
    free(root);
}

// -- Frees an entire LL-of-BST structure :
//    walks each LL node, frees its BST, then frees the LL node itself.
//    Sets *head to NULL after cleanup --
void freeStruct(Node **head) {
    Node *p = *head;
    while (p != NULL) {
        Node *next = Next(p);
        freeBST(p->tree);
        free(p);
        p = next;
    }
    *head = NULL;                               // -- reset pointer to avoid dangling --
}

//************************************************************************************
//                          -- PRINT & DISPLAY --
//************************************************************************************

// -- Prints the words of a BST in pre-order (root, left, right) --
void PrintTree_preord(TRNode *Root) {
    if (Root != NULL) {
        printf("%s | ", TreeValue(Root));
        PrintTree_preord(LC(Root));
        PrintTree_preord(RC(Root));
    }
}

// -- Prints all words in a full LL-of-BST structure.
//    Each LL node (letter Node) is printed on its own line --
void PrintStruct(Node *struc) {
    Node *p = struc;
    while (p != NULL) {
        printf("|-  ");
        PrintTree_preord(p->tree);
        printf("  -|");
        printf("\n");
        p = Next(p);
    }
}

//************************************************************************************
//                            -- FILE OUTPUT --
//************************************************************************************

// -- Writes the words of a BST in pre-order to an open file --
void FillFileTrav_preord(TRNode *Root, FILE *file) {
    if (Root != NULL) {
        fprintf(file, "%s | ", TreeValue(Root));
        FillFileTrav_preord(LC(Root), file);
        FillFileTrav_preord(RC(Root), file);
    }
}

// -- Saves a full LL-of-BST structure to a file at the given path.
//    Creates the file if it does not exist, overwrites if it does --
void fillfile(Node *struc, char *path) {
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

//************************************************************************************
//                        -- INTERFACE & ANIMATIONS --
//************************************************************************************

// -- Sets the console text and background colors using Windows API.
//    text and bg use standard Windows console color codes (0-15) --
void Color(int text, int bg) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), bg * 16 + text);
}

// -- Displays a step-by-step loading bar animation on the console.
//    8 steps with percentage and label, using green fill and gray empty bars --
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
        Color(10, 0);                           // -- green for filled portion --
        for (j = 0; j < filled; j++)  printf("|");
        Color(8, 0);                            // -- gray for empty portion --
        for (j = filled; j < total_bar; j++) printf("-");
        Color(7, 0);
        printf("] %3d%%\n\n", pcts[i]);
        printf("\t\t\t\t\t  %s\n", labels[i]);
        Sleep(500);
    }
    Sleep(400);
}

// -- Displays the landing card : ESI logo, module info, team members, tutor --
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
    // -- project information card --
    printf("\t  ---------------------------------------------------------------\n");
    printf("\t  |                                                             |\n");
    printf("\t  |  Module    :  Algorithmics & Dynamic Data Structures        |\n");
    printf("\t  |  Lab Work  :  TP1 - Set Theory on Linguistic Structures     |\n");
    printf("\t  |                                                             |\n");
    printf("\t  |  Member 01 :  GHILASSENE Abdelhalim                         |\n");
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

// -- Runs the full welcome sequence : loading bar then landing card --
void welcome() {
    loading_bar();
    landing_card();
    system("cls");
}

// -- Displays a simple inline dots animation : "Loading....." --
void loadingAnimation() {
    printf("Loading");
    for (int i = 0; i < 5; i++) {
        printf(".");
        fflush(stdout);                         // -- force immediate print --
        Sleep(300);
    }
    printf("\n");
}