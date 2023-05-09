#ifndef _MY_ARRAY_

#define _MY_ARRAY_

int **array( int, int );
int *vector( int );

void freeArray( int **, int );
void freeVector( int * );

void printArray( int **, int, int, char*);
void printVector( int *, int, char*);
#endif