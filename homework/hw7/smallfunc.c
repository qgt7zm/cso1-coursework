/*
 * Eric Weng
 * CS 2130 HW 7
 * Mon 10/31/22
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void capitalize(char *s);
void fibarray(unsigned char *dest, unsigned num);
long recpow(long x, unsigned char e); // x is positive, negative, or zero; e is positive or zero
unsigned long nextprime(unsigned long x);
int checkprime(unsigned long x); // helper functino for nextprime
void reverse(int *arr, unsigned length);

int main() {
    // 1. Capitalize
    char *s = strdup("the book \"The C Programming Language.\"");
    printf("before: %s\n", s);
    capitalize(s);
    printf("after:  %s\n", s);
    free(s);
    
    // 2. Fibonacci
    unsigned char a[64];
    fibarray(a, 64);
    for (int row=0; row<4; row+=1) {
        for (int col=0; col<16; col+=1)
            printf(" %02hhx", a[row*16 + col]);
        printf("\n");
    }
    
    // 3. Power
    printf("Power:\n");
    printf("%ld\n", recpow(0, 5));
    printf("%ld\n", recpow(1, 5));
    printf("%ld\n", recpow(10, 0));
    printf("%ld\n", recpow(10, 1));
    
    printf("%ld\n", recpow(2, 4));
    printf("%ld\n", recpow(2, 5));
    printf("%ld\n", recpow(-2, 4));
    printf("%ld\n", recpow(-2, 5));
    
    // 4. Next Prime
    printf("Next prime:\n");
    printf("%ld\n", nextprime(0));
    printf("%ld\n", nextprime(1));
    
    long x = 100;
    for (int i = 0; i < 10; i ++) {
        x = nextprime(x);
        printf("%ld\n", x);
    }
    printf("%ld\n", nextprime(1000000000000));
    
    // 5. Reverse
    printf("Reverse:\n");
    int y[] = {1, 1, 2, 3, 5, 8, 13, 21};
    for (int i = 0; i < 8; i++) printf("%d, ", y[i]); printf("\n");
    reverse(y, 6);
    for (int i = 0; i < 8; i++) printf("%d, ", y[i]); printf("\n");

    return 0;
}

void capitalize(char *s) {
    int i = 0;
    while (s[i]) {
        // lowercase from 97-122, subtract 32 to get uppercase
        if (s[i] >= 97 && s[i] <= 122) s[i] -= 32;
        i++;
    }
    return;
}

void fibarray(unsigned char *dest, unsigned num) {
    char prev = 0;
    char curr = 1;
    char next = prev + curr;
    for (int i = 0; i < num; i++) {
        dest[i] = curr % 256;
        prev = curr;
        curr = next;
        next = prev + curr;
    }
}

long recpow(long x, unsigned char e) {
    if (x == 0) return 0;
    else if (x == 1) return 1;
    else if (e == 0) return 1;
    else if (e == 1) return x;
    else return x * recpow(x, e - 1);
}

unsigned long nextprime(unsigned long x) {
    if (x <= 1) return 2; // first prime is 2
    int p = (x % 2) ? x + 2 : x + 1; // get next odd after x
    while (!checkprime(p)) { // stop when prime
        p += 2; // check all odd numbers after x
    }
    return p;
}

int checkprime(unsigned long x) {
    if (x == 0 || x == 1) return 0; // 0 and 1 are not prime
    if (x == 2) return 1; // 2 is prime
    if (x % 2 == 0) return 0; // evens are not prime
    for (unsigned long f = 3; f * f <= x; f += 2) { // check all odd factors from 3 to √x
        if (x % f == 0) return 0; // stop if divisible
    }
    return 1;
}

void reverse(int *arr, unsigned length) {
    int rev[length];
    for (int i = 0; i < length; i++) rev[i] = arr[length - 1 - i];
    for (int j = 0; j < length; j++) arr[j] = rev[j];
}