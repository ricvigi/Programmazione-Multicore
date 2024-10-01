#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// functions
void helloword();
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

int main() {
    void* a;
    printf("%d\n", sizeof(a));
    return 0;
}

void helloworld(){
    printf("Hello, World\n");
}
void printaddress(long x) {
    long *a = &x;
    printf("%ld\n", a);
}
void floatcasting(unsigned int *x, unsigned int *y) {
    float z;
    z = (float) *x / *y;
    printf("z:%f\n", z);
}
void esercizio1(unsigned int *x, unsigned int *y) {
    unsigned int somma;
    unsigned int prodotto;
    if (*x > *y) {printf("x %d > y %d\n", *x, *y);}
    if (*x < *y) {printf("x %d < y %d\n", *x, *y);}
    if (*x == *y) {printf("x %d == y %d\n", *x, *y);}
    somma = *x + *y;
    prodotto = *x * *y;
    printf("somma: %d, prodotto: %d\n", somma, prodotto);
}
void esercizio2(unsigned int *a) {
    unsigned int i;
    for (i=0; i <= *a; i++) {
        printf("%u ", i);
    }
    printf("\n");
    for (i=0; i <= *a; i++) {
        printf("%u ", (*a - i));
    }
}
void esercizio3(int *a) {
    unsigned int i = 0;
    while (i <= *a) {
        printf("%u ", i);
        i++;
    }
    printf("\n");
    while (*a >= 0) {
        printf("%u ", *a);
        *a = *a - 1;
    }
    printf("\n");
}
void esercizioarray1(int *a, int *b, int *c, int *d, int *e) {
    int arr[10];
    int somma = 0;
    int min = 0;
    int max = 0;
    int i;
    arr[0] = *a;
    arr[1] = *b;
    arr[2] = *c;
    arr[3] = *d;
    arr[4] = *e;
    for (i = 0; i <= 4; i++) {
        if (arr[i] < min) {min = arr[i];}
        if (arr[i] > max) {max = arr[i];}
        somma = somma + arr[i];
    }
    printf("min: %d, max: %d, somma: %d", min, max, somma);
}
int eserciziostringa1() {
    char stringa[100];
    char *c;
    int stlen;
    c = &stringa[0];
    printf("Stringa in input: ");
    scanf("%s", stringa);
    stlen = strlen(stringa);
    if (stlen > 10) {
        printf("[*] ERROR: string must have <= 10 chars");
        return 1;
    }
    while (*c != '\0') {
        if ( (*c >= '0') && (*c <= '9') ) {
            *c = '*';
        }
        c++;
    }
    printf("\nStringa modificata: %s \n", stringa);
    return 0;
}
void eserciziopuntatori1() {
    char arr[10];
    int i = 0;
    printf("Stringa: ");
    scanf("%s", arr);
    char *p = &arr[0];
    while (*p != '\0') {
        if ( (*p>= '0') && (*p <= '9') ) {
            *p = '*';
        }
        p++;
    }
    printf("\nStringa modificata: %s \n", arr);
}
void eserciziopuntatori2(int arr1[], int arr2[]) {
    int *p1;
    int *p2;
    int i;
    p1 = &arr1[0];
    p2 = &arr2[4];
    for (i = 0; i < 5; i++){
        int a = *p1;
        *p1 = *p2;
        *p2 = a;
        p1++;
        p2--;
    }
    for (i = 0; i < 5; i++){
        printf("%d ", arr1[i]);
    }
    printf("\n");
    for (i = 0; i < 5; i++){
        printf("%d ", arr2[i]);
    }
}
int esercizioarray2(int arr[], int size, int *x) {
    int i;
    for (i = 0; i < size; i++) {
        if (arr[i] == *x) {
            return i;
        }
    }
    printf("[*] Error code 404\n");
    return 404;
}
int esercizioarray2modificato(int arr[], int size, int *x) {
    int i;
    int found = 0;
    for (i = 0; i < size; i++) {
        if (arr[i] == *x) {
            found++;
        }
    }
    if (found >= 1) {
        printf("[*] FOUND %d occurrences of value %d\n", found, *x);
        return 0;
    }
    printf("[*] Error code 404\n");
    return 404;
}
int included(int A[], int n, int B[], int m) {
    for (int i = 0; i < n; i++) {
        int j = 0;
        while (j < m && B[j] != A[i]) j++;
        if (j == m) return 0;
    }
    return 1;
}


