#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Size 9973
#define wSize 20


struct node // saves word
{
    char word[wSize];
};

struct incorrectsNode   // for incorrect words table
{
    char word[wSize];
    char recommended[wSize];
};

// converts words to lowercase
void toLowerCase(char string[]);
// creates a new node for new word if necessary
struct node* createNode(char word[]);
// makes the hash table null at the start
void nullify_table(struct node* hashTable[]);
// makes incorrect words table null
void nullify_IncorrectsTable(struct incorrectsNode* incorrectsTable[]);
// string to key using horner method
int horner(char word[]);
// Calculates the Levenshtein Distance of two word. Stop when necessary
int LED(char word1[], char word2[]);
// Checks Dictionary Table and find similar words
void findSimilarWords(struct node* hashTable[], struct incorrectsNode* incorrectsTable[], char word[]);
// creates Dictionary using words in the document
int createHashtable(struct node* hashTable[],char word[]);
// searches for the word in the dictionary table
int searchTable(struct node* hashTable[], char word[]);
// reads document and creates a hashtable
void readFile(struct node* hashTable[]);
// minimum finder
int findMin(int a, int b, int c);
// to create new nodes for incorrect words table
struct incorrectsNode* createIncorrectsNode(char word[], char recommended[]);
// to search if an element exists in incorrect words table
int searchIncorrectsTable(struct incorrectsNode* incorrectsTable[], char word[]);
// adds the user selected word to the incorrect words table
int addToIncorrectsTable(struct incorrectsNode* incorrectsTable[], char word[], char recommended[]);
// Separates the words from the user entered sentence
void separateWords(struct node* hashTable[], struct incorrectsNode* incorrectsTable[],char sentence[]);



void nullify_table(struct node* hashTable[]) //makes the hash table null at the start
{
    int i;
    for(i = 0; i<Size; i++)
    {
        hashTable[i] = NULL;
    }
    return;
}

void nullify_IncorrectsTable(struct incorrectsNode* incorrectsTable[]) // makes another hashtable null
{
    int i;
    for(i=0;i<Size;i++)
    {
        incorrectsTable[i] = NULL;
    }
    return;
}

void toLowerCase(char string[]) // converts words to lowercase
{
    int i;
    for (i = 0; string[i]!='\0'; i++) {
        if(string[i] >= 'A' && string[i] <= 'Z') {
            string[i] = string[i]+32;
        }
    }
    return;
}

int LED(char word1[], char word2[]) // Calculates the Levenshtein Distance of two word. Stops when it is apparent that the distance threshold is
{                                                                                                               // overran
    short word1Lenth = strlen(word1);       // length of first word
    short word2Lenth = strlen(word2);       // length of second word
    int subcost;        // for calculating substitution cost

    int i;
    int** matrix = (int**)calloc((word1Lenth+1),sizeof(int*));

    if(matrix==NULL)
    {
        printf("Error!! allocation failed\n");
        return 0;
    }

    for(i=0;i<=word1Lenth;i++)
    {
        matrix[i] = (int*)calloc((word2Lenth+1),sizeof(int));   // allocate memory for the matrix
        if(matrix[i]==NULL)
        {
            printf("Error!! Allocation Failed\n");
            return 0;
        }
    }

    for(i=0;i<=word1Lenth;i++)
    {
        matrix[i][0] = i;
    }

    for(i=0;i<=word2Lenth;i++)
    {
        matrix[0][i] = i;
    }

    int counter;        // used for counting number of elements greater than threshold value which is 2,
    int j;
    for(i=1;i<=word1Lenth;i++)
    {
        counter = 0;
        for(j=1;j<=word2Lenth;j++)
        {
            if( word1[i-1] == word2[j-1] )
            {
                subcost = 0;
            }
            else
            {
                subcost = 1;
            }
                                    // Deletion         // Insertion            //Substitution
            matrix[i][j] = findMin( matrix[i-1][j]+1, matrix[i][j-1]+1, matrix[i-1][j-1]+subcost );

            if(matrix[i][j] >2)
            {
                counter++;  // increment counter if any element of i'th row is greater than threshold 2
            }
        }

        if(counter >= j-1)  // counter is equal to j-1 it means threshold value is already surpassed so no need to check other elments
        {
            return 10;      // if all elements of i'th row is greater than 2. stop. return any number greater than 2
        }
    }
    /*for(int i=0;i<=word1Lenth;i++)
    {
        printf("\n");
        for(int j=0;j<=word2Lenth;j++)
        {
            printf("%d\t", matrix[i][j]);
        }
    }*/
    int value = matrix[word1Lenth][word2Lenth]; // VALUE is necessary to free the matrix
    free(matrix);
    return value;

}

int findMin(int a, int b, int c)        // Find minimum of three numbers
{
    if(a < b) {
        if(a < c) return a;
        else return c;
    } else {
        if(b < c) return b;
        else return c;
    }
}


void findSimilarWords(struct node* hashTable[], struct incorrectsNode* incorrectsTable[], char word[])  //
{
    struct node* pointer[wSize];    // to store words which Edit distance is less than or equal to 2
    int k;
    for(k=0;k<wSize;k++)
    {
        pointer[k] = NULL;
    }

    int i;
    int j=0;
    char correctWord[wSize];

    for(i=0;i<Size;i++)
    {
        if(hashTable[i] != NULL)
        {
            if(LED(word, hashTable[i]->word) < 3)
               {
                    pointer[j] = hashTable[i];  // save similar words
                    j++;
               }
        }
    }

    if(j!=0)                //if Similar words found
    {
        printf("\"%s\" is not in the Dictionary. Did You Mean: ", word);
        while(j>0)
        {
            printf("\"%s\"\t", pointer[j-1]->word); // print similar words
            j--;
        }
        printf("\n");

        scanf("%s",correctWord);    // get selected word
        j=0;

        while(pointer[j] != NULL )
        {
            if(!strcmp(pointer[j]->word,correctWord))
            {
                addToIncorrectsTable(incorrectsTable, word, correctWord);   // add the word and selected similar word to incorrect words table
                strcpy(word, correctWord);  // change the word to the new selected word
                return;
            }
            j++;
        }

        printf("Wrong Choice\n");
        return;
    }

    //printf("No Similar Words Found\n");
    return;
}

int addToIncorrectsTable(struct incorrectsNode* incorrectsTable[], char word[], char recommended[])
{
    int key = horner(word); // get the key using horner method
    int doubleIndex;        // index for double hash
    int index1 = key%Size;  // first index
    int index2 = 1+(key%(Size-1)); // used for double hashing

    if( incorrectsTable[index1] != NULL ) // if element is not null, do double hashing, if element is null add the element, commands are in else branch
    {
        if(strcmp(word, incorrectsTable[index1]->word))// if element is not null, see if the elements are same strcmp returns 0 if same
        { // elements are not same get new index
            int i=1;
            doubleIndex = (index1+(i*index2))%Size; // new index using double hash
            while((incorrectsTable[doubleIndex] != NULL) && (i<Size))
            {   // if new index element is not null
                if(!strcmp(word, incorrectsTable[doubleIndex]->word)) // break if the elements are same
                {
                    break;
                }
                i++;
                doubleIndex = (index1+(i*index2))%Size;
            }
            if(incorrectsTable[doubleIndex] == NULL)  // if new index is null add the element
            {
                incorrectsTable[doubleIndex] = createIncorrectsNode(word, recommended);
                printf("index1 is %d\n", index1);
                printf("Positioned At %d\n", doubleIndex);
                return doubleIndex;
            }
            else if(i>= Size)   // coudn't produce new index
            {
                printf("Program Could not find a Suitable place for the Word '%s'\n", word);
                return -1;
            }
            else    // if the elements are the same do nothing
            {
                return doubleIndex;
            }
        }
        else // if elements are the same do nothing
        {
            return index1;
        }
    }
    else // if first index is null add the element
    {
        incorrectsTable[index1] = createIncorrectsNode(word,recommended);
        return index1;
    }
    printf("Program Could Not Add The Element\n");
    return -1;

}

struct incorrectsNode* createIncorrectsNode(char word[], char recommended[])
{
    struct incorrectsNode* pointer = NULL;
    pointer = (struct incorrectsNode*)malloc(sizeof(struct incorrectsNode));
    strcpy(pointer->word, word);                // copy the incorrect word into the hashtable
    strcpy(pointer->recommended, recommended);  // copy the recommended word into the incorrects word hashtable
    return pointer;
}

int createHashtable(struct node* hashTable[],char word[]) // creates hash table using word and document name given as argument
{
    int key = horner(word); // get the key using horner method
    int doubleIndex;        // index for double hash
    int index1 = key%Size;  // first index
    int index2 = 1+(key%(Size-1)); // used for double hashing

    if( hashTable[index1] != NULL ) // if element is not null, do double hashing, if element is null add the element, commands are in else branch
    {
        if(strcmp(word, hashTable[index1]->word))// if element is not null, see if the elements are same strcmp returns 0 if same
        { // elements are not same get new index
            int i=1;
            doubleIndex = (index1+(i*index2))%Size; // new index using double hash
            while((hashTable[doubleIndex] != NULL) && (i<Size))
            {   // if new index element is not null
                if(!strcmp(word, hashTable[doubleIndex]->word)) // break if the elements are same
                {
                    break;
                }
                i++;
                doubleIndex = (index1+(i*index2))%Size;
            }
            if(hashTable[doubleIndex] == NULL)  // if new index is null add the element
            {
                hashTable[doubleIndex] = createNode(word);
                return doubleIndex;
            }
            else if(i>= Size)   // coudn't produce new index
            {
                printf("Program Could not find a Suitable place for the Word '%s'\n", word);
                return -1;
            }
            else    // if the elements are the same do nothing
            {
                return doubleIndex;
            }
        }
        else // if elements are the same do nothing
        {
            return index1;
        }
    }
    else // if first index is null add the element
    {
        hashTable[index1] = createNode(word);
        return index1;
    }
    printf("Program Could Not Add The Element\n");
    return -1;
}

int horner(char word[]) // string to key using horner method
{
    int length = strlen(word);
    toLowerCase(word);
    unsigned long long int hash = 0;
    int i;
    for (i = 0; i < length; i++)
    {
        //printf("Hash is %")
        hash = (31 * hash) + (word[i]-'a'+1);
    }
    hash = hash%Size;
    return hash;
}

struct node* createNode(char word[]) // creates a new node for new word if necessary
{
    struct node* pointer = NULL;
    pointer = (struct node*)malloc(sizeof(struct node));
    strcpy(pointer->word, word);

    return pointer;
};

void readFile(struct node* hashTable[]) // Create the hashtable from dictionary.txt
{
    FILE *file;
    char line[Size];
    char word[wSize];

    file = fopen("smallDictionary.txt", "r");
    if ( file == NULL)
    {
        printf("Could no read the file\n");
        return;
    }

    while(!feof(file))
    {
        fgets(line,Size,file); // get one line first from the text
        int i=0;
        int len = strlen(line);
        while(i<len)
        {
            int k;
            for(k = 0; k< wSize; k++)  // making sure the array elements are null
            {
                word[k] = 0;
            }

            int j =0; // used for reading line character by character
            while( line[i] != ' ' && line[i] != ',' && line[i] != NULL && line[i] != '\n' && line[i] != '.' && line[i]!='?')
            {
                word[j] = line[i];  // get character by character
                i++;
                j++;
            }

            if(word[0] != 0)
            {
                createHashtable(hashTable,word);
                i++;
            }
            else
            {
                i++;
            }
        }
    }
    fclose(file);
    //printf("File Added Successfully\n");
    return;
}

void separateWords(struct node* hashTable[], struct incorrectsNode* incorrectsTable[],char sentence[])
{
    int length = strlen(sentence);
    int i = 0;
    char word[wSize];
    char newSentence[100];
    char recommended[wSize];

    while(i<length)
    {
        int k;
        for(k=0; k<wSize; k++)
        {
            word[k] = 0;
        }

        int j =0; // used for reading line character by character
        while( sentence[i] != ' ' && sentence[i] != ',' && sentence[i] != NULL && sentence[i] != '\n' && sentence[i] != '.' && sentence[i]!='?')
        {
            word[j] = sentence[i];  // get character by character
            i++;
            j++;
        }

        if(word[0] != 0)    // if word exists
        {
            int index = searchIncorrectsTable(incorrectsTable, word);
            if((!searchTable(hashTable, word)) && !index)       // if word is in dictionary and incorrects table or not
            {
                findSimilarWords(hashTable, incorrectsTable, word);      // if not change word to new word
            }

            if(index)   // if word is in incorrects List
            {
                printf("Hatali kelime Tablosundan: \"%s\" is incorrect. Did you mean : %s\n",word, incorrectsTable[index]->recommended);
                scanf("%s",recommended);
                //printf("Here\n");
                if(!strcmp(recommended, incorrectsTable[index]->recommended))   // if user selected a valid choice
                {
                    strcpy(word, recommended);  // change word to new word
                }
                else
                {
                    printf("No Acceptable Word Selected\n");
                }
            }

            strcat(newSentence, word);
            strcat(newSentence, " ");
            i++;
        }
        else
        {
            i++;
        }
    }

    printf("%s\n", newSentence);
    return;

}

int searchTable(struct node* hashTable[], char word[]) // searches if the elements exist
{
    int key = horner(word); // find the horner key and make the element lowerCase
    int doubleIndex;
    int index1 = key%Size;
    int index2 = 1+(key%(Size-1));
    if( hashTable[index1] != NULL ) // if first index is null it means the element does not exist
    {
        if(!strcmp(word, hashTable[index1]->word)) // if the first index is not null check if the words are same
        {
            //printf("Word is %s\n", hashTable[index1]->word);
            return 1;
        }
        else    // if they are not the same, find new index
        {
            int i=1;
            doubleIndex = (index1+(i*index2))%Size;
            while(hashTable[doubleIndex] != NULL && i<Size)
            {
                if(!strcmp(word, hashTable[doubleIndex]->word))
                {
                    //printf("Word is %s\n", hashTable[doubleIndex]->word);
                    return 1;
                }
                i++;
                doubleIndex = (index1+(i*index2))%Size;
            }
            //printf("Word Does Not Exist\n");
            return 0;

        }
    }
    else
    {
        //printf("Word Does Not Exist\n");
        return 0;
    }
}

int searchIncorrectsTable(struct incorrectsNode* incorrectsTable[], char word[]) // searches if the elements exist
{
    int key = horner(word); // find the horner key and make the element lowerCase
    int doubleIndex;
    int index1 = key%Size;
    int index2 = 1+(key%(Size-1));
    if( incorrectsTable[index1] != NULL ) // if first index is null it means the element does not exist
    {
        if(!strcmp(word, incorrectsTable[index1]->word)) // if the first index is not null check if the words are same
        {
            return index1;  // return the index if words are same
        }
        else    // if they are not the same, find new index
        {
            int i=1;
            doubleIndex = (index1+(i*index2))%Size;
            while(incorrectsTable[doubleIndex] != NULL && i<Size)
            {
                if(!strcmp(word, incorrectsTable[doubleIndex]->word))
                {
                    printf("Word is %s\n", incorrectsTable[doubleIndex]->word);
                    return doubleIndex;
                }
                i++;
                doubleIndex = (index1+(i*index2))%Size;
            }
            //printf("Word Does Not Exist\n");
            return 0;

        }
    }
    else
    {
        //printf("Word Does Not Exist\n");
        return 0;
    }
}

int main()
{
    int select;     // used for getting selection from user
    char sentence[100]; // for getting sentence from user
    int value=1;
    struct node* hashTable[Size]; // dictionary table
    nullify_table(hashTable);
    readFile(hashTable);
    struct incorrectsNode* incorrectsTable[Size]; // incorrect word table
    char *p;
    nullify_IncorrectsTable(incorrectsTable);


    while(value)
    {
        printf("Enter 1 : To ADD Sentence, Enter Any other number: To EXIT\n");
        scanf("%d", &select);
        fflush(stdin);

        switch(select)
        {
            case 1:
            {
                printf("Enter Your Sentence:\n");
                fgets(sentence, 98, stdin);
                if((p = strchr(sentence, '\n')) != NULL)
                {
                    *p = '\0';
                }
                separateWords(hashTable,incorrectsTable,sentence);
                break;
            }

            default:
                printf("Program Exited\n");
                value = 0;
                break;
        }
    }

    return 0;
}
