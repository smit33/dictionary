/*
Smit Shah 19BCE259
Harshil Tagadiya 19BCE270
Tilak Tejani 19BCE273

Definition:
Implement a dictionary with word and it's respective meaning using Trie data structure. 
Also when a user enters a root word, it also gives the related words.

Data Structure: Trie
Database: Data.txt

 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// Trie Node
struct Node {
    struct Node* child[26];
    int endOfWord;
    char meaning[100];
};

// New node function
struct Node* createNode(){
    struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
    return temp;
};

void changeToLowerCase(char *word){
    for(int i = 0; i < strlen(word); i++){
        if(word[i] >= 'A' && word[i] <= 'Z'){
            word[i] = word[i] + 32;     // converting to lowercase
        }
    }
}


// check function for proper input of word
int check(char word[]){
    for(int i = 0; i < strlen(word); i++){
        if( ! (word[i] >= 'a' && word[i] <= 'z') ){
            return 0;
        }
    }
    return 1;
}

//insert function of trie node
void insert(struct Node *root, char word[], char meaning[], int addCustom){
    changeToLowerCase(word);
    if(check(word) == 0){
        printf("Please enter proper word!\n"); return;      // not a proper word
    }
    for(int i = 0; i < strlen(word); i++) {
        if(root->child[word[i] - 'a'] != NULL) {
            root = root->child[word[i] - 'a'];
        }
        else {
            struct Node* childNode = createNode();      // creating new node
            for(int i = 0; i < 26; i++) {
                childNode->child[i] = NULL;         // initializing childNode with NULL
            }
            childNode->endOfWord = 0;

            root->child[word[i] - 'a'] = childNode;

            root = childNode;
        }
    }
    if(root->endOfWord == 1){ // check if word is already present in trie.
        return ;
    }

    root->endOfWord = 1;
    strcpy(root->meaning, meaning);

    // Add custom word by user
    if(addCustom == 1){
        FILE *fp = fopen("Data.txt", "a");

        fputs(word, fp);
        fputs(" ",fp);
        fputs(meaning,fp);
        fputs("\n", fp);
        fclose(fp);
    }
}


// search function
void search(struct Node* root, char word[]) {
    changeToLowerCase(word);
    if(check(word) == 0){
        printf("Please enter proper word!\n"); return;      // not a proper word
    }
    int isExist = 1;
    for(int i = 0; i < strlen(word); i++) {
        if(root->child[word[i] - 'a'] != NULL) {
            root = root->child[word[i] - 'a'];
        }
        else {
            isExist = 0;            // word does not exist
            break;
        }
    }
    if(isExist == 0 || root->endOfWord == 0) {
        printf("Word Not found\n");
    }
    else {
        printf("Word : %s\n", word);
        printf("Meaning : %s\n",root->meaning);     // word exists
    }
}

// DFS algo for searching related words
void dfs(struct Node* root, char curWord[], int curIndex) {
    if(root->endOfWord == 1) {
        printf("%s\n",curWord);
    }

    for(int i = 0; i < 26; i++) {
        if(root->child[i] != NULL) {
            curWord[curIndex] = (char)(i + 'a');
            curWord[curIndex + 1] = '\0';
            dfs(root->child[i], curWord, curIndex + 1);
            curWord[curIndex] = '\0';
        }
    }
}

// search function for related words
void RelatedWords(struct Node *root, char word[]) {
    changeToLowerCase(word);
    if(check(word) == 0){
        printf("Please enter proper word!\n"); return;      // not a proper word
    }
    int isExist = 1;
    for(int i = 0; i < strlen(word); i++) {
        if(root->child[word[i] - 'a'] != NULL) {
            root = root->child[word[i] - 'a'];
        }
        else {
            isExist = 0;
            break;
        }
    }
    if(isExist == 0) {
        printf("There is no word\n");
        return ;
    }
    char curWord[30];
    strcpy(curWord, word);
    int curIndex = strlen(curWord);

    dfs(root, curWord, curIndex);           // calling dfs function
}

void parseData(struct Node *root) {
    // Open the data file
    FILE *fp = fopen("Data.txt", "r");

    char word[30], meaning[100], c;
    int index = 0;

    // Parse word and it's meaning from data file and insert it to trie.
    while((c = fgetc(fp)) != EOF) {
        if(c != ' '){
            word[index] = c;
            index++;
        }
        else {
            word[index] = '\0'; index = 0;

            // find meaning of the current word.
            while((c = fgetc(fp)) == ' ' && c != EOF);
            meaning[index] = c; index++;

            while((c = fgetc(fp)) != '\n' && c != EOF){
                meaning[index] = c; index++;
            }
            meaning[index] = '\0';

            //Insert words and it's meaning to trie.
            insert(root, word, meaning, 0);

            index = 0;
        }
    }
    fclose(fp);
}


int main() {
    
    struct Node *root = createNode();   // creating root node
    for(int i = 0; i < 26; i++) {
        root->child[i] = NULL;      // initializing child with NULL
    }
    root->endOfWord = 0;

    parseData(root);    // Fetch data from file and insert it to trie.

    while(1) {
        int choice ;
        printf("1. Insert a Word\n2. Search meaning\n3. Search similar words\n4. Exit\n");      // User choice
        printf("Please enter your choice : ");
        while(1) {
            scanf("%d",&choice);
            if(choice >= 1 && choice <= 4){
                break;
            }
            else {
                printf("Please enter proper choice : ");
            }
        }

        if(choice == 1) {           // insertion of word
            printf("\n------------Insert------------\n");
            char word[30],meaning[100];
            printf("Enter the word : ");
            scanf("%s",word);
            printf("Enter the meaning : ");
            scanf("%s",meaning);
            insert(root, word, meaning, 1);
        }
        else if(choice == 2) {      // searching a particular word and its meaning
            printf("\n--------Search Meaning--------\n");
            char word[30];
            printf("Enter the word : ");
            scanf("%s",word);
            search(root, word);
        }
        else if(choice == 3) {         // searching related words
            printf("\n--------Similar Words --------\n");
            char word[30];
            printf("Enter the word : ");
            scanf("%s",word);
            RelatedWords(root, word);
        }
        else {
            printf("------------Thank you------------");
            break;
        }
        printf("\n");
    }

    return 0;
}
