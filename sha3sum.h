#ifndef SHA3SUM_H
#define SHA3SUM_H


#include <stdint.h>
#include <stddef.h>



typedef struct node{
          char  username[64];       // node's name string
          char  password[256];
   struct node *next;
}NODE;

void byte_to_hex(uint8_t b, char s[]);
int hashit(NODE *n);
int insertNew(NODE *root);
int insertNewPy(NODE *root, char* user, char* pass);
int insertHashed(NODE *root, char *user, char *pass);
void displayMenu();
void printAllNodes(NODE *root);
int login(NODE *root);
void export(NODE *root, const char *outfile);
int run(int choice, char* user, char* pass);


#endif
