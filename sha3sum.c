
/* -------------------------------------------------------------------------
 *  Nathan Bunge
 *  10/9/21
 * 
 *
 * Code based off of public domain code from Andrey Jivsov. crypto@brainhub.org - Jun 2018.
 * ---------------------------------------------------------------------- */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "sha3sum.h"
#include "sha3.h"


void byte_to_hex(uint8_t b, char s[23]) {
    unsigned i=1;
    s[0] = s[1] = '0';
    s[2] = '\0';
    while(b) {
        unsigned t = b & 0x0f;
        if( t < 10 ) {
            s[i] = '0' + t;
        } else {
            s[i] = 'a' + t - 10;
        }
        i--;
        b >>= 4;
    }
}


int hashit(NODE *n)
{
    sha3_context c;
    const uint8_t *hash;
    int image_size;

    int fd;
    struct stat st;
    void *p;
    unsigned i;
    unsigned use_keccak = 0;

    image_size = 256;


    //strcpy(file_path, "temphash.txt");
    

    fd = open("temphash.txt", O_RDONLY);
    //fd = fopen("temphash.txt", "r");
    if( fd == -1 ) {
	    printf("Cannot open file for reading");
	    return 2;
    }
    i = fstat(fd, &st);
    if( i ) {
	    close(fd);
	    printf("Cannot determine the size of file");
	    return 2;
    }

    p = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
    close(fd);
    //fclose(fd);
    if( p==NULL ) {
	    printf("Cannot memory-map file");
	    return 2;
    }

    switch(image_size) {
	case 256:
    		sha3_Init256(&c);
		break;
	case 384:
    		sha3_Init384(&c);
		break;
	case 512:
    		sha3_Init512(&c);
		break;
    }

    if( use_keccak ) {
        enum SHA3_FLAGS flags2 = sha3_SetFlags(&c, SHA3_FLAGS_KECCAK);
        if( flags2 != SHA3_FLAGS_KECCAK )  {
	    printf("Failed to set Keccak mode");
            return 2;
        }
    }


    sha3_Update(&c, p, st.st_size);
    hash = sha3_Finalize(&c);

    munmap(p, st.st_size);

    for(i=0; i<image_size/8; i++) 
    {
	    char s[3];
	    byte_to_hex(hash[i],s);
	    //printf("%s", s);
        strcpy(n->password+(i*2), s);
    }

    //delete contents of file
    char *temp = "\0";
    FILE* fp;
    fp = fopen("temphash.txt", "w");
    //fwrite(line, 1, len, fp);
    fprintf(fp, temp, sizeof(temp));
    fclose(fp);
    return 1;

}

int insertNewPy(NODE *root, char* user, char* pass)
{
    //NEED TO CHECK IF ALREADY HAS THAT USERNAME

    printf("in insertnewpy\n");
 

    //printf("User: %s, Password: %s\n", user, pass);

    
    //open file to overwrite
    FILE* fp;
    fp = fopen("temphash.txt", "w");
    //fwrite(line, 1, len, fp);
    fprintf(fp, pass, sizeof(pass));
    fclose(fp);

    //set to head if empty
    if(root->username == NULL)
    {
        strcpy(root->username, user);
        hashit(root);
        return 4;
    }

    //set to new node elsewise
    NODE *p;
    p = root;
    while(p->next !=NULL)
    {
        //printf("Next Node...\n");
        p = p->next;
    }
    struct node *new = (struct node*) malloc(sizeof(struct node));
    new->next = NULL;
    strcpy(new->username, user);
    hashit(new);

    p->next = new;

    return 4;
}

int insertNew(NODE *root)
{
    char * line = NULL;
    size_t len = 0;


    char *tempUser='\0';

    //get user input
    printf("Enter Username: ");
    getline(&line, &len, stdin);
    while(line[0] ==' ')
    {
        //printf("deated space\n");
        line = line+1;
    }
    if(line[strlen(line)-1] == '\n')
    {
        //printf("deleated new line\n");
        line[strlen(line)-1] = '\0';
    }
    strcpy(tempUser,line);

    //get password
    printf("Enter password:");
    getline(&line, &len, stdin);

    if(line[strlen(line)-1] == '\n')
    {
        //printf("deleated new line\n");
        line[strlen(line)-1] = '\0';
    }

    //open file to overwrite
    FILE* fp;
    fp = fopen("temphash.txt", "w");
    //fwrite(line, 1, len, fp);
    fprintf(fp, line, sizeof(line));
    fclose(fp);

    //set to head if empty
    if(root->username == NULL)
    {
        strcpy(root->username, tempUser);
        hashit(root);
        return 2;
    }

    //set to new node elsewise
    NODE *p;
    p = root;
    while(p->next !=NULL)
    {
        //printf("Next Node...\n");
        p = p->next;
    }
    struct node *new = (struct node*) malloc(sizeof(struct node));
    new->next = NULL;
    strcpy(new->username, tempUser);
    hashit(new);

    p->next = new;

    return 1;
}


int insertHashed(NODE *root, char *user, char *pass)
{
    //printf("Passordtest: %s same line\n", pass);
    if(root->username[0] == '\0')
    {
        printf("head case!!!\n");
        strcpy(root->username, user);
        strcpy(root->password, pass);
        return 2;
    }
    
    NODE *p;
    p = root;
    while(p->next !=NULL)
    {
        //printf("Next Node...\n");
        p = p->next;
    }
    struct node *new = (struct node*) malloc(sizeof(struct node));
    new->next = NULL;
    
    strcpy(new->username, user);
    strcpy(new->password, pass);
    printf("insrted new node\n");
    p->next = new;
    return 1;
}
void displayMenu()
{
    printf("1) Login\n");
    printf("2) New User\n");
    printf("3) Exit\n");

}

void printAllNodes(NODE *root)
{
    printf("\nPrinting all Nodes: \n\n");
    NODE *p;
    p = root;

    while(p->next != NULL)
    {
        printf("Username: %s\nPassword: %s\n", p->username, p->password);
        p = p->next;
    }
    printf("Username: %s\nPassword: %s\n", p->username, p->password);
}

int pyLogin(NODE *root, char* user, char* pass)
{
    printf("loggin in:\n");
    //printf("User: %s, Password: %s\n", user, pass);



    //open file to overwrite
    FILE* fp;
    fp = fopen("temphash.txt", "w");
    //fwrite(line, 1, len, fp);
    fprintf(fp, pass, sizeof(pass));
    fclose(fp);

   

   
    struct node *new = (struct node*) malloc(sizeof(struct node));
    new->next = NULL;
    
    strcpy(new->username, user);
    hashit(new);
    printf("\n\n");
    //search username
    NODE *p;
    p = root;
    while(p->username !=NULL)
    {
        //printf("Searching node user: %s Password: %s\n", p->username, p->password);
        if(strcmp(p->username,new->username)==0){
            if(strcmp(p->password,new->password)==0){
                printf("Login success!\n");
                return 1;
            }
            printf("Wrong password\n");
            return 2;
        }
        
        p = p->next;
    }
    printf("Sorry, no account under that username\n");
    free(new);
    return 3;
}

int login(NODE *root)
{
    printf("loggin in:\n");
    char * line = NULL;
    size_t len = 0;


    char *tempUser = '\0';

    //get user input
    printf("Enter Username: ");
    getline(&line, &len, stdin);
    while(line[0] ==' ')
    {
        //printf("deated space\n");
        line = line+1;
    }
    if(line[strlen(line)-1] == '\n')
    {
        //printf("deleated new line\n");
        line[strlen(line)-1] = '\0';
    }
    strcpy(tempUser,line);

    //get password
    printf("Enter password:");
    getline(&line, &len, stdin);

    if(line[strlen(line)-1] == '\n')
    {
        //printf("deleated new line\n");
        line[strlen(line)-1] = '\0';
    }

    //open file to overwrite
    FILE* fp;
    fp = fopen("temphash.txt", "w");
    //fwrite(line, 1, len, fp);
    fprintf(fp, line, sizeof(line));
    fclose(fp);

   

   
    struct node *new = (struct node*) malloc(sizeof(struct node));
    new->next = NULL;
    
    strcpy(new->username, tempUser);
    hashit(new);
    printf("\n\n");
    //search username
    NODE *p;
    p = root;
    while(p->username !=NULL)
    {
        //printf("Searching node user: %s Password: %s\n", p->username, p->password);
        if(strcmp(p->username,new->username)==0){
            if(strcmp(p->password,new->password)==0){
                printf("Login success!\n");
                return 1;
            }
            printf("Wrong password\n");
            return 2;
        }
        
        p = p->next;
    }
    printf("Sorry, no account under that username\n");
    free(new);
    return 3;
    

    

}

//save all usernames and passwords to a file
void export(NODE *root, const char *outfile)
{
    //printf("Exsporting...\n");

    FILE* fp;
    fp = fopen(outfile, "w");
    char*space = " ";
    char*new = "\n";

    //int fd;
    //fd = open(outfile, O_CREAT | O_WRONLY);

    NODE *p;
    p = root;
    while(p->username !=NULL)
    {
        fprintf(fp, p->username, sizeof(p->username));
        fprintf(fp, space, 1);
        fprintf(fp, p->password, sizeof(p->password));
        fprintf(fp, new, 1);
        p = p->next;
    }

    fclose(fp);
    //close(fd);
}


int run(int choice, char* user, char* pass)
{
    //should copy to new string to avoid and garbage collection?

    printf("user: %s", user);
    printf("Passowrd: %s", pass);

    //import all from file
    const char *outfile_path;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char seporator = ' ';

    struct node *head = (struct node*) malloc(sizeof(struct node));
    head->next = NULL;
    //strcpy(head->username, "\0");
    head->username[0] = '\0';

    outfile_path = "accounts.txt";


    FILE* fp;

    fp = fopen(outfile_path, "r");

    while ((read = getline(&line, &len, fp)) != -1){
    //printf("Retrieved line of length %zu:\n", read);
    //printf("%s", line);

    //token = strtok(line, " ");
    //strcpy(head->username, token);

    //deleat any leading spaces
    while(line[0] ==' ')
        {
            //printf("deated space\n");
            line = line+1;
        }
    //deleat newline character
    if(line[strlen(line)-1] == '\n')
    {
        //printf("deleated new line\n");
        line[strlen(line)-1] = '\0';
    }

    //for each line, create a new node, set username, set password
        char * const sep_at = strchr(line, seporator);
        *sep_at = '\0';
        printf("Username: %s\nPasswork: %s\n", line, sep_at+1);

        insertHashed(head, line, sep_at+1);
    }

    //main menu

    fclose(fp);



/*************************
 * return messages:
 *  0: normal?
 *  1: successful login
 *  2: failed login (wrong password)
 *  3: failed login (wrong username)
 *  4: success insert
 *  5: failed insert (already has username)
 * 
 * 
 * **********************/
    int result = 0;
    //printAllNodes(head);
    if(choice != 3)
    {
        switch (choice){
            case 1: result = (pyLogin(head, user, pass)); break;
            case 2: result =(insertNewPy(head, user, pass)); break;
            case 4: printAllNodes(head); break;
        }
        
    }
    //printAllNodes(head);
    export(head, outfile_path);
    printf("returning: %d", result);
    return result;
 

}
//test






//hash
//input string, open file and overwrite string, close file, run hashing, return hash string? (maybe assign pointer)

//hash function



/*
    sha3_context c;
    const uint8_t *hash;
    int image_size;
    const char *file_path;
    
    int fd;
    struct stat st;
    void *p;
    unsigned i;
    unsigned use_keccak = 0;

    if( argc != 3 && argc != 4 ) {
	    help(argv[0]);
	    return 1;
    }

    image_size = atoi(argv[1]);
    switch( image_size ) {
	case 256:
	case 384:
	case 512:
		break;
	default:
		help(argv[0]);
		return 1;
    }

    file_path = argv[2];
    outfile_path = argv[3];

    if( argc == 4 && file_path[0] == '-' && file_path[1] == 'k' )  {
        use_keccak = 1;
        file_path = argv[3];
        outfile_path = argv[4];
    }

    if( access(file_path, R_OK)!=0 ) {
	    printf("Cannot read file '%s'", file_path);
	    return 2;
    }

    fd = open(file_path, O_RDONLY);
    if( fd == -1 ) {
	    printf("Cannot open file '%s' for reading", file_path);
	    return 2;
    }
    i = fstat(fd, &st);
    if( i ) {
	    close(fd);
	    printf("Cannot determine the size of file '%s'", file_path);
	    return 2;
    }

    p = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
    close(fd);
    if( p==NULL ) {
	    printf("Cannot memory-map file '%s'", file_path);
	    return 2;
    }

    switch(image_size) {
	case 256:
    		sha3_Init256(&c);
		break;
	case 384:
    		sha3_Init384(&c);
		break;
	case 512:
    		sha3_Init512(&c);
		break;
    }

    if( use_keccak ) {
        enum SHA3_FLAGS flags2 = sha3_SetFlags(&c, SHA3_FLAGS_KECCAK);
        if( flags2 != SHA3_FLAGS_KECCAK )  {
	    printf("Failed to set Keccak mode");
            return 2;
        }
    }







    //enter username
    //enter password

    //run hash
    //add to linked list (no oder)
    //save to file



    sha3_Update(&c, p, st.st_size);
    hash = sha3_Finalize(&c);

    munmap(p, st.st_size);

*/