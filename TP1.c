#include "libtp1.h"

//************************************************************************************
//                              -- MAIN PROGRAM --
//************************************************************************************

int main() {

    // -- menu strings --
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

    // -- control variables --
    int i, j, choice, OPchoice, Schoice, SWchoice = 0;
    int filesnum, parsnum;
    float SimRatio = 0;

    // -- data structures --
    Afile *TABf = NULL;                         // -- array of loaded files --
    Apara *TABp = NULL;                         // -- array of selected paragraphs --
    FILE *file;
    bool bool1 = false;                         // -- true once files are loaded --
    bool bool2 = false;                         // -- true once paragraphs are selected --
    bool SWbool = false;                        // -- true once stopwords are loaded --

    // -- result structures for each set operation --
    Node *Uni    = NULL;
    Node *Inter  = NULL;
    Node *Diff   = NULL;
    Node *SyDiff = NULL;
    Node *StopW  = NULL;                        // -- loaded once, reused across operations --

    char path[256];                             // -- output file path buffer --

    // -- launch welcome animation and landing card --
    welcome();

    // -- main menu loop --
    do {
        // -- inner loop : validate menu choice --
        do {
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

        switch (choice) {

        // -- case 1 : load files from user-provided paths --
        case 1:
            do {
                system("cls");
                printf("How many files you want to apply operations on ?\n");
                printf("--> ");
                scanf("%d", &filesnum);
            } while (filesnum <= 0);

            free(TABf);                         // -- free previous file table --
            printf("Enter the path of each file : \n");
            printf("[example : C:\\Users\\user\\Desktop\\filename.txt]\n\n");

            for (i = 0; i < filesnum; i++) {
                TABf = realloc(TABf, (i + 1) * sizeof(Afile));
                // -- retry until a valid file path is given --
                do {
                    printf("--> ");
                    scanf("%s", TABf[i].filename);
                    file = fopen(TABf[i].filename, "r");
                    if (file == NULL) {
                        printf(" Can't open this file !! try again \n");
                    }
                } while (file == NULL);

                TABf[i].Filetab = NULL;
                TABf[i].size = 0;
                FileToStruct(file, &(TABf[i].Filetab), &(TABf[i].size));
                fclose(file);
            }
            bool1 = true;
            bool2 = false;                      // -- reset paragraph selection --
            printf("\n  -> Done !\n");
            Sleep(2000);
            break;

        // -- case 2 : select paragraphs from loaded files --
        case 2:
            if (bool1) {
                free(TABp);
                // -- validate number of paragraphs --
                do {
                    system("cls");
                    printf("how many paragraphes will you use :");
                    scanf("%d", &parsnum);
                } while (parsnum > Allpara(TABf, filesnum) || parsnum < 1);

                TABp = malloc(parsnum * sizeof(Apara));

                for (i = 0; i < parsnum; i++) {
                    system("cls");
                    // -- show available files --
                    printf("\n\nthe files that you had entred are :\n\n");
                    for (j = 0; j < filesnum; j++) {
                        printf("%d --> %s (contains %d paragraphs)\n", (j + 1), TABf[j].filename, TABf[j].size);
                    }
                    // -- show already selected paragraphs --
                    if (i != 0) {
                        printf("the paragraphs chosed : ");
                        for (j = 0; j < i; j++) {
                            printf("(%d : %d) | ", TABp[j].numfile, TABp[j].numpar);
                        }
                        printf("\n");
                    }
                    // -- get file number (1-based) --
                    do {
                        printf("enter the file : ");
                        scanf("%d", &TABp[i].numfile);
                    } while (TABp[i].numfile <= 0 || TABp[i].numfile > filesnum);

                    // -- get paragraph number within chosen file (1-based) --
                    do {
                        printf("enter the paragraph : ");
                        scanf("%d", &TABp[i].numpar);
                    } while (TABp[i].numpar <= 0 || TABp[i].numpar > TABf[(TABp[i].numfile) - 1].size);
                }

                printf("-------------------------------------------------");
                printf("\nthe paragraphs chosed : ");
                for (j = 0; j < i; j++) {
                    printf("(%d : %d) | ", TABp[j].numfile, TABp[j].numpar);
                }
                bool2 = true;
                printf("\n  -> Done !\n");
            }
            else {
                printf("   you can't enter para befor files pleas do choice 1- Add files first");
            };
            printf("    \nPress Any key to continue");
            getch();
            break;

        // -- case 3 : choose and perform a set operation --
        case 3:
            if (bool1 && bool2) {
                // -- operation sub-menu --
                do {
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

                switch (OPchoice) {

                // -- Union of selected paragraphs --
                case 1:
                    loadingAnimation();
                    freeStruct(&Uni);           // -- reset previous union result --
                    // -- ask user about stopword filtering --
                    do {
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

                    UnionNPara(TABp, parsnum, TABf, &Uni);

                    // -- SWchoice==2 means "No stopwords" (exclude them) --
                    if (SWchoice == 2) {
                        if (!SWbool) {
                            loadStopwords(&StopW); // -- load only once --
                            SWbool = true;
                        }
                        Diff2Para(StopW, &Uni);
                    }
                    printf(" The result of the Union is : \n");
                    PrintStruct(Uni);
                    printf("\n          -> Done !\n");

                    // -- save to file option --
                    do {
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
                        scanf("%s", path);
                        fillfile(Uni, path);
                    }
                    printf("    \nPress Any key to continue");
                    getch();
                    break;

                // -- Intersection of selected paragraphs --
                case 2:
                    loadingAnimation();
                    freeStruct(&Inter);         // -- reset previous intersection result --
                    // -- ask user about stopword filtering --
                    do {
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
                        if (!SWbool) {
                            loadStopwords(&StopW);
                            SWbool = true;
                        }
                        Diff2Para(StopW, &Inter);
                    }
                    printf("\n  --> The result of the Intersection is : <--\n");
                    if(Inter == NULL){
                        printf("\n   Paragraphs are disjoints (intersection is Empty)\n");
                    }
                    else{
                        PrintStruct(Inter);
                    }
                    printf("\n          -> Done !\n");

                    // -- save to file option --
                    do {
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
                        scanf("%s", path);
                        fillfile(Inter, path);
                    }
                    printf("    \nPress Any key to continue");
                    getch();
                    break;

                // -- Difference of selected paragraphs --
                case 3:
                    loadingAnimation();
                    freeStruct(&Diff);          // -- reset previous difference result --
                    DiffNPara(TABp, parsnum, TABf, &Diff);
                    printf("\n  --> The result of the Difference is : <--\n");
                    PrintStruct(Diff);
                    printf("\n          -> Done !\n");

                    // -- save to file option --
                    do {
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
                        scanf("%s", path);
                        fillfile(Diff, path);
                    }
                    printf("    \nPress Any key to continue");
                    getch();
                    break;

                // -- Symmetric Difference of selected paragraphs --
                case 4:
                    loadingAnimation();
                    freeStruct(&SyDiff);        // -- reset previous symmetric difference result --
                    SyDiffNPara(TABp, parsnum, TABf, &SyDiff);
                    printf("\n  --> The result of the Symetric Difference is : <--\n");
                    PrintStruct(SyDiff);
                    printf("\n          -> Done !\n");

                    // -- save to file option --
                    do {
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
                        scanf("%s", path);
                        fillfile(SyDiff, path);
                    }
                    printf("    \nPress Any key to continue");
                    getch();
                    break;

                // -- Jaccard Similarity between selected paragraphs --
                case 5:
                    SimRatio = Similarity(TABp, parsnum, TABf);
                    printf("\n+--------------------------------------+\n");
                    printf("|  the paragraphs are %.2f %% similar  |\n", SimRatio * 100.0f);
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

        // -- case 4 : exit — free all allocated memory before closing --
        case 4:
            freeStruct(&Uni);
            freeStruct(&Inter);
            freeStruct(&Diff);
            freeStruct(&SyDiff);
            freeStruct(&StopW);
            free(TABf);
            free(TABp);
            break;

        default:
            break;
        }

    } while (choice != 4);

    // -- closing animation --
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