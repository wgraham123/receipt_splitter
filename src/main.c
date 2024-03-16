#include <ReceiptLoader.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){

    if (argc == 2) {
        char** content;
        content = ReadReceipt(argv[1]);

        int i = 0;
        while (content[i]){
            printf("%s\n", content[i]);
            free(content[i]);
            i += 1;
        }

        free(content);
        return 0;
    }

    printf("Incorrect number of arguments!\n");
    return -1;
}