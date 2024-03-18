#include <stdio.h>
#include <stdlib.h>
#include <Receipt.h>
#include <Models.h>

int main(int argc, char** argv){

    if (argc == 2) {
        LoadReceipt(argv[1]);
        return 0;
    }

    printf("Incorrect number of arguments!\n");
    return -1;
}