#include <stdio.h>
#include <stdlib.h>
#include <Receipt.h>
#include <Models.h>

int main(int argc, char** argv){

    if (argc == 2) {
        Receipt *receipt = LoadReceipt(argv[1]);
        if (receipt == NULL)
            exit(EXIT_FAILURE);
        return 0;
    }

    printf("Incorrect number of arguments!\n");
    return -1;
}