#include <stdio.h>

void main() {

    FILE *pF = fopen("baba.txt", "r");
    char buffer[255];

    while(fgets(buffer, 255, pF) != NULL) {    
        printf("%s", buffer);
    }
    fclose(pF);
}