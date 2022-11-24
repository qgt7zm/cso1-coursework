#include <stdio.h>
#include <string.h>
#include <math.h> // for fabs()

int main(int argc, const char* argv[]) {
    // Decide what to add
    char choice;
    if (argc > 1) { // user entered 1 CL arg
        const char* flag = argv[1];
        // if (argv[1] == "-i") choice = 'I';
        // else if (argv[1] == "-f") choice = 'F';
        // else if (argv[1] == "-s") choice = 'S';
        if (!strcmp(flag, "-i")) choice = 'I';
        else if (!strcmp(flag, "-f")) choice = 'F';
        else if (!strcmp(flag, "-s")) choice = 'S';
        else {
            printf("Invalid flag supplied.\n");
            return 0;
        }
    } else {
        printf("What do you want to add (I/F/S)? ");
        scanf("%c", &choice);
    }

    // Add
    if (choice == 'I') {
        int int1;
        int int2;
        // Scan the ints
        printf("Enter first int: ");
        scanf("%d", &int1);
        printf("Enter second int: ");
        scanf("%d", &int2);
        // Add them together
        printf("%d + %d = %d\n", int1, int2, int1 + int2);
    } else if (choice == 'F') {
        float flt1;
        float flt2;
        // Scan the floats
        printf("Enter first float: ");
        scanf("%f", &flt1);
        printf("Enter second float: ");
        scanf("%f", &flt2);
        // Add them together
        float flt3 = flt1 + flt2;
        if (fabs(flt1) <= 1e-6f || fabs(flt2) <= 1e-6f || fabs(flt3) <= 1e-6f)
            printf("%e + %e = %e\n", flt1, flt2, flt3);
        else
            printf("%f + %f = %f\n", flt1, flt2, flt3);
    } else if (choice == 'S') {
        // Two strings of 10 chars + null
        char str1[11];
        char str2[11];
        char str3[22];
        // Scan the stirngs
        printf("Enter first string: ");
        scanf("%10s", str1);
        printf("Enter second string: ");
        scanf("%10s", str2);
        // Add them together
        strcpy(str3, str1);
        strcat(str3, str2);
        printf("%s + %s = %s\n", str1, str2, str3);
    } else {
        printf("You did not enter a valid choice.\n");
    }
    return 0;
}
