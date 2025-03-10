#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define N 26

// This is the structure of the trie
typedef struct trie 
{
    char c;                   
    struct trie* child[N];
    int is_end;
} trie;

trie* make_node(char a) 
{
	int i;
    trie* temp=(trie*)malloc(sizeof(trie));
    if (temp==NULL) 
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    for (i=0;i<N;i++) 
    {
        temp->child[i]=NULL;
    }
    temp->is_end=0;
    temp->c=a;
    return temp;
}

void add_the_node(trie* temp,char* data) 
{
	int i;
    for (i=0;data[i]!='\0';i++) 
    {
        int position=(int)data[i]-'a';
        if(temp->child[position]==NULL) 
        {
            temp->child[position]=make_node(data[i]);
        }
        temp=temp->child[position];
    }
    temp->is_end=1;
}

int search_trie(trie* root,char* word) 
{
    trie* temp=root;
    int i;
    for (i=0;word[i]!='\0';i++) 
    {
        int position=word[i]-'a';
        if (temp->child[position]==NULL)
        {
            return 0;
        }
        temp=temp->child[position];
    }
    return (temp!=NULL&&temp->is_end==1);
}

void word_from_dict_adder(trie* headroot) 
{
    FILE* fp=fopen("dic.txt", "r");
    if (fp==NULL) 
    {
        perror("Error opening dictionary file");
        exit(EXIT_FAILURE);
    }
    char word[50];
    while (fscanf(fp,"%s", word)!=EOF) 
    {
        add_the_node(headroot,word);
    }
    fclose(fp);
}

void suggestions_giver(trie* root, char* word) 
{
    char suggestions[50][50],pre[50];
    int suggestion_count=0;
    trie* temp=root;
    int i;
    for (i=0;word[i]!='\0';i++) 
    {
        int position=word[i]-'a';
        if(temp->child[position]==NULL) 
        {
            break;
        }
        pre[i]=word[i];
        temp=temp->child[position];
    }
    pre[i] ='\0';
    void dfs(trie* node,char* prefix,int length) 
    {
        if (node->is_end) 
        {
            strcpy(suggestions[suggestion_count],prefix);
            suggestion_count++;
        }
        int j;
        for (j=0;j<N;j++) 
        {
            if (node->child[j]) 
            {
                prefix[length]='a'+j;
                prefix[length+1]='\0';
                dfs(node->child[j],prefix,length + 1); 
            }
        }
    }
    dfs(temp,pre,i);
    if (suggestion_count>0) 
    {
        printf("Suggestions for %s:\n",word);
        int k;
        for (k=0;k<suggestion_count;k++) 
        {
            printf("%s\n",suggestions[k]);
        }
    } 
    else 
    {
        printf("No suggestions found.\n");
    }
}

void add_word_to_dic(trie* headroot,char* word)
{
	int i;
    add_the_node(headroot,word);
    FILE* fp=fopen("dic.txt","a");
    if (fp==NULL) 
    {
        perror("Error opening dictionary file");
        exit(EXIT_FAILURE);
    }
    fprintf(fp,"%s\n",word);
    fclose(fp);
    printf("The word '%s' has been added to the dictionary.\n",word);
}

void print_search(trie* root,char* word) 
{
    if (!search_trie(root,word))
    {
        printf("The word %s is not found in dictionary.\n",word);
        suggestions_giver(root,word);
        char ans;
        printf("Do you want to add this word to dictionary (Enter 'y' for yes and 'n' for no): ");
        scanf(" %c",&ans);
        if (ans=='y')
        {
            add_word_to_dic(root, word);
        }
        else if (ans=='n')
        {
            printf("\n\nYou can continue!!\n\n");
        }
    }
}

void work_tokenizer(trie* headroot) 
{
    char str[100];
    printf("Enter a sentence: ");
    scanf(" %[^\n]s", str);
    char delimiter[] =" ,.;:?!";
    char* token=strtok(str,delimiter);
    while (token) 
    {
        print_search(headroot,token);
        token=strtok(NULL,delimiter);
    }
}

void words_in_dic() 
{
    FILE* fp=fopen("dic.txt", "r");
    if (fp==NULL) 
    {
        perror("Error opening dictionary file");
        exit(EXIT_FAILURE);
    }
    char word[50];
    while (fscanf(fp,"%s",word)!=EOF) 
    {
    	int i;
        for (i=0;word[i];i++) 
        {
            word[i]=tolower(word[i]);
        }
        printf("%s\n",word);
    }
    fclose(fp);
}

void free_trie(trie* root) 
{
    if (root==NULL)
    {
        return;
    }
    int i;
    for (i=0;i<N;i++) 
    {
        free_trie(root->child[i]);
    }
    free(root);
}

int main() 
{
    int x;
    char word[50];
    trie* headroot=make_node('\0');
    word_from_dict_adder(headroot);
    do {
        printf("1. SPELL CHECKING PROGRAM\n2. WORDS IN DICTIONARY\n3. ADD A WORD TO THE DICTIONARY\n4. EXIT\n");
        printf("Enter the option: ");
        scanf("%d", &x);
        switch (x) {
            case 1:
                system("cls");
                work_tokenizer(headroot);
                printf("\n\nYou can continue!!\n\n");
                break;
            case 2:
                system("cls"); 
                words_in_dic();
                printf("\n\nYou can continue!!\n\n");
                break;
            case 3:
            	system("cls");
                printf("Enter a word to add to the dictionary: ");
                scanf("%s", word);
                add_word_to_dic(headroot, word);
                printf("\n\nYou can continue!!\n\n");
                break;
            case 4:
                free_trie(headroot);
                printf("Exiting the program\n");
                break;
            default:
                printf("\nEnter a valid option\n");
        }
    }while (x!=4);
    return 0;
}

