#ifndef ESERCIZI_H
#define ESERCIZI_H
void printaddress(long x);
void floatcasting(unsigned int *x, unsigned int *y);
void esercizio1 (unsigned int *x, unsigned int *y);
void esercizio2 (unsigned int *a);
void esercizio3 (int *a);
void esercizioarray1(int *a, int *b, int *c, int *d, int *e);
int esercizioarray2(int arr[], int size, int *x);
int esercizioarray2modificato(int arr[], int size, int *x);
void eserciziopuntatori1();
void eserciziopuntatori2(int arr1[], int arr2[]);
int included(int A[], int n, int B[], int m);
int eserciziostringa1();
#endif
#ifndef ESERCIZI_MALLOC_H
#define ESERCIZI_MALLOC_H
int esercizio_malloc1();
int esercizio_malloc2();
#endif
#ifndef ESERCIZI_STRUCT_H
#define ESERCIZI_STRUCT_H
int esercizio_struct1();
#endif

#ifndef AUTOMOBILE_STRUCT_H
#define AUTOMOBILE_STRUCT_H
typedef struct {
  float prezzo;
  char modello[30];
  int cilindrata;
  char colore[15];
} Automobile;
#endif
